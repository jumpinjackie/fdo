#ifndef FDOSMPHINDEXLOADER_H
#define FDOSMPHINDEXLOADER_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/DbObjectComponentLoader.h>
#include <Sm/Ph/Rd/IndexReader.h>
#include <Sm/Ph/Owner.h>

// This class provides lazy bulk loader for indexes.
// DbObject components include columns, keys, indexes, constraints and base object relationships.
// Most of these components are bulk fetched when the DbObjects are fetched. 
// However, indexes are fetched only when the indexes for a DbObject are asked for. It is anticipated
// that if the indexes for one DbObject are needed, the indexes for other DbObjects will also be 
// requested. Therefore, the requested indexes plus indexes for some of the other cached DbObjects are 
// loaded in the same select.
//
class FdoSmPhIndexLoader : public FdoSmPhDbObjectComponentLoader<FdoSmPhRdIndexReader>
{
public:

    /// Constructs an instance of an Index Loader
    FdoSmPhIndexLoader(
        FdoSmPhOwnerP owner,             // load indexes for DbObjects in this owner.
        FdoSmPhDbObjectsP dbObjects      // candidate dbObject list. Indexes will be loaded for requested
                                         // DbObject plus some of these candidates.
    ); 
	~FdoSmPhIndexLoader(void);

    virtual void LoadCandidates( 
        FdoSmPhDbObjectsP dbObjects,
        FdoStringP objectName,
        int& nextCandIdx 
    );

    virtual bool ComponentsLoaded( FdoSmPhDbObjectP dbObject );
    
    virtual bool IsCandidate( FdoSmPhDbObjectP dbObject );

    virtual FdoPtr<FdoSmPhRdIndexReader> CreateReader( FdoStringsP candidates );

    virtual FdoPtr<FdoSmPhRdIndexReader> CreateReader();

    virtual FdoStringP GetObjectName( FdoSmPhRdIndexReaderP rdr );

    virtual bool CacheObjectComponents( FdoSmPhDbObjectP dbObject, FdoSmPhRdIndexReaderP rdr );

private:
    int mNextIndexRootTableCandIdx;
};

typedef FdoPtr<FdoSmPhIndexLoader> FdoSmPhIndexLoaderP;

#endif



