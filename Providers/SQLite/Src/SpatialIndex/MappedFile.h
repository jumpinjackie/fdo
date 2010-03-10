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

#ifndef MAPPEDFILE_H
#define MAPPEDFILE_H

#include <windows.h>

typedef unsigned int IndexType;
typedef unsigned int size_type;

//#define CACHE_STATS 1

struct MemoryPage
{
    MemoryPage()
        : startIndex((IndexType)-1),
          itemCount(0),
          p(0),
          refcount(0)
    {
    }

    unsigned char* p;
    IndexType startIndex;
    IndexType itemCount;    
    int refcount;
    MemoryPage* pnext;
    MemoryPage* pprev;
	bool mapped;
};

template <typename T, int recsPerPage>
class MappedFile
{
public:

    MappedFile(int numPages);
    ~MappedFile();

    HANDLE create(const wchar_t* name, bool deleteFile = true);
    HANDLE open(const wchar_t* name, bool readOnly = false);
    HANDLE close();
    HANDLE reopen();

    //variaous ways to get a pointer to a record, depending on what
    //we need it for
    T* load(IndexType index);
    T* load_noaddref(IndexType index);
    T* load_newrec(IndexType index);
    
    void release(void* ptr);
    void flushall();
    void syncsize();
	void empty();

    IndexType numrecs() { return _numrecs; }

    HANDLE fd() { return _fd; }

	void copyTo(const wchar_t* dest);
	std::wstring createTempFile(const std::wstring &strPrefix);

    typedef bool(* EnumPagesCallback)(void* pBase, IndexType iBase, IndexType nRecs, void* pUserData);
    bool enumPages(EnumPagesCallback pCallback, void* pUserData);

	bool MappedFile::samePage(IndexType i1, IndexType i2);

	static std::wstring getTempFileName(const std::wstring &strPrefix);

private:

    MemoryPage* __fastcall load_page(IndexType index);


    void swappage(MemoryPage* page, IndexType pageoff);

    void writepage(MemoryPage* page, bool freeMem = false);
    void readpage(MemoryPage* page, IndexType pageoff);

    void mmappage(MemoryPage* page, IndexType pageoff);
    void munmappage(MemoryPage* page);

    void grow(IndexType index, MemoryPage* page);


    HANDLE openInternal(const wchar_t* name, bool open);
    IndexType numrecsInternal();
	void closeMapping();
	void error( const wchar_t* operation);

private:

    MemoryPage* _pgHead;
    MemoryPage* _pages;

    HANDLE _fd;
	HANDLE _handle;
    int _canwrite;

    //the are constant once the MappedFile is initialized
    int _numPages;
    size_t _numRecsAllPages;

    //these vary as the file grows
	IndexType _numrecs; //logical number of records
    IndexType _numRecsOriginal; //number of records when the file was originally opened
    IndexType _numRecsFile; //physical number of records allocated in the file

	std::wstring m_strFileName;
	bool _bPagesLocked;
    bool _bDeleteFile;

public:
    size_t _cache_callcount;
    size_t _cache_misses_L1;
    size_t _cache_misses_L2;

	size_t _cache_hits, _cache_hitsHead;
	size_t _numQueries, _numPagesSearched;
	int _maxPagesSearchedForQuery;
};

//
//
// Implementation of inline functions for MappedFile
//
//

//loads a given record into a memory page -- will load a whole page
//and not just the requested item. You need to call release()
//with the returned pointer as argument once done using
//the record that the pointer refers to.
template <typename T, int recsPerPage>
inline T* MappedFile<T, recsPerPage>::load(IndexType index)
{
#ifdef CACHE_STATS
    _cache_callcount++;
#endif

    //index of the record within its corresponding memory page page
    IndexType pageIndex = index % recsPerPage;

    //get the page that has the record
    MemoryPage* page = _pgHead;
    IndexType baseIndex = index - pageIndex;
	if (page->startIndex != baseIndex) 
	{
        page = load_page(index);
	}
	else 
    {
#ifdef CACHE_STATS
		_cache_hitsHead++;
#endif
    }		

    //refcount the page and return the correct pointer into page data
    page->refcount ++;
    return &(((T*)page->p)[index % recsPerPage]);
}

