/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"
#include <Sm/Base.h>

#ifdef SCHEMA_DEBUG
// The following array tracks the objects of this class that have been
// created, and whether they have been deleted. 
static long aCount = 0;
static wchar_t* aNames[100000];
static void* aAddrs[100000];
static bool  aAlc[100000];
#endif

FdoSmBase::FdoSmBase(const wchar_t* parent, const wchar_t* name)
{
#ifdef SCHEMA_DEBUG
	// Used when purify license is busy.
	// Records each created object in an array.
	wchar_t* localName = new wchar_t[wcslen(parent) + wcslen(name) + 10];
	wcscpy( localName, parent );
	wcscat( localName, L" | " );
	wcscat( localName, name );

	// for checking class of a particular object. 
	// provides place to put breakpoint.
	if ( aCount == 8987 ) {
		int j = 8;
	}

	aNames[aCount] = localName;
	aAddrs[aCount] = (void*) this;
	aAlc[aCount] = true;
	aCount++;
#endif
}

FdoSmBase::~FdoSmBase(void)
{
#ifdef SCHEMA_DEBUG
	// Used when purify license is busy.
	// Find this object in the array and flag it as deleted
	for ( int i = 0; i < aCount; i++ ) {
		if ( aAlc[i] && (aAddrs[i] == (void*) this) ) {
			aAlc[i] = false;
			break;
		}
	}
#endif
}

/*
void * FdoSmBase::operator new (size_t size)
{
	void* pPtr = ::operator new(size);
	if ( !pPtr )
		throw "exception Todo";
	return(pPtr);
}

void * FdoSmBase::operator new (size_t size, const char* filler, int line)
{
	void* pPtr = ::operator new(size);
	//Todo: handle filler.
	if ( !pPtr )
		throw "exception Todo";
	return(pPtr);
}

void   FdoSmBase::operator delete (void*)
{
	operator delete();
}
*/
#ifdef SCHEMA_DEBUG
void FdoSmBase::Report()
{
	FILE* fp = fopen("memrpt.txt", "w" );
	int leakCount = 0;

	fprintf( fp, "Leaked Name Position\n\n" );

	for ( int i = 0; i < aCount; i++ ) {
        fprintf( fp, " {%d} %ls %d\n", aAlc[i], aNames[i], i );
		if ( aAlc[i] ) 
			leakCount++;

		delete aNames[i];
	}

	fprintf( fp, "Total Count: %d Leak Count: %d\n", aCount, leakCount );

	fclose(fp);

    aCount = 0;
}
#endif
