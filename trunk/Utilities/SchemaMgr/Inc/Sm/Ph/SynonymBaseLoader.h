#ifndef FDOSMPHSYNONYMBASELOADER_H
#define FDOSMPHSYNONYMBASELOADER_H		1
/*
 * Copyright (C) 2011  Autodesk, Inc.
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/DbObjectComponentLoader.h>
#include <Sm/Ph/Rd/SynonymReader.h>
#include <Sm/Ph/Owner.h>

// This class provides lazy bulk loader for synonym references. The actual synonyms 
// are cached along with other types of db objects. This loader loads the synonym to 
// referenced object correspondences.
class FdoSmPhSynonymBaseLoader : public FdoSmPhDbObjectComponentLoader<FdoSmPhRdSynonymReader>
{
public:

    /// Synonym Base Loader Constructor
    FdoSmPhSynonymBaseLoader(
        FdoSmPhOwnerP owner,             // load synonym references for this owner.
        FdoSmPhDbObjectsP dbObjects      // candidate dbObject list. References will be loaded for requested
                                         // synonym plus some synonyms from this list.
    ); 
	~FdoSmPhSynonymBaseLoader(void);

    // Various overrides for FdoSmPhDbObjectComponentLoader functions.

    virtual bool ComponentsLoaded( FdoSmPhDbObjectP dbObject );
    
    virtual bool IsCandidate( FdoSmPhDbObjectP dbObject );

    virtual FdoPtr<FdoSmPhRdSynonymReader> CreateReader( FdoStringsP candidates );

    virtual FdoPtr<FdoSmPhRdSynonymReader> CreateReader();

    virtual FdoStringP GetObjectName( FdoSmPhRdSynonymReaderP rdr );

    virtual bool CacheObjectComponents( FdoSmPhDbObjectP dbObject, FdoSmPhRdSynonymReaderP rdr );

private:
    FdoSmPhOwnerP mOwner;
};

typedef FdoPtr<FdoSmPhSynonymBaseLoader> FdoSmPhSynonymBaseLoaderP;

#endif



