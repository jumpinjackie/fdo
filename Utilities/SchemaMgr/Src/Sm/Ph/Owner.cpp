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
    mDbObjectsCached(false)
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
    if ( dbObject == L"" ) 
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

FdoSmPhSpatialContextGeomsP FdoSmPhOwner::GetSpatialContextGeoms()
{
    LoadSpatialContexts();

    return mSpatialContextGeoms;
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

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhOwner::CreateFkeyReader() const
{
    return (FdoSmPhRdFkeyReader*) NULL;
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhOwner::CreateIndexReader() const
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

FdoPtr<FdoSmPhRdSpatialContextReader> FdoSmPhOwner::CreateRdSpatialContextReader()
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

        // No need for fetch candidates since this function caches all object for this owner.
        mCandDbObjects->Clear();

        FdoSmPhRdDbObjectReaderP objReader;
        FdoSmPhRdColumnReaderP columnReader;
        FdoSmPhRdBaseObjectReaderP baseObjectReader;
        FdoSmPhRdConstraintReaderP ukeyReader;
        FdoSmPhRdConstraintReaderP ckeyReader;
        FdoSmPhRdFkeyReaderP fkeyReader;
        FdoSmPhRdIndexReaderP indexReader;
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
            columnReader = CreateColumnReader();
            baseObjectReader = CreateBaseObjectReader();
            ukeyReader = CreateConstraintReader( L"", L"U" );
            ckeyReader = CreateConstraintReader( L"", L"C" );
            fkeyReader = CreateFkeyReader();
            indexReader = CreateIndexReader();
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

                    if ( indexReader ) 
                        table->CacheIndexes( indexReader );
                }
            }
        }
    }

    return GetDbObjects();
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
	fprintf( xmlFp, "<owner name=\"%ls\">\n",
			GetName()
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

    // Also check if this name is used by a constraint
    FdoSmPhRdConstraintReaderP conRdr = CreateConstraintReader(objectName);
    if ( conRdr->ReadNext() ) 
        bReserved = true;

	if ( !bReserved ) {
		// Still not found. Check if it is referenced by the metaschema.

        FdoSmPhDbObjectP classDef = FindDbObject( GetManager()->GetDcDbObjectName(L"F_CLASSDEFINITION") );
        FdoSmPhDbObjectP attDef = FindDbObject( GetManager()->GetDcDbObjectName(L"F_ATTRIBUTEDEFINITION") );
        FdoStringP localObjectName = GetManager()->DbObject2MetaSchemaName(objectName);

        if ( classDef && attDef ) {
		    FdoStringP statement = 
			    FdoStringP::Format( 
				    L"select 1 from %ls where tablename in ( %ls, %ls ) union select 1 from %ls where tablename in ( %ls, %ls )",
				    (FdoString*)(GetManager()->GetDcDbObjectName(L"F_CLASSDEFINITION")),
                    (FdoString*) GetManager()->FormatSQLVal(objectName, FdoSmPhColType_String),
                    (FdoString*) GetManager()->FormatSQLVal(localObjectName, FdoSmPhColType_String),
                    (FdoString*)(GetManager()->GetDcDbObjectName(L"F_ATTRIBUTEDEFINITION")),
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
    FdoSmPhRdConstraintReaderP ukeyReader;
    FdoSmPhRdConstraintReaderP ckeyReader;
    FdoSmPhRdColumnReaderP columnReader;

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
            if ( GetManager()->GetBulkLoadConstraints() ) {
                ukeyReader = CreateConstraintReader( cands, L"U" );
                ckeyReader = CreateConstraintReader( cands, L"C" );
            }

            columnReader = CreateColumnReader( cands );

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

            if ( table ) {
                if ( ukeyReader ) 
                    table->CacheUkeys( ukeyReader );

                if ( ckeyReader ) 
                    table->CacheCkeys( ckeyReader );
            }
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

void FdoSmPhOwner::LoadSpatialContexts()
{
    if ( !mSpatialContexts ) {
        mSpatialContexts = new FdoSmPhSpatialContextCollection();
        mSpatialContextGeoms = new FdoSmPhSpatialContextGeomCollection();

		// reverse-engineering. The PH schema object will return the appropiate reader or
		// the default one.
        FdoSmPhRdSpatialContextReaderP scReader = CreateRdSpatialContextReader();
			
		FdoInt32	currSC = 0;

        while (scReader->ReadNext())
        {
			FdoStringP	scName = L"Default";
				
			if ( currSC != 0 )
				scName = FdoStringP::Format(L"%ls_%ld", scReader->GetName(), currSC);

            // Generate physical spatial context from current SpatialContextGeom
            FdoPtr<FdoByteArray> scExtent = scReader->GetExtent();
			FdoSmPhSpatialContextP sc = new FdoSmPhSpatialContext(
                GetManager(),
                scReader->GetSrid(),
                scName,
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

			int indexSC = mSpatialContexts->FindExistingSC( sc );

			// New Spatial context definition, add it to collection
			if ( indexSC == -1 )
			{
				sc->SetId( currSC ); 
				mSpatialContexts->Add( sc );
				currSC++;
			}

            // Create Spatial context geometry object and associate it with this scId
		    FdoSmPhSpatialContextGeomP  scgeom = new FdoSmPhSpatialContextGeom(
                                                            GetManager(),
			  											    ( indexSC != -1 )? indexSC : currSC - 1,
															scReader->GetGeomTableName(),
															scReader->GetGeomColumnName(),
															scReader->GetHasElevation(),
                                                            scReader->GetHasMeasure()
            );

            if (NULL == scgeom.p)
				throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

            mSpatialContextGeoms->Add( scgeom );												
        }
    }
}

