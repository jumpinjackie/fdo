/*
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 * 
 */

#include "stdafx.h"
#include <Sm/Ph/IndexLoader.h>
#include <Sm/Ph/Owner.h>

FdoSmPhIndexLoader::FdoSmPhIndexLoader(
    FdoSmPhOwnerP owner,
    FdoSmPhDbObjectsP dbObjects
) :
    FdoSmPhDbObjectComponentLoader<FdoSmPhRdIndexReader>( owner, dbObjects ),
    mNextIndexRootTableCandIdx(0)
{
}

FdoSmPhIndexLoader::~FdoSmPhIndexLoader()
{
}

void FdoSmPhIndexLoader::LoadCandidates( 
    FdoSmPhDbObjectsP dbObjects,
    FdoStringP objectName,
    int& nextCandIdx 
)
{
    FdoSmPhDbObjectComponentLoader<FdoSmPhRdIndexReader>::LoadCandidates( dbObjects, objectName, nextCandIdx );

    FdoDictionaryP candidates = GetCandidates();

    FdoInt32 ix = candidates->IndexOf( objectName );

    if ( ix < 0 ) {
        // Requested object still not on candidates list. Object might be a table with pkey, that
        // has dependent view that does not include all of the table's pkey columns
        // (in this case, we try to generate view identity from a unique index on the
        // table. Load all tables with dependent views (where the view has no direct pkey).

        FdoInt32 idx;

        if ( dbObjects ) {
            FdoInt32 start = mNextIndexRootTableCandIdx;

            // Update starting index now in case this function
            // gets called recursively. Prevents infinite recursion.
            mNextIndexRootTableCandIdx = dbObjects->GetCount();

            // Check each dbObject not yet checked
            for ( idx = start; idx < dbObjects->GetCount(); idx++ ) {
                FdoSmPhDbObjectP dbObject = dbObjects->GetItem( idx );

                // Skip objects whose columns have not been loaded. Loading
                // these causes column loads for each individual object.
                // (which is slow, and nullifies the advantage of bulk loading their 
                // indexes).
                if ( dbObject->ColumnsLoaded() ) {
                    if ( (dbObject->GetPkeyColumns()->GetCount() == 0) && dbObject->ClassifyObjectType(false) ) {
                        FdoSmPhDbObjectP rootObject = dbObject->GetLowestRootObject();
                    
                        // Loading indexes into this owner so skip any base objects in 
                        // other owners.
                        if ( rootObject && (GetOwner()->GetQName() == rootObject->GetParent()->GetQName()) ) {
                            if ( dbObject->GetQName() != rootObject->GetQName() ) {
                                // Object has no primary key but has a root object. We might 
                                // use root object's indexes to generate object's identity.
                                FdoSmPhTableP table = rootObject->SmartCast<FdoSmPhTable>();

                                if ( table && !table->IndexesLoaded() )
                                    AddCandidate( table->GetName() );
                            }
                        }
                    }
                }
            }
        }
    }
}



bool FdoSmPhIndexLoader::ComponentsLoaded( FdoSmPhDbObjectP dbObject )
{
    return dbObject->IndexesLoaded();
}
    
bool FdoSmPhIndexLoader::IsCandidate( FdoSmPhDbObjectP dbObject )
{
    bool isIndexCand = false;

    // Skip objects whose columns have not been loaded. Loading
    // these causes column loads for each individual object.
    // (which is slow, and nullifies the advantage of bulk loading their 
    // indexes).
    if ( dbObject->ColumnsLoaded() ) {
        if ( dbObject->GetPkeyColumns()->GetCount() == 0 ) {
            // Indexes are used to generate identity when no primary key, so this
            // DbObject is a candidate for bulk loading indexes.
            isIndexCand = true;
        }
        else {
            FdoSmPhColumnsP columns = dbObject->GetColumns();
            int i;
            int geomCount = 0;

            for ( i = 0; i < columns->GetCount(); i++ ) {
                FdoSmPhColumnP column = columns->GetItem(i);
                if ( column->GetType() == FdoSmPhColType_Geom ) 
                    geomCount++;

                if ( geomCount > 1 ) {
                    // Spatial indexes are used to pick main geometry when 
                    // DbOBject  has multiple geometric columns.
                    isIndexCand = true;
                    break;
                }
            }
        }
    }

    return isIndexCand;
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhIndexLoader::CreateReader( FdoStringsP candidates )
{
    return GetOwner()->CreateIndexReader( candidates );
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhIndexLoader::CreateReader()
{
    return GetOwner()->CreateIndexReader();
}

FdoStringP FdoSmPhIndexLoader::GetObjectName( FdoSmPhRdIndexReaderP rdr )
{
    return rdr->GetString( L"", L"table_name" );
}

bool FdoSmPhIndexLoader::CacheObjectComponents( FdoSmPhDbObjectP dbObject, FdoSmPhRdIndexReaderP rdr )
{
    return dbObject->CacheIndexes( rdr );
}