//same as load() but does not addref. Useful when we only need
//the pointer for a short time and are done with it before we request
//another pointer
template <typename T, int recsPerPage>
inline T* MappedFile<T, recsPerPage>::load_noaddref(IndexType index)
{
#ifdef CACHE_STATS
    _cache_callcount++;
#endif

    //index of the record within its corresponding memory page page
    IndexType pageIndex = index % recsPerPage;

    //get the page that has the record
    MemoryPage* page = _pgHead;
    IndexType baseIndex = index - pageIndex;
	if (page->startIndex != baseIndex) 
	{
        page = load_page(index);
	}
	else 
    {
#ifdef CACHE_STATS
		_cache_hitsHead++;
#endif
    }		

    //return the pointer to the record -- we are not addreffing here
    return &(((T*)page->p)[pageIndex]);
}

//same as load() but used when adding new records to the file -- it will
//perform file resizing logic (increment record count, flush the file if needed).
//This function adds ref.
template <typename T, int recsPerPage>
inline T* MappedFile<T, recsPerPage>::load_newrec(IndexType index)
{
#ifdef CACHE_STATS
    _cache_callcount++;
#endif

    //index of the record within its corresponding memory page page
    IndexType pageIndex = index % recsPerPage;

    //get the page that has the record
    MemoryPage* page = _pgHead;
    IndexType baseIndex = index - pageIndex;
	if (page->startIndex != baseIndex) 
	{
        page = load_page(index);
	}
	else 
    {
#ifdef CACHE_STATS
		_cache_hitsHead++;
#endif
    }		

    //grow the file, refcount the page, return the pointer
    if ( index >= _numrecs)
        grow(index, page);    
    page->refcount ++;
    return &(((T*)page->p)[index % recsPerPage]);
}

