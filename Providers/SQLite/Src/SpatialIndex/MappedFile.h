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

struct MemoryPage
{
    MemoryPage()
        : offset((IndexType)-1),
          size(0),
          p(0),
          refcount(0)
    {
    }

    IndexType offset;
    IndexType size;
    unsigned char* p;
    int refcount;
    MemoryPage* pnext;
    MemoryPage* pprev;
	bool mapped;
};

class MappedFile
{
public:

    MappedFile(size_t szData, size_t szPage, int numPages);
    ~MappedFile();

    HANDLE create(const wchar_t* name, bool deleteOnExit = false);
    HANDLE open(const wchar_t* name, bool readOnly = false);
    HANDLE close();


    //variaous ways to get a pointer to a record, depending on what
    //we need it for
    void* load(IndexType index);
    void* load_noaddref(IndexType index);
    void* load_newrec(IndexType index);
    
    void release(void* ptr);
    void flushall();
    void syncsize();
	void empty();

    IndexType numrecs() { return _numrecs; }

    HANDLE fd() { return _fd; }

    const std::wstring& name() { return m_strFileName; }

private:

    MemoryPage* __fastcall load_page(IndexType index);

    void swappage(MemoryPage* page, IndexType pageoff);

    void writepage(MemoryPage* page, bool freeMem = false);
    void readpage(MemoryPage* page, IndexType pageoff);

    void mmappage(MemoryPage* page, IndexType pageoff);
    void munmappage(MemoryPage* page);

    void grow(IndexType index, MemoryPage* page, IndexType pageoff);


    HANDLE openInternal(const wchar_t* name, bool open);
    IndexType numrecsInternal();
	void closeMapping();
	void error( const wchar_t* operation);


    HANDLE _fd;
	HANDLE _handle;
    int _canwrite;

    //these are constant once the MappedFile is initialized
    int _numPages;
    size_t _szPage;
    size_t _szData;
    int _iszDataShift;
    size_type _pageMask;
    size_t _szAllPages;

    //these vary as the file grows
	IndexType _numrecs;
    IndexType _szFileOriginal;
    IndexType _szFile;

    //linked list of pages that are currently mapped
    //in RAM
    MemoryPage* _pages;
    MemoryPage* _pgHead;

	std::wstring m_strFileName;
	bool _bPagesLocked;
    bool _bDeleteOnExit;

public:
    size_t _cache_callcount;
    size_t _cache_misses_L1;
    size_t _cache_misses_L2;
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
inline void* MappedFile::load(IndexType index)
{
    //offset to beginning of record.
    IndexType recoff = index << _iszDataShift;

    //offset to the page that contains it
    IndexType pageoff = recoff & _pageMask;

#ifdef CACHE_STATS
    _cache_callcount++;
#endif

    //get the page that has the record
    MemoryPage* page = (_pgHead->offset == pageoff) ? _pgHead : load_page(pageoff);

    //refcount the page and return the correct pointer into page data
    page->refcount ++;
    return &page->p[recoff - pageoff];
}

//same as load() but does not addref. Useful when we only need
//the pointer for a short time and are done with it before we request
//another pointer
inline void* MappedFile::load_noaddref(IndexType index)
{
    //offset to beginning of record.
    IndexType recoff = index << _iszDataShift;

    //offset to the page that contains it
    IndexType pageoff = recoff & _pageMask;

#ifdef CACHE_STATS
    _cache_callcount++;
#endif

    //get the page that has the record
    MemoryPage* page = (_pgHead->offset == pageoff) ? _pgHead : load_page(pageoff);

    //return the pointer to the record -- we are not addreffing here
    return &page->p[recoff - pageoff];
}

//same as load() but used when adding new records to the file -- it will
//perform file resizing logic (increment record count, flush the file if needed).
//This function adds ref.
inline void* MappedFile::load_newrec(IndexType index)
{
    //offset to beginning of record.
    IndexType recoff = index << _iszDataShift;

    //offset to the page that contains it
    IndexType pageoff = recoff & _pageMask;

#ifdef CACHE_STATS
    _cache_callcount++;
#endif

    //get the page that has the record
    MemoryPage* page = (_pgHead->offset == pageoff) ? _pgHead : load_page(pageoff);

    //grow the file, refcount the page, return the pointer
    if ( index >= _numrecs)
        grow(index, page, pageoff);    
    page->refcount ++;
    return &page->p[recoff - pageoff];
}


#endif
