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
#include <Sm/Ph/Rd/DbObjectReader.h>
#include <Sm/Ph/Rd/CoordSysReader.h>
#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Ph/OptionsReader.h>
#include <Sm/Ph/SchemaReader.h>


FdoSmPhOwner::FdoSmPhOwner(
    FdoStringP name, 
    bool hasMetaSchema,
    const FdoSmPhDatabase* pDatabase,
	FdoSchemaElementState elementState
) : 
    FdoSmPhDbElement(name, (FdoSmPhMgr*) NULL, pDatabase, elementState )
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
    mReservedDbObjectNames = FdoStringCollection::Create();
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

FdoLtLockModeType FdoSmPhOwner::GetLtMode()
{
    LoadLtLck();

	return mLtMode;
}

void FdoSmPhOwner::SetLckMode( FdoLtLockModeType LckMode )
{
    LoadLtLck();

	mLckMode = LckMode;
}

FdoLtLockModeType FdoSmPhOwner::GetLckMode()
{
    LoadLtLck();

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

    if ( !pDbObject ) {
        // Not in cache so read it in.
        FdoSmPhRdDbObjectReaderP reader = CreateDbObjectReader(dbObject);

        if ( reader->ReadNext() )
            pDbObject = CacheDbObject( reader );
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

FdoPtr<FdoSmPhRdCoordSysReader> FdoSmPhOwner::CreateCoordSysReader( FdoStringP csysName) const
{
    return new FdoSmPhRdCoordSysReader();
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

FdoSmPhDbObjectP FdoSmPhOwner::CacheDbObject(
    FdoPtr<FdoSmPhRdDbObjectReader> reader
)
{
    FdoStringP objName = reader->GetString(L"", L"name");
    FdoSmPhDbObjectP pDbObject = GetDbObjects()->FindItem( objName );

    if ( !pDbObject ) {
        pDbObject = NewDbObject( objName, FdoSchemaElementState_Unchanged, reader);

        if ( pDbObject ) 
            // Database object found, add it the the cache.
            GetDbObjects()->Add( pDbObject );
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
            // Only tables have foreign keys.
            FdoSmPhTableP table = mDbObjects->GetItem(i)->SmartCast<FdoSmPhTable>(true);
            if ( table ) 
                table->CommitFkeys( isBeforeParent );
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
            FdoSmPhColumnP column = row->GetDbObject()->CreateColumnInt16( L"one", true );
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