#pragma warning (disable: 4127)
template <typename T, int recsPerPage>
MappedFile<T, recsPerPage>::MappedFile(int numPages)
: _numPages(numPages),
  _fd(0),
  _handle(INVALID_HANDLE_VALUE),
  _pgHead(NULL),
  _canwrite(0),
  _cache_misses_L1(0),
  _cache_misses_L2(0),
  _cache_callcount(0),
  _numrecs(0),
  _numRecsFile(0),
  _bPagesLocked(false),
  _cache_hits(0),
  _cache_hitsHead(0),
  _numQueries(0),
  _numPagesSearched(0),
  _maxPagesSearchedForQuery(0),
  _bDeleteFile(true)
{
    //page size must be a power of 2
    if (!recsPerPage || (recsPerPage & (recsPerPage - 1)))
        throw L"Items per page must be a power of 2!";

    //data record size must be a power of 2
    if ((sizeof(T) & (sizeof(T) - 1)))
        throw L"Data record size must be a power of 2!";

    _numRecsAllPages = recsPerPage * _numPages;

    _pages = new MemoryPage[_numPages];

    //initialize the memory cache pages
    for (int i=0; i<_numPages; i++) 
    {
        _pages[i].p = 0;
        _pages[i].startIndex = (IndexType)-1;
        _pages[i].refcount = 0;
        _pages[i].itemCount = 0;
        
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

template <typename T, int recsPerPage>
MappedFile<T, recsPerPage>::~MappedFile()
{
    close();

    delete [] _pages;
    _pages = 0;

    if(_bDeleteFile)
    	DeleteFile(m_strFileName.c_str());
}

template <typename T, int recsPerPage>
std::wstring
MappedFile<T, recsPerPage>::createTempFile(const std::wstring &strPrefix)
{
	std::wstring strFileName = getTempFileName(strPrefix);
	create(strFileName.c_str(), true);
	return strFileName;
}

template <typename T, int recsPerPage>
std::wstring
MappedFile<T, recsPerPage>::getTempFileName(const std::wstring &strPrefix)
{
	std::wstring strFileName = L"";

	const int _BUFSIZE = 4096;
	TCHAR szTempName[_MAX_PATH];
	TCHAR buffer[_BUFSIZE]; 

	if (GetTempPath(_BUFSIZE, buffer) != 0 &&							// Get a temp path
		GetTempFileName(buffer, strPrefix.c_str(), 0, szTempName) != 0)	// Create a temporary file.
	{
		strFileName = std::wstring(szTempName);
	}

	return (strFileName);
}

template <typename T, int recsPerPage>
HANDLE MappedFile<T, recsPerPage>::create(const wchar_t* name, bool deleteFile /*=true"*/)
{
    _bDeleteFile = deleteFile;
    _canwrite = 1;
    return openInternal( name, false);
}

template <typename T, int recsPerPage>
HANDLE MappedFile<T, recsPerPage>::open(const wchar_t* name, bool readOnly/* = false*/)
{
    _bDeleteFile = false;
    _canwrite = readOnly ? 0 : 1;
    return openInternal( name, true);
}

template <typename T, int recsPerPage>
HANDLE MappedFile<T, recsPerPage>::openInternal(const wchar_t* name, bool open)
{
	// save the name
	m_strFileName = name;

    //acquire file handle
    _handle = CreateFile( 
        name, 
        GENERIC_READ | (_canwrite ? GENERIC_WRITE : 0), 
        FILE_SHARE_READ | (_canwrite ? FILE_SHARE_WRITE : 0), 
        0, 
        open ? OPEN_EXISTING : CREATE_ALWAYS,
        FILE_FLAG_SEQUENTIAL_SCAN | (_bDeleteFile ? FILE_ATTRIBUTE_TEMPORARY : 0),
        0);
    
    if ( _handle == INVALID_HANDLE_VALUE)
    {
        //only flag the error if a Create failed, it is ok to fail
        //if we are only trying to open to see if the file exists.
        if (!open)
            error(L"CreateFile");
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
				error(L"CreateFileMapping");
		}
    }
    else
    {
        _numrecs = 0;
    }

    _numRecsFile = (IndexType)(_numrecs);
    _numRecsOriginal = _numrecs;

    return _fd;
}
    
template <typename T, int recsPerPage>
HANDLE MappedFile<T, recsPerPage>::close()
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

template <typename T, int recsPerPage>
HANDLE MappedFile<T, recsPerPage>::reopen()
{
    close();
    return openInternal(m_strFileName.c_str(), true);
}

template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::closeMapping()
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

            page->startIndex = (IndexType)-1;
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

// determine whether or not the records with the two given indices are on the same page
template <typename T, int recsPerPage>
bool MappedFile<T, recsPerPage>::samePage(IndexType i1, IndexType i2)
{
	// get offset to page containing i1
    IndexType recoff1 = i1 * sizeof(T);
    IndexType pageoff1 = recoff1 & (-recsPerPage);

	// get offset to page containing i2
    IndexType recoff2 = i2 * sizeof(T);
    IndexType pageoff2 = recoff2 & (-recsPerPage);

	return pageoff1 == pageoff2;
}

//given a page offset within the file, checks if the page is
//already mapped in memory -- if yes it returns it, and if not,
//it swaps out a page from the cache and maps in the one we want
//from disk
template <typename T, int recsPerPage>
MemoryPage* MappedFile<T, recsPerPage>::load_page(IndexType index)
{
#ifdef CACHE_STATS
    //guess we are going to have a mild cache miss 
    //meaning record is not on the head page but is mapped
    _cache_misses_L1++; 
	_numQueries++;
	int numPagesSearchedForQuery = 0;
#endif

    IndexType baseIndex = index & (-recsPerPage);

    //check the cache
    MemoryPage* page = _pgHead;
    MemoryPage* tmp = page;
    do 
    {
#ifdef CACHE_STATS
		numPagesSearchedForQuery++;
#endif
		if (page->startIndex == baseIndex) 
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
    swappage(page, baseIndex);

	if (page == _pgHead) {
#ifdef CACHE_STATS
		_numPagesSearched += numPagesSearchedForQuery;
		if (numPagesSearchedForQuery > _maxPagesSearchedForQuery)
			_maxPagesSearchedForQuery = numPagesSearchedForQuery;
#endif
		return page;
	}

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

#ifdef CACHE_STATS
	_numPagesSearched += numPagesSearchedForQuery;
	if (numPagesSearchedForQuery > _maxPagesSearchedForQuery)
		_maxPagesSearchedForQuery = numPagesSearchedForQuery;
#endif
    return page;
}

//called when load() is called with an index that will grow the file (i.e. index >= numrecs).
//This function increments the record count and also if the file has grown by a certain amount,
//will close and reopen the file in order to allow for a bigger portion of the file to be 
//mapped in memory using MapViewOfFile rather than ReadFile
template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::grow(IndexType index, MemoryPage* page)
{
    //increment record count
    _numrecs = index+1;
    
    //also if we have grown a lot beyond _szFileOriginal, we should
    //close and reopen the file, so that we can use Windows memory mapping
    //for the section of the file that was added

    if (!_bPagesLocked && (_numrecs - _numRecsOriginal > _numRecsAllPages))
    {
        //reopen the file -- this will unload all in-memory cache
        reopen();

        //load back the page that we were working with (so that caller doesn't
        //know what evil thing we did here)
        swappage(page, index);
    }
}

//writes out (or unmaps) data that is in the given page and then
//loads data from the given offest into the page
template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::swappage(MemoryPage* page, IndexType index)
{
    //in an attempt to minimize mallocs, we try to reuse space if memory
    //mapping can't be used on both pages of interest.
    //TODO: What if the file is 0 bytes and open for read only? It will crash...
    bool willmap = !_canwrite || (_numRecsOriginal > index && _numRecsOriginal - index >= recsPerPage);

    //unmap or write out the page to file
    if ( page->mapped)
        munmappage(page);
    else
        writepage(page, willmap);

    //use Windows memory mapping if the entire page is within the file
    //or if it is read-only
    if ( willmap)
        mmappage(page, index);
    else
        readpage(page, index);
}

//writes out a page that is managed by us (i.e. is not memory mapped by Windows)
template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::writepage(MemoryPage* page, bool freeMem)
{
    if (!page->itemCount) 
        return;

    DWORD written = 0;

    if (_canwrite)
    {
        //determine how many bytes to physically write -- if it is the
        //last page, it may not have _szPage bytes actually in use
        IndexType itemsToCopy = _numrecs - page->startIndex; 
        itemsToCopy = min (itemsToCopy, page->itemCount);

        LARGE_INTEGER offset;
        offset.QuadPart = page->startIndex * sizeof(T);
       
        SetFilePointer( _handle, offset.LowPart, &offset.HighPart, FILE_BEGIN);
        if ( !WriteFile( _handle, page->p, itemsToCopy * sizeof(T), &written, 0))
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
        page->itemCount = 0;
    }

    //update file size if the page expanded the file
    _numRecsFile = max(_numRecsFile, page->startIndex + (written / sizeof(T)));
}

//reads in a page that we will manage ourselves (i.e. is not memory mapped by Windows)
template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::readpage(MemoryPage* page, IndexType index)
{
    IndexType baseIndex = index & (-recsPerPage); 

    if ( !page->p)
        page->p = (unsigned char*) VirtualAlloc(page->p, recsPerPage * sizeof(T), MEM_COMMIT, PAGE_READWRITE);

    LARGE_INTEGER offset;
    offset.QuadPart = baseIndex * sizeof(T);
    SetFilePointer( _handle, offset.LowPart, &offset.HighPart, FILE_BEGIN);

    DWORD itemsToRead = (DWORD)(_numrecs - baseIndex);
    if ( itemsToRead > recsPerPage)
        itemsToRead = recsPerPage;
    DWORD sizeRead;
    ReadFile( _handle, page->p, itemsToRead * sizeof(T), &sizeRead, 0);

    page->itemCount = recsPerPage;
    page->mapped = false;
    page->startIndex = baseIndex;
    page->refcount = 0;
}

//loads a page using Windows memory mapping -- Windows will manage the memory
template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::mmappage(MemoryPage* page, IndexType index)
{
    IndexType baseIndex = index & (-recsPerPage); 

    IndexType thisPageRecs = min(recsPerPage, _numRecsFile - baseIndex); 

    LARGE_INTEGER offset;
    offset.QuadPart = baseIndex * sizeof(T);

    page->p = (unsigned char *)MapViewOfFileEx( 
        _fd, 
        _canwrite ? FILE_MAP_WRITE : FILE_MAP_READ,  
        offset.HighPart, 
        offset.LowPart, 
        thisPageRecs * sizeof(T), 
        0);

    if ( !page->p)
    {
        error( L"MapViewOfFile");
        return;
    }

    page->mapped = true;
    //page->itemCount = (IndexType)recsPerPage;
    page->itemCount = thisPageRecs;
    page->startIndex = baseIndex;
    page->refcount = 0;
}

template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::munmappage(MemoryPage* page)
{
    if (!page->itemCount) 
        return;

    UnmapViewOfFile( page->p);
    page->itemCount = 0;
    page->p = 0;
    page->startIndex = (IndexType)-1;
}

//decrements the refcount of the cache page that the given pointer belongs to 
//once the refcount is 0, i.e. nobody is using any of this page's data,
//the page can be reused
template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::release(void* ptr)
{
    MemoryPage* page = _pgHead;

    do 
    {
        size_type diff = (size_type)((T*)ptr - (T*)page->p);

        if (diff >= 0 && diff < (size_type)(page->itemCount))
        {
            page->refcount--;
            return;
        }
    } 
    while ((page = page->pnext) != _pgHead);

    throw L"Page was released before we were done using it!!!";
}

template <typename T, int recsPerPage>
IndexType MappedFile<T, recsPerPage>::numrecsInternal()
{
    LARGE_INTEGER  offset;
    GetFileSizeEx( _handle, &offset);
    return (IndexType)(offset.QuadPart / sizeof(T));
}

//writes out all memory pages to disk
template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::flushall()
{
    //flush all mapped pages
    MemoryPage* page = _pgHead;
    do 
    {
        if ( page->itemCount) 
        {
            if (page->mapped)
                FlushViewOfFile( page->p, page->itemCount * sizeof(T)); 
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
template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::syncsize()
{
    if ( _numrecs - _numRecsOriginal >= recsPerPage)
    {
        //reopen the file -- this will unload all in-memory cache
        reopen();
    }
}

template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::error( const wchar_t* operation)
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
        (wchar_t*) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((FdoString*)lpMsgBuf)+lstrlen((FdoString*)operation)+40)*sizeof(FdoString)); 
    wsprintf((wchar_t*)lpDisplayBuf, L"%s failed with error %d: %s", operation, error, reinterpret_cast<FdoString*>(lpMsgBuf)); 
    MessageBox(NULL, (FdoString*)lpDisplayBuf, L"Error", MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::empty()
{
	//
	// clear out the file contents -- set the file length to zero
	//

	closeMapping();

	SetFilePointer(_handle, 0, NULL, FILE_BEGIN);
	SetEndOfFile(_handle);

	_numRecsFile = _numRecsOriginal = _numrecs = 0;
}

template <typename T, int recsPerPage>
void MappedFile<T, recsPerPage>::copyTo(const wchar_t* dest)
{
	flushall();

	int stat = CopyFile(m_strFileName.c_str(), dest, FALSE);
	AECC_ASSERT(stat != 0);
	if (stat == 0)
	{
		error(L"copyTo");
	}
}

template <typename T, int recsPerPage>
bool MappedFile<T, recsPerPage>::enumPages(EnumPagesCallback pCallback, void* pUserData)
{
	IndexType recs_per_page = recsPerPage;
    IndexType index = 0;

    while (index < _numrecs)
    {
		void* pBase = load(index);
		_bPagesLocked = true;	// disallow pages being swapped out to disk if the file grows while it's being traversed;
								// this sometimes happens when we add smoothing points (see defect 1025001)

	    bool ret = pCallback(pBase, index, min(_numrecs - index, recs_per_page), pUserData);

		release(pBase);
		_bPagesLocked = false;
    
		if (!ret) return false;
		
		index += recs_per_page;
	}

	syncsize(); // in case the file grew during the above traversal

    return true;
}

#endif
