// 
//  
//  Copyright (C) 2008 Autodesk Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include "stdafx.h"
#include "MappedFile.h"

#include <stdio.h>
#include <io.h>
#include <memory.h>
#include <malloc.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>


//////////////////////////////////////////////////////////////////////////
//
// Memory mapped file
// 
//////////////////////////////////////////////////////////////////////////


MappedFile::MappedFile(size_t szData, size_t szPage, int numPages)
: _szData(szData),
  _numPages(numPages),
  _fd(0),
  _handle(INVALID_HANDLE_VALUE),
  _pgHead(NULL),
  _canwrite(0),
  _cache_misses_L1(0),
  _cache_misses_L2(0),
  _cache_callcount(0),
  _szFile(0),
 _bPagesLocked(false)
{
    //page size must be a power of 2
    if (!szPage || (szPage & (szPage - 1)))
        throw "Page size must be a power of 2!";

    //data record size must be a power of 2
    if (!szData || (szData & (szData - 1)))
        throw "Data record size must be a power of 2!";

    _szPage = szPage;
    _szAllPages = _szPage * _numPages;

    //mask that converts a given pointer to a starting offset for the
    //pointer's memory page
    _pageMask = -(size_type)_szPage;    


    //compute bit shift to use for multiplying by data record size
    _iszDataShift = -1;
    do
    {
        _iszDataShift ++;
        szData = szData >> 1;
    }
    while(szData);

    _pages = new MemoryPage[_numPages];

    //initialize the memory cache pages
    for (int i=0; i<_numPages; i++) 
    {
        _pages[i].p = 0;
        // _pages[i].p = (unsigned char*) malloc(_szPage);
        _pages[i].offset = (IndexType)-1;
        _pages[i].refcount = 0;
        _pages[i].size = 0;
        
        //set up a circular queue-like linked list of pages also
        //we will use it to maintain a most-recently accessed order
        if (i == 0)
            _pages[i].pprev = &_pages[_numPages-1];
        else
            _pages[i].pprev = &_pages[i - 1];

        if (i == _numPages - 1)
            _pages[i].pnext = &_pages[0];
        else
            _pages[i].pnext = &_pages[i+1];
    }

    _pgHead = &_pages[0];
}

MappedFile::~MappedFile()
{
    close();

    delete [] _pages;
    _pages = 0;

	DeleteFile(m_strFileName.c_str());
}


HANDLE MappedFile::create(const wchar_t* name)
{
    _canwrite = 1;
    return openInternal( name, false);
}

HANDLE MappedFile::open(const wchar_t* name, bool readOnly/* = false*/)
{
    _canwrite = readOnly ? 0 : 1;
    return openInternal( name, true);
}

HANDLE MappedFile::openInternal(const wchar_t* name, bool open)
{
	// save the name
	m_strFileName = name;

    //acquire file handle
    _handle = CreateFile( 
        name, 
        _canwrite ? GENERIC_READ|GENERIC_WRITE : FILE_READ_DATA, 
        FILE_SHARE_READ, 
        0, 
        open ? OPEN_EXISTING : CREATE_ALWAYS,
        FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_SEQUENTIAL_SCAN, 
        0);
    
    if ( _handle == INVALID_HANDLE_VALUE)
    {
        //only flag the error if a Create failed, it is ok to fail
        //if we are only trying to open to see if the file exists.
        if (!open)
            error( L"CreateFile");
        return _handle;
    }

    //OK, we successfully opened or created the file, now create the file
    //memory mapping if the file already existed -- otherwise it is an empty file
    //and we will be managing memory pages manually.
    //TODO: What if the file exists, but is zero size? Will CreateFileMapping crash then?
    
    if (open)
    {
        _numrecs = numrecsInternal();
		if (_numrecs  > 0)
		{
			_fd = CreateFileMapping( 
				_handle, 
				NULL, 
				_canwrite ? PAGE_READWRITE|SEC_COMMIT : PAGE_READONLY|SEC_RESERVE,  
				0, 
				0,
				0); 

			if ( !_fd)
				error( L"CreateFileMapping");
		}
    }
    else
    {
        _numrecs = 0;
    }

    _szFile = (IndexType)(_szData * _numrecs);
    _szFileOriginal = _szFile;

    return _fd;
}
    
HANDLE MappedFile::close()
{
    //close the file itself
    if ( _handle != INVALID_HANDLE_VALUE)
    {
        //close mapping stuff
        closeMapping();

        //close the file mapping
        CloseHandle( _handle);
        _handle = INVALID_HANDLE_VALUE;
    }

    return _handle;
}

void MappedFile::closeMapping()
{
    //unmap or write out all used pages
    if ( _pages)
    {
        MemoryPage* page = _pgHead;
        do 
        {
            if ( page->mapped)
                munmappage(page);
            else
                writepage(page, true);

            page->offset = (IndexType)-1;
            page->refcount = 0;
        }
        while ((page = page->pnext) != _pgHead);
    }

    //close the file mapping object
    if ( _fd != 0)
    {
        CloseHandle( _fd);
        _fd = 0;
    }
}


