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
#include <math.h>
#include <Sm/Ph/Owner.h>
#include <Sm/Error.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Ph/Rd/ColumnReader.h>
#include <Sm/Ph/Rd/BaseObjectReader.h>
#include <Sm/Ph/Rd/DbObjectReader.h>
#include <Sm/Ph/Rd/ViewReader.h>
#include <Sm/Ph/Rd/CoordSysReader.h>
#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Ph/Rd/FkeyReader.h>
#include <Sm/Ph/Rd/IndexReader.h>
#include <Sm/Ph/Rd/PkeyReader.h>
#include <Sm/Ph/Rd/SpatialContextReader.h>
#include <Sm/Ph/Rd/TableJoin.h>
#include <Sm/Ph/OptionsReader.h>
#include <Sm/Ph/SchemaReader.h>


FdoSmPhOwner::FdoSmPhOwner(
    FdoStringP name, 
    bool hasMetaSchema,
    const FdoSmPhDatabase* pDatabase,
	FdoSchemaElementState elementState
) : 
    FdoSmPhDbElement(name, (FdoSmPhMgr*) NULL, pDatabase, elementState ),
    mDbObjectsCached(false), mDbComponentsCached(false)
{
    SetHasMetaSchema( hasMetaSchema );
    mLtMode = NoLtLock;
    mLckMode = NoLtLock;

    // Load LT and Lock settings from datastore only for existing owners.
    mLtLckLoaded = (elementState == FdoSchemaElementState_Added);

    mSchemaVersion = 0.0;
    // Schema Info is handled a bit differently. We want to trip a load
    // after a new owner is committed, so that the right schema version
    // is retrieved.
    mSchemaInfoLoaded = false;
	SetIsSystem(false);

    mNotFoundObjects = FdoDictionary::Create();

    mReservedDbObjectNames = FdoStringCollection::Create();

    // Add commonly accessed MetaSchema tables to fetch candidates list. This
    // causes them to be fetched in one reader.
    mCandDbObjects = FdoDictionary::Create();
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_associationdefinition") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_attributedefinition") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_attributedependencies") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_classdefinition") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_dbopen") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_sad") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_lockname") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_options") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_schemainfo") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_schemaoptions") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_spatialcontext") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_spatialcontextgeom") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_spatialcontextgroup") );

    mCandIndexes = FdoDictionary::Create();   
    mNextIndexTableCandIdx = 0;
    mNextIndexRootTableCandIdx = 0;

    mNextBaseCandIdx = 0;

    mSpatialContextsLoaded = false;

    mCoordinateSystemsLoaded = false;

    mNextBaseCandIdx = 0;

    mBulkLoadPkeys = false;
}

FdoSmPhOwner::~FdoSmPhOwner(void)
{
}

void FdoSmPhOwner::SetPassword( FdoStringP password )
{
    mPassword = password;
}

FdoStringP FdoSmPhOwner::GetPassword()
{
    return mPassword;
}

void FdoSmPhOwner::SetIsSystem( bool isSystem )
{
	mIsSystem = isSystem;
}

bool FdoSmPhOwner::GetIsSystem()
{
	return mIsSystem;
}

void FdoSmPhOwner::SetHasMetaSchema( bool hasMetaSchema )
{
    mHasMetaSchema = hasMetaSchema;
}

bool FdoSmPhOwner::GetHasMetaSchema()
{
    return mHasMetaSchema;
}

double FdoSmPhOwner::GetSchemaVersion()
{
    LoadSchemaInfo();

    return mSchemaVersion;
}

FdoString* FdoSmPhOwner::GetDescription() const
{
    ((FdoSmPhOwner*) this)->LoadSchemaInfo();

    return FdoSmPhDbElement::GetDescription();
}

void FdoSmPhOwner::SetLtMode( FdoLtLockModeType LtMode )
{
    LoadLtLck();

	mLtMode = LtMode;
}

FdoLtLockModeType FdoSmPhOwner::GetLtMode() const
{
    ((FdoSmPhOwner*)this)->LoadLtLck();

	return mLtMode;
}

void FdoSmPhOwner::SetLckMode( FdoLtLockModeType LckMode )
{
    LoadLtLck();

	mLckMode = LckMode;
}

FdoLtLockModeType FdoSmPhOwner::GetLckMode() const
{
    ((FdoSmPhOwner*)this)->LoadLtLck();

	return mLckMode;
}

const FdoLockType* FdoSmPhOwner::GetLockTypes( FdoLtLockModeType lckMode, FdoInt32& size)
{
    // Inltialze lock lists
    LoadLtLck();

    // Get entry for the given lock mode.
    const FdoSmPhLockTypes* lockTypes = GetLockTypesCollection()->RefItem( lckMode );
    if ( !lockTypes ) 
        // Default to the NoLock mode if mode not supported.
        lockTypes = mLockTypes->RefItem( NoLtLock );

    if ( !lockTypes ) {
        // No lock types even for default mode so return nothing
        size = 0;
        return (FdoLockType*) NULL;
    }

    return lockTypes->GetLockTypes( size );
}

const FdoSmPhDbObject* FdoSmPhOwner::RefDbObject(FdoStringP dbObject) const
{
    return (FdoSmPhDbObject*) ((FdoSmPhOwner*) this)->FindDbObject(dbObject);
}

FdoSmPhDbObjectP FdoSmPhOwner::FindDbObject(FdoStringP dbObject)
{
    // If Db Object name is too short or too long then we know right away that it
    // does not exist.
    if ( (dbObject == L"") || (dbObject.GetLength() > GetManager()->DbObjectNameMaxLen()) )
        return (FdoSmPhDbObject*) NULL;

    // Check cache for database object
    FdoSmPhDbObjectP pDbObject = GetDbObjects()->FindItem(dbObject);

    if ( (!pDbObject) && (dbObject != L"")) {
        // Not in cache. If it is in the fetch candidate list then fetch
        // it along with some other candidates. Some other candidates
        // are selected to help performance since these will likely be
        // asked for later.
        pDbObject = CacheCandDbObjects( dbObject );
    }

    if ( !pDbObject ) {
        // Not a candidate either. Check if previously fetched but not found.
        if ( mNotFoundObjects->IndexOf( dbObject ) >= 0 ) 
            return pDbObject;

        // Not in cache so read it in.
        FdoSmPhRdDbObjectReaderP reader = CreateDbObjectReader(dbObject);
   
        if ( reader->ReadNext() )
            pDbObject = CacheDbObject( reader );

        if ( (!pDbObject) && (dbObject != L"")) {
            // Not in RDBMS so add to not found list (avoids multiple RDBMS fetches when this
            // object is asked for repeatedly).
            FdoDictionaryElementP elem = FdoDictionaryElement::Create( dbObject, L"" );
            mNotFoundObjects->Add( elem );
        }
    }
   
    return(pDbObject);
}