//given a page offset within the file, checks if the page is
//already mapped in memory -- if yes it returns it, and if not,
//it swaps out a page from the cache and maps in the one we want
//from disk
MemoryPage* MappedFile::load_page(IndexType pageoff)
{
#ifdef CACHE_STATS
    //guess we are going to have a mild cache miss 
    //meaning record is not on the head page but is mapped
    _cache_misses_L1++; 
#endif

    //check the cache
    MemoryPage* page = _pgHead;
    MemoryPage* tmp = page;
    do 
    {
		if (page->offset == pageoff) 
            goto rewire_cache;
    }
    while ((page = page->pnext) != tmp);

    //if we reach here, means we have to load the requested page
    //from file into the cache

    //let's find an unused memory page we can swap out
    //iterate backwards so that we hit the first (most recently used) page last
    page = tmp->pprev;
    tmp = page; //loop sentinel is the last page rather than the head page -- we want to hit the head page in the loop, not terminate
    do
    {
        if (page->refcount == 0) break;
    }
    while ((page = page->pprev) != tmp);

#if 0
    //TODO: it is possible that a free page was not found -- this case needs to be 
    //handled here
    if (page == tmp)
        error(L"No free surface memory pages available. Release some!");
#endif

#ifdef CACHE_STATS
    _cache_misses_L1--;
    _cache_misses_L2++; //the worst kind of cache miss -- it will require swapping
#endif

    //load data from the file into the empty page
    swappage(page, pageoff);

    if (page == _pgHead) return page;

rewire_cache:
    //we have a page with the required data, now do administrative and
    //cache maintenance work

    //put the page at the head of the page linked list in order to maintain
    //the most recently used order

    //yank the page from its current place in the page circular linked list
    MemoryPage* pn = page->pnext;
    MemoryPage* pp = page->pprev;

    pp->pnext = pn;
    pn->pprev = pp;

    //now insert it at the head of the linked list
    pn = _pgHead;
    pp = pn->pprev;
    page->pnext = pn;
    page->pprev = pp;
    pp->pnext = page;
    pn->pprev = page;
    
    _pgHead = page;

    return page;
}

//called when load() is called with an index that will grow the file (i.e. index >= numrecs).
//This function increments the record count and also if the file has grown by a certain amount,
//will close and reopen the file in order to allow for a bigger portion of the file to be 
//mapped in memory using MapViewOfFile rather than ReadFile
void MappedFile::grow(IndexType index, MemoryPage* page, IndexType pageoff)
{
    //increment record count
    _numrecs = index+1;
    
    //also if we have grown a lot beyond _szFileOriginal, we should
    //close and reopen the file, so that we can use Windows memory mapping
    //for the section of the file that was added

    if (!_bPagesLocked && ((_numrecs << _iszDataShift) - _szFileOriginal > _szAllPages))
    {
        //reopen the file -- this will unload all in-memory cache
        close();
        open(m_strFileName.c_str(), !_canwrite);

        //load back the page that we were working with (so that caller doesn't
        //know what evil thing we did here)
        swappage(page, pageoff);
    }
}

//writes out (or unmaps) data that is in the given page and then
//loads data from the given offest into the page
void MappedFile::swappage(MemoryPage* page, IndexType pageoff)
{
    //in an attempt to minimize mallocs, we try to reuse space if memory
    //mapping can't be used on both pages of interest.
    //TODO: What if the file is 0 bytes and open for read only? It will crash...
    bool willmap = !_canwrite || (_szFileOriginal > pageoff && _szFileOriginal - pageoff >= _szPage);

    //unmap or write out the page to file
    if ( page->mapped)
        munmappage(page);
    else
        writepage(page, willmap);

    //use Windows memory mapping if the entire page is within the file
    //or if it is read-only
    if ( willmap)
        mmappage(page, pageoff);
    else
        readpage(page, pageoff);
}

//writes out a page that is managed by us (i.e. is not memory mapped by Windows)
void MappedFile::writepage(MemoryPage* page, bool freeMem)
{
    if (!page->size) 
        return;

    DWORD written = 0;

    if (_canwrite)
    {
        //determine how many bytes to physically write -- if it is the
        //last page, it may not have _szPage bytes actually in use
		//ASSERT(  (_numrecs * _szData - page->offset) < static_cast<size_t>(-1) );	// ASSERT writeSize fits in a 32-bit DWORD
        size_type sizeToCopy = _numrecs * _szData - page->offset; 
        sizeToCopy = (size_type)(min (sizeToCopy, page->size));

        LARGE_INTEGER offset;
        offset.QuadPart = page->offset;
               
        SetFilePointer( _handle, offset.LowPart, &offset.HighPart, FILE_BEGIN);
        if ( !WriteFile( _handle, page->p, sizeToCopy, &written, 0))
            error( L"WriteFile");
    }

    //for manually managed pages, it is not necessary to set the size to 0
    //after a write, since the page is still coherent. A non-zero size
    //will not prevent us from loading a new chunk into the page from swappage()
    //page->size = 0;
    
    if (freeMem)
    {
        VirtualFree( page->p, 0, MEM_RELEASE);
        page->p = 0;
        page->size = 0;
    }

    //update file size if the page expanded the file
    _szFile = max(_szFile, page->offset + written);
}

//reads in a page that we will manage ourselves (i.e. is not memory mapped by Windows)
void MappedFile::readpage(MemoryPage* page, IndexType pageoff)
{
    if ( !page->p)
        page->p = (unsigned char*) VirtualAlloc(page->p, _szPage, MEM_COMMIT, PAGE_READWRITE);

    LARGE_INTEGER offset;
    offset.QuadPart = pageoff;
    SetFilePointer( _handle, offset.LowPart, &offset.HighPart, FILE_BEGIN);

    DWORD sizeToRead = (DWORD)(_numrecs * _szData - pageoff);
    if ( sizeToRead > _szPage)
        sizeToRead = (DWORD)_szPage;
    DWORD sizeRead;
    ReadFile( _handle, page->p, sizeToRead, &sizeRead, 0);

    page->size = (IndexType)_szPage;
    page->mapped = false;
    page->offset = pageoff;
    page->refcount = 0;
}

//loads a page using Windows memory mapping -- Windows will manage the memory
void MappedFile::mmappage(MemoryPage* page, IndexType pageoff)
{
    size_t thisPageSize = min(_szPage, _szFile - pageoff); 

    LARGE_INTEGER offset;
    offset.QuadPart = pageoff;

    page->p = (unsigned char *)MapViewOfFileEx( 
        _fd, 
        _canwrite ? FILE_MAP_WRITE : FILE_MAP_READ,  
        offset.HighPart, 
        offset.LowPart, 
        thisPageSize, 
        0);

    if ( !page->p)
    {
        error( L"MapViewOfFile");
        return;
    }

    page->mapped = true;
    //page->size = (IndexType)_szPage;
    page->size = (IndexType)thisPageSize;
    page->offset = pageoff;
    page->refcount = 0;
}

void MappedFile::munmappage(MemoryPage* page)
{
    if (!page->size) 
        return;

    UnmapViewOfFile( page->p);
    page->size = 0;
    page->p = 0;
    page->offset = (IndexType)-1;
}

//decrements the refcount of the cache page that the given pointer belongs to 
//once the refcount is 0, i.e. nobody is using any of this page's data,
//the page can be reused
void MappedFile::release(void* ptr)
{
    MemoryPage* page = _pgHead;

    do 
    {
        size_type diff = (size_type)((unsigned char*)ptr - page->p);

        if (diff >= 0 && diff < (size_type)page->size)
        {
            page->refcount--;
            return;
        }
    } 
    while ((page = page->pnext) != _pgHead);

    throw "Page was released before we were done using it!!!";
}

IndexType MappedFile::numrecsInternal()
{
    LARGE_INTEGER  offset;
    GetFileSizeEx( _handle, &offset);
    return (IndexType)(offset.QuadPart / _szData);
}

//writes out all memory pages to disk
void MappedFile::flushall()
{
    //flush all mapped pages
    MemoryPage* page = _pgHead;
    do 
    {
        if ( page->size) 
        {
            if (page->mapped)
                FlushViewOfFile( page->p, page->size); 
            else
			{
                writepage(page, false);
				FlushFileBuffers(_handle);
			}
        }
    }
    while ((page = page->pnext) != _pgHead);
}


//if the file has grown, it will close it and then reopen it, so that
//we can use MapViewOfFile on records that have been added since the last open
//This is similar to grow() except that it doesn't preload a MemoryPage
//and is not as lax about how much the file must have grown in order to
//initiate the reopen. The assumption is this gets called rarely before
//large read operations after operations that have grown the file
void MappedFile::syncsize()
{
    if ( (_numrecs << _iszDataShift) - _szFileOriginal >= _szPage)
    {
        //reopen the file -- this will unload all in-memory cache
        close();
        open(m_strFileName.c_str(), !_canwrite);
    }
}

void MappedFile::empty()
{
	//
	// clear out the file contents -- set the file length to zero
	//

	closeMapping();

	SetFilePointer(_handle, 0, NULL, FILE_BEGIN);
	SetEndOfFile(_handle);

	_szFile = _szFileOriginal = _numrecs = 0;
}

void MappedFile::error( const wchar_t* operation)
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD error = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)operation)+40)*sizeof(TCHAR)); 
    wsprintf((LPTSTR)lpDisplayBuf, L"%s failed with error %d: %s", operation, error, reinterpret_cast<LPCTSTR>(lpMsgBuf)); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, L"Error", MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}