FdoSmPhDbObjectP FdoSmPhOwner::GetDbObject(FdoStringP dbObject)
{
    FdoSmPhDbObjectP pDbObject = FindDbObject(dbObject);

    if ( !pDbObject ) 
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_4),
                (FdoString*) GetQName(),
				(FdoString*) dbObject
            )
        );

    return(pDbObject);
}

FdoSmPhDbObjectP FdoSmPhOwner::FindReferencedDbObject(FdoStringP dbObject, FdoStringP owner, FdoStringP database )
{   
    FdoSmPhDbObjectP pDbObject;

    // Look for referenced object in the cache for its owner.
    FdoSmPhOwnerP refOwner = GetManager()->FindOwner( owner, database );

    if ( refOwner ) {
        pDbObject = refOwner->GetDbObjects()->FindItem( dbObject );

        if ( !pDbObject ) {
            // Not in cache. Set up base objects for bulk loading
            LoadBaseObjectCands();

            // Find the object. This causes the bulk loading of it and some other
            // objects into the schema cache.
            pDbObject = refOwner->FindDbObject( dbObject );
        }
    }

    return pDbObject;
}

FdoSmPhDbObjectP FdoSmPhOwner::GetCachedDbObject(FdoInt32 idx)
{
    FdoSmPhDbObjectP dbObject;
    FdoSmPhDbObjectsP dbObjects = GetDbObjects();

    if ( (idx >= 0) && (idx < dbObjects->GetCount()) ) 
        dbObject = dbObjects->GetItem( idx );

    return dbObject;
}

FdoSmPhSpatialContextsP FdoSmPhOwner::GetSpatialContexts()
{
    LoadSpatialContexts();

    return mSpatialContexts;
}

FdoSmPhSpatialContextP FdoSmPhOwner::FindSpatialContext( FdoInt64 scId )
{
    FdoSmPhSpatialContextP sc;

    if ( mSpatialContexts ) 
        sc = mSpatialContexts->FindItemById( scId );

    if ( !sc ) {
        LoadSpatialContexts();
        sc = mSpatialContexts->FindItemById( scId );
    }

    return sc;
}

FdoSmPhSpatialContextGeomsP FdoSmPhOwner::GetSpatialContextGeoms()
{
    LoadSpatialContexts();

    return mSpatialContextGeoms;
}

FdoSmPhSpatialContextGeomP FdoSmPhOwner::FindSpatialContextGeom( FdoStringP dbObjectName, FdoStringP columnName )
{
    FdoStringP scGeomName = FdoSmPhSpatialContextGeom::MakeName(dbObjectName, columnName);
    FdoSmPhSpatialContextGeomP scGeom;

    // Check if required association already cached.
    if ( mSpatialContextGeoms )
        scGeom = mSpatialContextGeoms->FindItem(scGeomName);

    if ( !scGeom ) {
        // Not in cache, so load the associations for the given db object and
        // try again.
        LoadSpatialContexts( dbObjectName );
        scGeom = mSpatialContextGeoms->FindItem(scGeomName);
    }

    return scGeom;
}

FdoSmPhCoordinateSystemP FdoSmPhOwner::FindCoordinateSystem( FdoInt64 srid )
{
    FdoSmPhCoordinateSystemP coordSys;

    // Check the cache first
    if ( mCoordinateSystems ) 
        coordSys = mCoordinateSystems->FindItemById( srid );

    if ( !coordSys && !mCoordinateSystemsLoaded ) {
        // Not in the cache, load from the RDBMS and try again.
        LoadCoordinateSystems( CreateCoordSysReader(srid) );
        coordSys = mCoordinateSystems->FindItemById( srid );
    }

    return coordSys;
}

FdoSmPhCoordinateSystemP FdoSmPhOwner::FindCoordinateSystem( FdoStringP csName )
{
    FdoSmPhCoordinateSystemP coordSys;

    // Check the cache first
    if ( mCoordinateSystems ) 
        coordSys = mCoordinateSystems->FindItem( csName );

    if ( !coordSys && !mCoordinateSystemsLoaded ) {
        // Not in the cache, load from the RDBMS and try again.
        LoadCoordinateSystems( CreateCoordSysReader(csName) );
        coordSys = mCoordinateSystems->FindItem( csName );
    }

    return coordSys;
}

FdoSmPhCoordinateSystemP FdoSmPhOwner::FindCoordinateSystemByWkt( FdoStringP wkt )
{
    FdoSmPhCoordinateSystemP coordSys;

    // Check the cache first
    if ( mCoordinateSystems ) 
        coordSys = mCoordinateSystems->FindItemByWkt( wkt );

    if ( !coordSys && !mCoordinateSystemsLoaded ) {
        // Not in the cache, load from the RDBMS and try again.
        mCoordinateSystemsLoaded = true;
        LoadCoordinateSystems( CreateCoordSysReader() );
        coordSys = mCoordinateSystems->FindItemByWkt( wkt );
    }

    return coordSys;
}

FdoStringP FdoSmPhOwner::GetBestSchemaName() const
{
    return FdoSmPhMgr::RdSchemaPrefix + GetName();
}

FdoStringP FdoSmPhOwner::UniqueDbObjectName( FdoStringP objectName )
{
	// Censor out characters not allowed.
    // Also convert name to default case. 
    // LogicalPhysical Schema Manager does not support creating 
    // objects with names in other cases.
	FdoStringP workName = GetManager()->GetDcDbObjectName( GetManager()->CensorDbObjectName(objectName) );
	FdoStringP outName = workName;
    FdoSize tableMaxSize = GetManager()->TableNameMaxLen();      

	// Adjust the name if it is too long, is a reserved name or the name of an existing
	// table.
	//
	// Do length check against char* version since this is what goes into the database.

	if ( ((int)strlen(outName) > tableMaxSize) ||
		 GetManager()->IsDbObjectNameReserved(outName) ||
		 IsDbObjectNameReserved( outName )
	) {

		for ( int i = 1; ; i++ ) {
			// Truncate and tack on a unique number
			outName = FdoStringP::Format( L"%ls%d",
							(FdoString*) GetManager()->GetDcDbObjectName( 
                                workName.Mid( 
                                    0, 
                                    tableMaxSize - ((int) log10((double)i)) - 1, 
                                    !(GetManager()->IsRdbUnicode()) 
                                ) 
                            ),
							i
					);

			if ( (!IsDbObjectNameReserved(outName)) && 
                 (!GetManager()->IsDbObjectNameReserved(outName)) )
				// Name not in use, we're done
				break;

			// Otherwise, increment unique number and try again.
		}
	}

	ReserveDbObjectName(outName, false);

	return(outName);
}

FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhOwner::CreateDbObjectReader( FdoStringsP objectNames ) const
{
    return (FdoSmPhRdDbObjectReader*) NULL;
}
 
FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhOwner::CreateDbObjectReader( FdoSmPhRdTableJoinP join ) const
{
    return (FdoSmPhRdDbObjectReader*) NULL;
}

FdoPtr<FdoSmPhRdViewReader> FdoSmPhOwner::CreateViewReader() const
{
    return (FdoSmPhRdViewReader*) NULL;
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhOwner::CreateConstraintReader( FdoStringsP ownerNames, FdoStringP constraintType ) const
{
    return (FdoSmPhRdConstraintReader*) NULL;
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhOwner::CreateConstraintReader( FdoSmPhRdTableJoinP join, FdoStringP constraintType ) const
{
    return (FdoSmPhRdConstraintReader*) NULL;
}

FdoPtr<FdoSmPhRdCoordSysReader> FdoSmPhOwner::CreateCoordSysReader( FdoStringP csysName) const
{
    return new FdoSmPhRdCoordSysReader();
}

FdoPtr<FdoSmPhRdCoordSysReader> FdoSmPhOwner::CreateCoordSysReader( FdoInt64 srid ) const
{
    return new FdoSmPhRdCoordSysReader();
}

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhOwner::CreateFkeyReader() const
{
    return (FdoSmPhRdFkeyReader*) NULL;
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhOwner::CreateIndexReader() const
{
    return (FdoSmPhRdIndexReader*) NULL;
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhOwner::CreateIndexReader( FdoStringsP objectNames ) const
{
    return (FdoSmPhRdIndexReader*) NULL;
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhOwner::CreatePkeyReader() const
{
    return (FdoSmPhRdPkeyReader*) NULL;
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhOwner::CreatePkeyReader( FdoPtr<FdoSmPhRdTableJoin> join ) const
{
    return (FdoSmPhRdPkeyReader*) NULL;
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhOwner::CreateColumnReader() const
{
    return (FdoSmPhRdColumnReader*) NULL;
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhOwner::CreateColumnReader( FdoStringsP objectNames ) const
{
    return (FdoSmPhRdColumnReader*) NULL;
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhOwner::CreateColumnReader( FdoSmPhRdTableJoinP join ) const
{
    return (FdoSmPhRdColumnReader*) NULL;
}

FdoPtr<FdoSmPhRdBaseObjectReader> FdoSmPhOwner::CreateBaseObjectReader() const
{
    return (FdoSmPhRdBaseObjectReader*) NULL;
}

FdoPtr<FdoSmPhRdBaseObjectReader> FdoSmPhOwner::CreateBaseObjectReader( FdoStringsP objectNames ) const
{
    return (FdoSmPhRdBaseObjectReader*) NULL;
}

FdoPtr<FdoSmPhRdSpatialContextReader> FdoSmPhOwner::CreateRdSpatialContextReader()
{
    return new FdoSmPhRdSpatialContextReader(FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhRdSpatialContextReader> FdoSmPhOwner::CreateRdSpatialContextReader( FdoStringP dbObjectName )
{
    return new FdoSmPhRdSpatialContextReader(FDO_SAFE_ADDREF(this) );
}

FdoSmPhTableP FdoSmPhOwner::CreateTable(
    FdoStringP tableName,
    FdoStringP pkeyName
)
{
    if ( FindDbObject( tableName ) )
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_210),
				(FdoString*) GetQName(), 
                (FdoString*) tableName
			)
		);

    // TODO: set pkey name.
    FdoSmPhDbObjectP table = NewTable( tableName, FdoSchemaElementState_Added, NULL );
    GetDbObjects()->Add(table);

    return( table->SmartCast<FdoSmPhTable>() );
}

FdoSmPhViewP FdoSmPhOwner::CreateView(
    FdoStringP viewName,
    FdoStringP rootDatabase,
    FdoStringP rootOwner,
    FdoStringP rootObjectName
)
{
    if ( FindDbObject( viewName ) )
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_210),
				(FdoString*) GetQName(), 
                (FdoString*) viewName
			)
		);

    FdoSmPhDbObjectP view = NewView( 
        viewName, 
        rootDatabase,
        rootOwner,
        rootObjectName,
        FdoSchemaElementState_Added,
        NULL
    );
    
    GetDbObjects()->Add(view);

    return( view->SmartCast<FdoSmPhView>() );
}

FdoSmPhDbObjectsP FdoSmPhOwner::CacheDbObjects( bool cacheComponents )
{
    // skip if all objects already cached.
    if ( !mDbObjectsCached ) {
        mDbObjectsCached = true;
        mDbComponentsCached = cacheComponents;

        ReadAndCacheDbObjects(cacheComponents);

    }
    else if (cacheComponents && !mDbComponentsCached) {
        mDbComponentsCached = true;

        ReadAndCacheDbObjects(cacheComponents);
    }


    return GetDbObjects();
}

void FdoSmPhOwner::ReadAndCacheDbObjects(bool cacheComponents)
{
    // No need for fetch candidates since this function caches all object for this owner.
    mCandDbObjects->Clear();

    FdoSmPhRdDbObjectReaderP objReader;
    FdoSmPhRdViewReaderP viewReader;
    FdoSmPhRdColumnReaderP columnReader;
    FdoSmPhRdBaseObjectReaderP baseObjectReader;
    FdoSmPhRdConstraintReaderP ukeyReader;
    FdoSmPhRdConstraintReaderP ckeyReader;
    FdoSmPhRdFkeyReaderP fkeyReader;
    FdoSmPhRdPkeyReaderP pkeyReader;

   // Create reader for owner's db objects
    objReader = CreateDbObjectReader();

    if ( cacheComponents ) {
        // Caching db object components so create readers for components.
        // This function does interleaved fetches from each reader so all readers
        // (including dbObject reader) must return rows ordered by dbObject name.
        //
        // Doing a single query per owner for each component is more efficient than
        // a query per dbObject.
        viewReader = CreateViewReader();
        columnReader = CreateColumnReader();
        baseObjectReader = CreateBaseObjectReader();
        ukeyReader = CreateConstraintReader( L"", L"U" );
        ckeyReader = CreateConstraintReader( L"", L"C" );
        fkeyReader = CreateFkeyReader();
        pkeyReader = CreatePkeyReader();
    }

    while ( objReader->ReadNext() ) {
        // Cache the current dbObject
        FdoSmPhDbObjectP dbObject = CacheDbObject( objReader );

        if ( dbObject && cacheComponents ) {

            if ( columnReader ) 
                dbObject->CacheColumns( columnReader );

            if ( baseObjectReader ) 
                dbObject->CacheBaseObjects( baseObjectReader );

            if ( fkeyReader ) 
                dbObject->CacheFkeys( fkeyReader );

            if ( pkeyReader ) 
                dbObject->CachePkeys( pkeyReader );

            // Load the components into the db object.
            FdoSmPhTableP table = dbObject->SmartCast<FdoSmPhTable>();

            if ( table ) {
                if ( ukeyReader ) 
                    table->CacheUkeys( ukeyReader );

                if ( ckeyReader ) 
                    table->CacheCkeys( ckeyReader );
            }

            // Load the components into the db object.
            FdoSmPhViewP view = dbObject->SmartCast<FdoSmPhView>();

            if ( view ) {
                if ( viewReader ) 
                    view->CacheView( viewReader );
            }
        }
    }

    // At this point, all geometric columns have been bulk loaded so need
    // to bulk load spatial contexts as well.
    GetManager()->SetBulkLoadSpatialContexts(true);
}

FdoSmPhDbObjectP FdoSmPhOwner::CacheDbObject(
    FdoPtr<FdoSmPhRdDbObjectReader> reader
)
{
    FdoStringP objName = reader->GetString(L"", L"name");
    FdoSmPhDbObjectP pDbObject = GetDbObjects()->FindItem( objName );

    if ( !pDbObject ) {
        pDbObject = NewDbObject( objName, FdoSchemaElementState_Unchanged, reader);

        if ( pDbObject ) {
            // Database object found, add it the the cache.
            GetDbObjects()->Add( pDbObject );

            // Now that it's cached, it no longer needs to be a fetch candidate
            RemoveCandDbObject( pDbObject->GetName() );
        }
    }


    return pDbObject;
}

FdoSmPhDbObjectP FdoSmPhOwner::NewDbObject(
    FdoStringP objName,
    FdoSchemaElementState elementState,
    FdoPtr<FdoSmPhRdDbObjectReader> reader
)
{
    FdoSmPhDbObjectP pDbObject;

    // Instantiate the right type of database object
    switch( reader->GetType() ) {
    case FdoSmPhDbObjType_Table:
        pDbObject = NewTable(objName, FdoSchemaElementState_Unchanged, reader);
        break;

    case FdoSmPhDbObjType_View:
        // TODO: Find out if there is a way to get the root table info.
        pDbObject = NewView(objName, L"", L"", L"", FdoSchemaElementState_Unchanged, reader);
        break;
    }

    return pDbObject;
}

void FdoSmPhOwner::DiscardDbObject( FdoSmPhDbObject* dbObject )
{
    if ( mDbObjects )
        mDbObjects->Remove( dbObject );
}

void FdoSmPhOwner::AddCandDbObject( FdoStringP objectName )
{
    // No need for fetch candidates when all objects for owner are cached. 
    // Bulk fetching candidates is pointless when fetch size is 1.
    if ( (!mDbObjectsCached) && (GetCandFetchSize() > 1) ) {
	    FdoSmPhDbObjectP pDbObject = GetDbObjects()->FindItem(objectName);

        if ( !pDbObject ) {
            FdoDictionaryElementP elem = mCandDbObjects->FindItem( objectName );
            
            if ( !elem ) {
                elem = FdoDictionaryElement::Create( objectName, L"" );
                mCandDbObjects->Add( elem );
            }
        }
    }
}

void FdoSmPhOwner::RemoveCandDbObject( FdoStringP objectName )
{
    FdoInt32 ix = mCandDbObjects->IndexOf(objectName);
    if ( ix >= 0 ) 
        mCandDbObjects->RemoveAt( ix );

}

void FdoSmPhOwner::AddCandIndex( FdoStringP objectName )
{
    // Bulk fetching candidates is pointless when fetch size is 1.
    if ( GetCandFetchSize() > 1 ) {
        FdoDictionaryElementP elem = mCandIndexes->FindItem( objectName );
            
        if ( !elem ) {
            elem = FdoDictionaryElement::Create( objectName, L"" );
            mCandIndexes->Add( elem );
        }
    }
}

void FdoSmPhOwner::RemoveCandIndex( FdoStringP objectName )
{
    FdoInt32 ix = mCandIndexes->IndexOf(objectName);
    if ( ix >= 0 ) 
        mCandIndexes->RemoveAt( ix );

}

FdoSchemaExceptionP FdoSmPhOwner::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	// Tack on errors for this element
	FdoSchemaExceptionP pException = FdoSmPhDbElement::Errors2Exception(pFirstException);

    if ( mDbObjects ) {
    	// Add errors for the owner's database objects.
	    for ( int i = 0; i < mDbObjects->GetCount(); i++ )
    		pException = mDbObjects->RefItem(i)->Errors2Exception(pException);
    }

	return pException;
}

void FdoSmPhOwner::Discard()
{
    ((FdoSmPhDatabase*) GetParent())->DiscardOwner( this );
}

void FdoSmPhOwner::OnAfterCommit()
{
    // An object previously not found might get created on commit.
    // Therefore, clear the not found list since it is now stale. 
    mNotFoundObjects->Clear();
}

void FdoSmPhOwner::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<owner name=\"%s\">\n",
			(const char*) FdoStringP(GetName())
	);

	if ( mDbObjects && ref == 0 ) {
		for ( int i = 0; i < mDbObjects->GetCount(); i++ ) 
			mDbObjects->RefItem(i)->XMLSerialize(xmlFp, ref);
	}

	fprintf( xmlFp, "</owner>\n" );

}

void FdoSmPhOwner::CommitChildren( bool isBeforeParent )
{
    int i;

    if ( mDbObjects ) {
        // Handle foreign key deletes before table deletes to keep foreign 
        // keys from dangling
        if ( isBeforeParent ) 
            CommitFkeys(isBeforeParent);

        // Commit the database objects. Make sure tables are deleted after other
        // objects, since these objects might depend on these tables. Similarly
        // make sure tables are added before other objects. 

        for ( i = (mDbObjects->GetCount() - 1); i >= 0; i-- ) {
            FdoSmPhDbObjectP dbObject = mDbObjects->GetItem(i);
            FdoSmPhTableP table = dbObject.p->SmartCast<FdoSmPhTable>();
            if ( isBeforeParent == (table == NULL) ) 
                dbObject->Commit( true, isBeforeParent );
        }

        for ( i = (mDbObjects->GetCount() - 1); i >= 0; i-- ) {
            FdoSmPhDbObjectP dbObject = mDbObjects->GetItem(i);
            FdoSmPhTableP table = dbObject.p->SmartCast<FdoSmPhTable>();
            if ( isBeforeParent == (table != NULL) ) 
                dbObject->Commit( true, isBeforeParent );
        }

        // Handle foreign key adds before table adds to ensure all referenced tables
        // and columns are present.
        if ( !isBeforeParent ) 
            CommitFkeys(isBeforeParent);
    }
}

void FdoSmPhOwner::CommitFkeys( bool isBeforeParent )
{
    int i;

    if ( mDbObjects ) {
        for ( i = (mDbObjects->GetCount() - 1); i >= 0; i-- ) {
            FdoSmPhDbObjectP dbObject = mDbObjects->GetItem(i);
            dbObject->CommitFkeys( isBeforeParent );
        }
    }
}

void FdoSmPhOwner::ReserveDbObjectName( FdoStringP objectName, bool bCheckReserved  )
{
	if ( !bCheckReserved || !IsDbObjectNameReserved(objectName) ) 
		mReservedDbObjectNames->Add( objectName );
}

bool FdoSmPhOwner::IsDbObjectNameReserved( FdoStringP objectName )
{
	int i;
	bool bReserved = false;

	// Check the in-memory reserved list first. Implementing this list as a
	// collection seems to provide reasonable performance. However, we might
	// have to change it to a map if this changes in the future.

	for ( i = 0; i < mReservedDbObjectNames->GetCount(); i++ ) {
		if ( objectName.ICompare( mReservedDbObjectNames->GetString(i) ) == 0 ) {
			bReserved = true;
			break;
		}
	}

	// If not in reserved list, check if object exists.

	if ( !bReserved && FindDbObject(objectName) )
		bReserved = true;

    // The rest of the checks are unnecessary if this datastore does not yet exist.
    if ( GetElementState() != FdoSchemaElementState_Added ) {
        // Also check if this name is used by a constraint
        FdoSmPhRdConstraintReaderP conRdr = CreateConstraintReader(objectName);
        if ( conRdr->ReadNext() ) 
            bReserved = true;

	    if ( !bReserved ) {
		    // Still not found. Check if it is referenced by the metaschema.

            FdoSmPhDbObjectP classDef = FindDbObject( GetManager()->GetDcDbObjectName(L"f_classdefinition") );
            FdoSmPhDbObjectP attDef = FindDbObject( GetManager()->GetDcDbObjectName(L"f_attributedefinition") );
            FdoStringP localObjectName = GetManager()->DbObject2MetaSchemaName(objectName);

            if ( classDef && attDef ) {
		        FdoStringP statement = 
			        FdoStringP::Format( 
				        L"select 1 from %ls where tablename in ( %ls, %ls ) union select 1 from %ls where tablename in ( %ls, %ls )",
				        (FdoString*)(GetManager()->GetDcDbObjectName(L"f_classdefinition")),
                        (FdoString*) GetManager()->FormatSQLVal(objectName, FdoSmPhColType_String),
                        (FdoString*) GetManager()->FormatSQLVal(localObjectName, FdoSmPhColType_String),
                        (FdoString*)(GetManager()->GetDcDbObjectName(L"f_attributedefinition")),
				        (FdoString*) GetManager()->FormatSQLVal(objectName, FdoSmPhColType_String),
				        (FdoString*) GetManager()->FormatSQLVal(localObjectName, FdoSmPhColType_String)
			        );

		        // Running a query for each table is not the most efficient way to do 
		        // things but this query is only run when schemas are updated. 

                FdoSmPhRowP row = new FdoSmPhRow( GetManager(), L"findtable" );
                FdoSmPhColumnP column = FdoSmPhDbObjectP(row->GetDbObject())->CreateColumnInt16( L"one", true );
                FdoSmPhFieldP field = new FdoSmPhField( row, L"one", column, L"", false );
		        FdoSmPhRdQueryReaderP tableRef = GetManager()->CreateQueryReader( row, statement );

		        if ( tableRef->ReadNext() ) {
			        // Table is referenced by metaschema
			        bReserved = true;
			        mReservedDbObjectNames->Add(objectName);
		        }
            }
	    }
    }

	return(bReserved);
}

FdoSmPhDbObjectP FdoSmPhOwner::CacheCandDbObjects( FdoStringP objectName )
{
    FdoSmPhDbObjectP retDbObject;
    FdoDictionaryP candDbObjects = FdoDictionary::Create();
    FdoInt32 fetchSize = GetCandFetchSize();
    FdoInt32 ix = mCandDbObjects->IndexOf( objectName );

    // No candidate fetching if object not a candidate or fetch size too small.
    if ( (ix < 0) || (fetchSize < 2) ) 
        return retDbObject;

    // Fetch some of the other candidates. Get the ones in the neighbourhood of the given object.
    FdoInt32 start = ix - (fetchSize/2);
    if ( start < 0 ) 
        start = 0;

    // Build the candidates list.
    FdoStringsP cands = FdoStringCollection::Create();
    FdoInt32 end;

    for ( end = start; (end < mCandDbObjects->GetCount()) && (cands->GetCount() < fetchSize); end++ ) {
        FdoDictionaryElementP elem = mCandDbObjects->GetItem(end);
        cands->Add( elem->GetName() );
    }

    // Pad out list with empty object names. 
    // Candidate fetches are done by binding candidate names into selects. Selects can be re-used
    // if number of bind variables stays consistent
    while ( cands->GetCount() < fetchSize ) 
        cands->Add( L"" );

    // Remove candidates from candidate list.
    // Put in temporary list to track which ones were not found.
    for  ( ix = (end - 1); ix >= start; ix-- ) {
        FdoDictionaryElementP elem = mCandDbObjects->GetItem( ix );
        candDbObjects->Add( elem );
        mCandDbObjects->RemoveAt(ix);
    }

    // Read the candidates.
    FdoSmPhRdDbObjectReaderP objReader;
    FdoSmPhRdPkeyReaderP pkeyReader;
    FdoSmPhRdConstraintReaderP ukeyReader;
    FdoSmPhRdConstraintReaderP ckeyReader;
    FdoSmPhRdColumnReaderP columnReader;
    FdoSmPhRdBaseObjectReaderP baseObjectReader;

    // Create reader for candidate db objects.
    objReader = CreateDbObjectReader( cands );
    if ( !objReader ) 
        return retDbObject;

    bool first = true;

    while ( objReader && objReader->ReadNext() ) {
        // Caching db object components so create readers for components.
        // This function does interleaved fetches from each reader so all readers
        // (including dbObject reader) must return rows ordered by dbObject name.
        //
        // For datastores with MetaSchema, only columns and constraints need to be bulk fetched.
        // Primary and Foreign keys, and indexes are never fetched.
        //
        // Doing a single query per owner for each component is more efficient than
        // a query per dbObject.
        // The join is used to limit results to those needed for this schema.

        if ( first ) {
            if ( GetBulkLoadPkeys() ) 
                pkeyReader = CreatePkeyReader();

            if ( GetManager()->GetBulkLoadConstraints() ) {
                ukeyReader = CreateConstraintReader( cands, L"U" );
                ckeyReader = CreateConstraintReader( cands, L"C" );
            }

            columnReader = CreateColumnReader( cands );

            baseObjectReader = CreateBaseObjectReader(cands);
 
            first = false;
        }

        // Cache the current dbObject
        FdoSmPhDbObjectP dbObject = CacheDbObject( objReader );
        FdoDictionaryElementP elem = candDbObjects->FindItem( dbObject->GetName() );
       if ( elem )
            // Mark it has having been read (fetched).
            elem->SetValue(L"f");

        if ( dbObject ) {
            if ( objectName == dbObject->GetName() ) 
                // Keep track of the object asked for so it can be returned.
                retDbObject = dbObject;

            // Load the components into the db object.
            FdoSmPhTableP table = dbObject->SmartCast<FdoSmPhTable>();

            if ( columnReader ) 
                dbObject->CacheColumns( columnReader );

            if ( pkeyReader ) 
                dbObject->CachePkeys( pkeyReader );

            if ( table ) {
                if ( ukeyReader ) 
                    table->CacheUkeys( ukeyReader );

                if ( ckeyReader ) 
                    table->CacheCkeys( ckeyReader );
            }

            if ( baseObjectReader ) 
                dbObject->CacheBaseObjects( baseObjectReader );
        }
    }

    // Add any candidates not fetched to the not found list.
    for  ( ix = 0; ix < candDbObjects->GetCount(); ix++ ) {
        FdoDictionaryElementP elem = candDbObjects->GetItem( ix );
        if ( wcslen(elem->GetValue()) == 0 )
            mNotFoundObjects->Add( elem );
    }

    return retDbObject;
}

void FdoSmPhOwner::CacheCandIndexes( FdoStringP objectName )
{
    FdoInt32 fetchSize = GetCandFetchSize();

    // Can't bulk load if fetch size too small or no db objects
    // have been cached
    if ( (fetchSize < 2) || (mDbObjects->GetCount() == 0) ) 
        return;

    // Check if requested dbObject is on the index candidates list
    FdoDictionaryP candIndexes = FdoDictionary::Create();
    FdoInt32 ix = mCandIndexes->IndexOf( objectName );
    FdoInt32 idx;

    if ( ix < 0 ) {
        // Not on the candidates list. May need to initialize the list with tables
        // whose indexes are needed for reverse-engineering.
        // First add tables without primary key, or tables with multiple geometric columns
        LoadIndexTableCands();
        ix = mCandIndexes->IndexOf( objectName );

        if ( ix < 0 )
            // Still not on candidates list. Object might be a table with pkey, that
            // has dependent view that does not include all of the table's pkey columns
            // (in this case, we try to generate view identity from a unique index on the
            // table. Load all tables with dependent views (where the view has no direct pkey).
            LoadIndexRootTableCands();
    
        ix = mCandIndexes->IndexOf( objectName );
    }

    // dbObject is still not a candidate, fall back to loading only the indexes
    // for that dbObject.
    if ( ix < 0 ) 
        return;

    FdoStringsP cands = FdoStringCollection::Create();
    FdoSmPhRdIndexReaderP indexReader;

    // The following block creates an index reader to fetch indexes for given object plus
    // a certain number of other candidates (bulk fetched for better performance).
    // This type of fetch is done by binding all of the candidates into a select statement.
    // When there are many candidates, it is more efficient to fetch all indexes.
    // Therefore, the following is skipped if all dbObjects for this owner have been cached,
    // and at least half of them are candidates.
    //
    // TODO: check if 50% is the optimal threshold.
    if ( (!mDbObjectsCached) || (((double)(mCandIndexes->GetCount())) / ((double)(mDbObjects->GetCount())) < 0.5) ) {
        // Fetch some of the other candidates. Get the ones in the neighbourhood of the given object.
        FdoInt32 start = ix - (fetchSize/2);
        if ( start < 0 ) 
            start = 0;

        // Build the candidates list.
        FdoInt32 end;

        for ( end = start; (end < mCandIndexes->GetCount()) && (cands->GetCount() < fetchSize); end++ ) {
            FdoDictionaryElementP elem = mCandIndexes->GetItem(end);
            cands->Add( elem->GetName() );
        }

        // Pad out list with empty object names. 
        // Candidate fetches are done by binding candidate names into selects. Selects can be re-used
        // if number of bind variables stays consistent
        while ( cands->GetCount() < fetchSize ) 
            cands->Add( L"" );

        // Remove candidates from candidate list.
        // Put in temporary list to track which ones were not found.
        for  ( idx = (end - 1); idx >= start; idx-- ) {
            FdoDictionaryElementP elem = mCandIndexes->GetItem( idx );
            candIndexes->Add( elem );
            mCandIndexes->RemoveAt(idx);
        }

        // Read the candidates.

        // Create reader for candidate indexes.
        indexReader = CreateIndexReader( cands );
    }

    if ( (!indexReader) && mDbObjectsCached ) {
        // Reader for candidates not created. There were either too many candidates or
        // the provider does not support index retrieval by binding multiple dbObjects.

        // If all dbObjects are cached then load all indexes for this owner. Otherwise,
        // fall back to loading one dbObject at a time.
        indexReader = CreateIndexReader();
        if ( indexReader ) {
            // Candidates list no longer needed. All indexes will be loaded.
            mCandIndexes->Clear();

            // Reset the current candidates list to include all dbObjects for this owner.
            candIndexes->Clear();

            for ( idx = 0; idx < mDbObjects->GetCount(); idx++ ) {
                FdoSmPhDbObjectP dbObject = mDbObjects->GetItem(idx);
                FdoDictionaryElementP elem = FdoDictionaryElement::Create( dbObject->GetName(), L"" );
                candIndexes->Add( elem );
            }
        }
    }

    if ( indexReader && indexReader->ReadNext() ) {
        // An index reader was created so cache the indexes that it fetches.

        while ( !indexReader->IsEOF()) {
            // Get the dbObject name for the currently read row.
            FdoStringP objectName = indexReader->GetString( L"", L"table_name" );

            // Cache all indexes for this dbObject
            if ( !CacheObjectIndexes( indexReader ) )
                // If the indexes were cached, the index reader is advanced to just 
                // after the current dbObject. However, if they weren't, then must
                // advance the reader here. This can happen if the current dbObject
                // is not in the cache.
                indexReader->ReadNext();

            FdoDictionaryElementP elem = candIndexes->FindItem( objectName );
            if ( elem )
                // Mark current candidate dbObject has having been read.
                elem->SetValue(L"f");
        }

        // Do index caching for any candidates for which no indexes were found. 
        // This forces the index list for each candidate to be initialized so that
        // we won't retry loading its indexes. The retry would be done with one query
        // per candidate, which would be slow.
        // At this point, the index reader is at EOF, so no indexes are cached.
        for  ( ix = 0; ix < candIndexes->GetCount(); ix++ ) {
            FdoDictionaryElementP elem = candIndexes->GetItem( ix );
            if ( wcslen(elem->GetValue()) == 0 ) {
                FdoSmPhTableP table = this->FindDbObject( elem->GetName() ).p->SmartCast<FdoSmPhTable>();
                if (table )
                    table->CacheIndexes( indexReader );
            }
        }
    }
}

bool FdoSmPhOwner::GetBulkLoadPkeys()
{
    return mBulkLoadPkeys;
}

void FdoSmPhOwner::SetBulkLoadPkeys( bool bulkLoad )
{
    mBulkLoadPkeys = bulkLoad;
}

bool FdoSmPhOwner::CacheObjectIndexes( FdoSmPhRdIndexReaderP indexReader )
{
    // Get current dbObject name
    FdoStringP objectName = indexReader->GetString( L"", L"table_name" );
    FdoSmPhTableP table = mDbObjects->FindItem( objectName )->SmartCast<FdoSmPhTable>(true);
 
    if (table )
        // dbObject is cached so load its indexes from the reader.
        table->CacheIndexes( indexReader );

    return ( table != NULL );
}

void FdoSmPhOwner::LoadBaseObjectCands()
{
    int idx1;
    int idx2;

    if ( mDbObjects ) {
        // Check each dbObject not yet checked
        for ( idx1 = mNextBaseCandIdx; idx1 < mDbObjects->GetCount(); idx1++ ) {
            FdoSmPhDbObjectP dbObject = mDbObjects->GetItem(idx1);

            FdoSmPhBaseObjectsP baseObjects = dbObject->GetBaseObjects();

            // Add each base object to it's owner's candidates list.
            for ( idx2 = 0; idx2 < baseObjects->GetCount(); idx2++ ) {
                FdoSmPhBaseObjectP baseObject = baseObjects->GetItem(idx2);

                FdoSmPhOwnerP baseOwner = GetManager()->FindOwner( baseObject->GetOwnerName(), baseObject->GetDatabaseName() );

                if ( baseOwner ) {
                    baseOwner->AddCandDbObject( baseObject->GetObjectName() );
                    // Need primary keys of base objects (to determine view identity properties)
                    // so bulk load them.
                    baseOwner->SetBulkLoadPkeys(true);
                }
            }
        }

        mNextBaseCandIdx = mDbObjects->GetCount();
    }
}

void FdoSmPhOwner::LoadIndexTableCands()
{
    FdoInt32 idx;

    if ( mDbObjects ) {
        // Check each dbObject not yet checked
        for ( idx = mNextIndexTableCandIdx; idx < mDbObjects->GetCount(); idx++ ) {
            FdoSmPhDbObjectP dbObject = mDbObjects->GetItem( idx );
            FdoSmPhTableP table = dbObject->SmartCast<FdoSmPhTable>();

            // Skip tables whose indexes are already loaded
            if ( table && !table->IndexesLoaded() ) {
                bool isIndexCand = false;

                if ( table->GetPkeyColumns()->GetCount() == 0 ) {
                    // Indexes are used to generate identity when no primary key, so this
                    // table is a candidate for bulk loading indexes.
                    isIndexCand = true;
                }
                else {
                    FdoSmPhColumnsP columns = table->GetColumns();
                    int i;
                    int geomCount = 0;

                    for ( i = 0; i < columns->GetCount(); i++ ) {
                        FdoSmPhColumnP column = columns->GetItem(i);
                        if ( column->GetType() == FdoSmPhColType_Geom ) 
                            geomCount++;

                        if ( geomCount > 1 ) {
                            // Spatial indexes are used to pick main geometry when 
                            // table has multiple geometric columns.
                            isIndexCand = true;
                            break;
                        }
                    }
                }

                if ( isIndexCand ) 
                    AddCandIndex( table->GetName() );
            }
        }

        mNextIndexTableCandIdx = mDbObjects->GetCount();
    }
}

void FdoSmPhOwner::LoadIndexRootTableCands()
{
    FdoInt32 idx;

    if ( mDbObjects ) {
        // Check each dbObject not yet checked
        for ( idx = mNextIndexRootTableCandIdx; idx < mDbObjects->GetCount(); idx++ ) {
            FdoSmPhDbObjectP dbObject = mDbObjects->GetItem( idx );
            if ( dbObject->GetPkeyColumns()->GetCount() == 0 ) {
                FdoSmPhDbObjectP rootObject = dbObject->GetLowestRootObject();
                
                // Loading indexes into this owner so skip any base objects in 
                // other owners.
                if ( GetQName() == rootObject->GetParent()->GetQName() ) {
                    if ( rootObject && (dbObject->GetQName() != rootObject->GetQName()) ) {
                        // Object has no primary key but has a root object. We might 
                        // use root object's indexes to generate object's identity.
                        FdoSmPhTableP table = rootObject->SmartCast<FdoSmPhTable>();

                        if ( table && !table->IndexesLoaded() )
                            AddCandIndex( table->GetName() );
                    }
                }
            }
        }

        mNextIndexRootTableCandIdx = mDbObjects->GetCount();
    }
}

FdoInt32 FdoSmPhOwner::GetCandFetchSize()
{
    // 50 bound objects seems to give optimal performance for Oracle. 
    return 50;
}

void FdoSmPhOwner::LoadLtLck()
{
    // Reading options for owners in other database instances is not yet supported.
    if ( (!mLtLckLoaded) && (wcslen(GetParent()->GetName()) == 0) ) {
        mLtLckLoaded = true;
        FdoSmPhOptionsReaderP optRdr = GetManager()->CreateOptionsReader( GetName() );

        // read each option, looking for the long transaction and locking options.
        while ( optRdr->ReadNext() ) {
            FdoStringP optName = optRdr->GetName();

            if ( optName == L"LT_MODE" )
                mLtMode = (FdoLtLockModeType)(optRdr->GetValue().ToLong());
            else if ( optName == L"LOCKING_MODE" ) 
                mLckMode = (FdoLtLockModeType)(optRdr->GetValue().ToLong());
        }
    }

    LoadLckTypes(mLckMode);
}

void FdoSmPhOwner::LoadLckTypes( FdoLtLockModeType lockMode )
{
    FdoLockType* noLockTypes = new FdoLockType[1];
    noLockTypes[0] = FdoLockType_Transaction;
    FdoSmPhLockTypesP lockTypes = new FdoSmPhLockTypes( NoLtLock, 1, noLockTypes );
    GetLockTypesCollection()->Add( lockTypes );

    if ( lockMode == FdoMode ) {
        FdoLockType* fdoLockTypes = new FdoLockType[3];
        fdoLockTypes[0] = FdoLockType_Transaction;
        fdoLockTypes[1] = FdoLockType_Exclusive;
        fdoLockTypes[2] = FdoLockType_LongTransactionExclusive;

        lockTypes = new FdoSmPhLockTypes( FdoMode, 3, fdoLockTypes );
        GetLockTypesCollection()->Add( lockTypes );
    }
}
   
void FdoSmPhOwner::LoadSchemaInfo()
{
    // Load only if this is an existing owner.
    if ( (!mSchemaInfoLoaded) && 
            ((GetElementState() == FdoSchemaElementState_Unchanged) ||
             (GetElementState() == FdoSchemaElementState_Modified)
            ) 
    ) {
        mSchemaInfoLoaded = true;

        // Read the schema info record for the owner (datastore).
        FdoSmPhSchemaReaderP schemaRdr = new FdoSmPhSchemaReader( FDO_SAFE_ADDREF(this), true );

        if ( schemaRdr->ReadNext() ) {
            SetDescription( schemaRdr->GetDescription() );
            mSchemaVersion = schemaRdr->GetVersion();
        }
    }
}

FdoSmPhDbObjectsP FdoSmPhOwner::GetDbObjects()
{
    if ( !mDbObjects ) 
        // Created object cache.
        mDbObjects = new FdoSmPhDbObjectCollection(NULL);

    return mDbObjects;
}

FdoSmPhLockTypesCollection* FdoSmPhOwner::GetLockTypesCollection()
{
    if ( !mLockTypes ) 
        mLockTypes = new FdoSmPhLockTypesCollection();

    return mLockTypes;
}

void FdoSmPhOwner::LoadSpatialContexts( FdoStringP dbObjectName )
{
    FdoStringP  scInfoTable = GetManager()->GetRealDbObjectName( FdoSmPhMgr::ScInfoNoMetaTable );

    if ( !mSpatialContexts ) {
        mSpatialContexts = new FdoSmPhSpatialContextCollection();
        mSpatialContextGeoms = new FdoSmPhSpatialContextGeomCollection();
        // Make sure ScInfo table is loaded first, since real spatial 
        // context names can be determined from the columns in this table.
        DoLoadSpatialContexts( FdoSmPhMgr::ScInfoNoMetaTable );
    }

    if ( GetManager()->GetBulkLoadSpatialContexts() || (dbObjectName != scInfoTable) ) 
        DoLoadSpatialContexts( GetManager()->GetBulkLoadSpatialContexts() ? FdoStringP() : dbObjectName );
}

void FdoSmPhOwner::DoLoadSpatialContexts( FdoStringP dbObjectName )
{        
    if ( GetElementState() == FdoSchemaElementState_Added ) 
        return;

    if ( !mSpatialContextsLoaded ) {
        FdoInt32 firstScGeom = mSpatialContextGeoms->GetCount();

        // reverse-engineering. The PH schema object will return the appropiate reader or
	    // the default one.
        FdoSmPhRdSpatialContextReaderP scReader;
        
        if ( dbObjectName == L"") {
            // We're either asked to or forced to load all spatial contexts.
            scReader = CreateRdSpatialContextReader();
            mSpatialContextsLoaded = true;
        }
        else {
            // Incremental loading (SC's associated with given dbObject).
            scReader = CreateRdSpatialContextReader(dbObjectName);
        }
    
        while (scReader->ReadNext())
        {
            // Generate physical spatial context from current SpatialContextGeom
            FdoPtr<FdoByteArray> scExtent = scReader->GetExtent();
            FdoSmPhSpatialContextP sc = new FdoSmPhSpatialContext(
                GetManager(),
                scReader->GetSrid(),
                scReader->GetGeomColumnName(),
                scReader->GetDescription(),
                scReader->GetCoordinateSystem(),
                scReader->GetCoordinateSystemWkt(),
                scReader->GetExtentType(),
                scExtent,
                scReader->GetXYTolerance(),
                scReader->GetZTolerance()
            );

            if (NULL == sc.p)
    		    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

            // Create Spatial context geometry object and associate it with this scId
	        FdoSmPhSpatialContextGeomP  scgeom = new FdoSmPhSpatialContextGeom(
                                                            this,
														    scReader->GetGeomTableName(),
														    scReader->GetGeomColumnName(),
														    scReader->GetHasElevation(),
                                                            scReader->GetHasMeasure(),
                                                            scReader->IsDerived(),
                                                            sc,
                                                            mSpatialContexts
            );

            if (NULL == scgeom.p)
			    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

            if ( mSpatialContextGeoms->IndexOf(scgeom->GetName()) < 0 ) 
                mSpatialContextGeoms->Add( scgeom );	
        }

        // Resolve the SCGeoms that were loaded to their spatial contexts. This is triggered
        // by doing a GetSpatialContext on each SCGeom. This does the following:
        //  - coalesce and cache spatial contexts in mSpatialContexts
        //  - associates each derived SCGeom to the associated spatial context of its
        //    root SCGeom. For example, this is done for SQLServerSpatial to associate
        //    geometry columns in views to their spatial contexts, by associating to the 
        //    spatial context for the root column.

        FdoInt32 lastScGeom = mSpatialContextGeoms->GetCount() - 1;
        for ( FdoInt32 i = firstScGeom; i <= lastScGeom; i++ )
        {
            FdoSmPhSpatialContextGeomP  scgeom = mSpatialContextGeoms->GetItem(i);
            FdoSmPhSpatialContextP sc = scgeom->GetSpatialContext();
        }
    }
}

void FdoSmPhOwner::LoadCoordinateSystems( FdoSmPhRdCoordSysReaderP rdr )
{
    if ( !mCoordinateSystems ) 
        mCoordinateSystems = new FdoSmPhCoordinateSystemCollection();

    while ( rdr->ReadNext() ) {
        FdoSmPhCoordinateSystemP coordSys = new FdoSmPhCoordinateSystem(
            GetManager(),
            rdr->GetString(L"", L"name"),
            L"",
            rdr->GetInt64(L"", L"srid"),
            rdr->GetString(L"", L"wktext")
        );

        // Add the coordinate system to the cache if not already present.
        if ( mCoordinateSystems->IndexOf(coordSys->GetName()) < 0 )
            mCoordinateSystems->Add( coordSys );
    }
}
