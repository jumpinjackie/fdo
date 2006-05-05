 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdafx.h"
#include <Sm/SchemaManager.h>
#include <Sm/Error.h>

long FdoSchemaManager::mCurrRevision = 0;
FdoCommonThreadMutex FdoSchemaManager::mMutex;

FdoSchemaManager::FdoSchemaManager() :
    mRevision(mCurrRevision)
{
}

FdoSchemaManager::~FdoSchemaManager(void)
{
}

const FdoSmLpSchemaCollection* FdoSchemaManager::RefLogicalPhysicalSchemas() const
{
    return (FdoSmLpSchemaCollection*) ((FdoSchemaManager*) this)->GetLogicalPhysicalSchemas();
}


FdoSmPhMgrP FdoSchemaManager::GetPhysicalSchema(void)
{
	SynchRevision();

	if ( mPhysicalSchema == NULL ) 
		mPhysicalSchema = CreatePhysicalSchema();

	return mPhysicalSchema;
}

FdoSchemaMappingsP FdoSchemaManager::GetSchemaMappings( 
    FdoStringP schemaName, 
    bool bIncludeDefaults 
)
{
    FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
    const FdoSmLpSchemaCollection* pSchemas = RefLogicalPhysicalSchemas();
    FdoInt32 i;

    for ( i = 0; i < pSchemas->GetCount(); i++ ) {
        const FdoSmLpSchema* pSchema = pSchemas->RefItem(i);

        // Get mappings for current schema if it is the requested one. Get
        // mappings for all schemas if no schema name specified.
        // Always skip the MetaClass system schema.
        if ( ((schemaName.GetLength() == 0) && (wcscmp(pSchema->GetName(), FdoSmLpSchema::mMetaClassSchemaName) != 0)) || 
             (schemaName == pSchema->GetName()) 
        ) {
            FdoPhysicalSchemaMappingP mapping = pSchema->GetSchemaMappings( bIncludeDefaults );

            // mapping is NULL when there are no mappings for this schema.
            if ( mapping ) 
                mappings->Add( mapping );
        }
    }
    
    return mappings;
}

void FdoSchemaManager::DestroySchema( const wchar_t* schemaName  )
{
	try {
		FdoFeatureSchemaP pFeatSchema = FdoFeatureSchema::Create( schemaName, L"" );
		pFeatSchema->Delete();

        FdoPhysicalSchemaMappingP mapping;
		ApplySchema( pFeatSchema, mapping );

	}
	catch (FdoSchemaException* ex) {
		throw ex;
	}
	catch (FdoException* pException) {
		FdoSchemaException* pNewException =  FdoSchemaException::Create( 
			FdoStringP( 
                FdoSmError::NLSGetMessage(
					FDO_NLSID(FDOSM_219),
					schemaName
				)
			),
			pException
		);

		FDO_SAFE_RELEASE(pException);

		throw pNewException;
	}
}

void FdoSchemaManager::SynchPhysical( const wchar_t* schemaName, bool bRollbackOnly )
{
	int             i;
	bool            bDidSomething = false;

	try {
        // Proceed if synchronizing everything or the connection has rollback entries.
		if ( !bRollbackOnly || mPhysicalSchema->HasRollbackEntries() ) {
            // Clear schema manager cache in order to get the latest metaschema information.
            Clear();

            // Synchronize each feature schema in the DataStore, except for the special 
            // read-only MetaClass schema.
			for ( i = 0; i < GetLogicalPhysicalSchemas()->GetCount(); i++ ) {
				FdoSmLpSchemaP pSchema = mLpSchemas->GetItem(i);

				if ( wcscmp( pSchema->GetName(), FdoSmLpSchema::mMetaClassSchemaName ) != 0 ) { 
					if ( (wcslen(schemaName) == 0) || (wcscmp(schemaName, pSchema->GetName()) == 0) ) {
						pSchema->SynchPhysical(bRollbackOnly);
						bDidSomething = true;

                        if ( wcslen(schemaName) > 0 )
                            break;
					}
				}
			}

			if ( bDidSomething ) {
				// Check if there were errors and thow exception if any.
				FdoSchemaException* pException = mLpSchemas->Errors2Exception();

				if ( pException ) 
					throw pException;

                // Post the schema changes.
		        mLpSchemas->Commit();

				// Post the database table changes. 
				mPhysicalSchema->Commit();

				// Flag the fact that a schema was changed. This forces all Schema Managers
				// to reload their LogicalPhysical and Physical schemas.
                mMutex.Enter();
		        mCurrRevision++;
                mMutex.Leave();

                // Now that the rollbacks have been handled, clear the rollback cache for the 
                // connection.
                mPhysicalSchema->ClearRollback();
  			}
		}
	}
	catch (FdoException* pException) {
		try {
			// Some table changes might still have happened so force schema reloads
			// on failure.
            mMutex.Enter();
            mCurrRevision++;
            mMutex.Leave();
		}
		catch ( ... ) {
		}

		// message: Errors while synchronizing Schema tables with MetaData
		FdoSchemaException* pNewException = FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
    			FDO_NLSID(FDOSM_277)
			),
			pException
		);

		FDO_SAFE_RELEASE(pException);
		throw pNewException;
	}
	catch (...)
	{
        mMutex.Enter();
        mCurrRevision++;
        mMutex.Leave();
		throw;
	}
}

void FdoSchemaManager::ApplySchema( 
    FdoFeatureSchemaP pFeatSchema, 
    FdoPhysicalSchemaMappingP pOverrides,
    bool bIgnoreStates
)
{
    FdoSchemaElementState   schemaState = FdoSchemaElementState_Unchanged;

	// The MetaClass schema is a special schema describing Feature schema meta-classes.
	// Updates to this schema are not allowed.
	if ( wcscmp( pFeatSchema->GetName(), FdoSmLpSchema::mMetaClassSchemaName ) == 0 ) 
		throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
    			FDO_NLSID(FDOSM_220),
				pFeatSchema->GetName()
			)
		);

	try {
        if ( bIgnoreStates ) {
            // When ignoring element states, operation depends on whether
            // schema already exists.
        	GetLogicalPhysicalSchemas();

	        if ( mLpSchemas->FindItem(pFeatSchema->GetName()) )
                schemaState = FdoSchemaElementState_Modified;
            else
                schemaState = FdoSchemaElementState_Added;
        }
        else {
            schemaState = pFeatSchema->GetElementState();
        }

		switch ( schemaState ) {
		case FdoSchemaElementState_Added:
			NewSchema( pFeatSchema, pOverrides, bIgnoreStates );
			break;

		case FdoSchemaElementState_Deleted:
			DestroySchema( pFeatSchema );
			break;

		case FdoSchemaElementState_Modified:
		case FdoSchemaElementState_Unchanged:
			UpdateSchema( pFeatSchema, pOverrides, bIgnoreStates );
			break;
		}

		// Check if there were errors and thow exception if any.
		FdoSchemaExceptionP pException = mLpSchemas->Errors2Exception();

		if ( pException ) 
			throw FDO_SAFE_ADDREF((FdoSchemaException*) pException);

		// No errors, post the schema changes.
		mLpSchemas->Commit();
		// Post the database table changes. Table changes can't be rolled
		// back so it's best to do them last.
		mPhysicalSchema->Commit();

		// Flag the fact that a schema was changed. This forces all Schema Managers
		// to reload their LogicalPhysical and Physical schemas.
        mMutex.Enter();
		mCurrRevision++;
        mMutex.Leave();

        // Synchronize the element states in the Feature Schema
		pFeatSchema->AcceptChanges();
	}
	catch (FdoException* pException) {
		try {
			// Some table changes might still have happened so force schema reloads
			// on failure.
            mMutex.Enter();
			mCurrRevision++;
            mMutex.Leave();
		}
		catch ( ... ) {
		}

		FdoSchemaException* pNewException = FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_221),
				pFeatSchema->GetName()
			),
			pException
		);

		FDO_SAFE_RELEASE(pException);
		throw pNewException;
	}
	catch (...) {
		try {
			// Some table changes might still have happened so force schema reloads
			// on failure.
            mMutex.Enter();
			mCurrRevision++;
            mMutex.Leave();
		}
		catch ( ... ) {
		}

		throw;
	}
}

void FdoSchemaManager::CreateSpatialContext(
    FdoString* name,
    FdoString* description,
    FdoString* coordinateSystem,
    FdoString* coordinateSystemWkt,
    FdoSpatialContextExtentType extentType,
    FdoByteArray * extent,
    double xyTolerance,
    double zTolerance,
	bool   update
)
{
	// Nothing to do if the name is not set
	if ( wcscmp( name, L"" ) == 0 )
        throw FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_412)
			)
        );

	FdoSmPhMgrP physMgr = GetPhysicalSchema();

    FdoSmLpSpatialContextsP scs = GetLpSpatialContexts();

    FdoSmLpSpatialContextP sc = scs->FindItem(name);
    
    if ( sc && !update )
        throw FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
                //Cannot create Spatial Context '%1$ls', it already exists
				FDO_NLSID(FDOSM_278),
				name
			)
        );

	if ( !update ) {
		sc = scs->CreateSpatialContext(
			name, description,
			coordinateSystem, coordinateSystemWkt,
			extentType, extent,
			xyTolerance, zTolerance,
			true // ignore states flag. It is ignored
		);

		if (NULL == sc.p)
			throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
	}

	sc->SetElementState(update ? FdoSchemaElementState_Modified : FdoSchemaElementState_Added);

	// Check if there were errors and throw exception if any.
	FdoSchemaExceptionP pException = sc->Errors2Exception();
    if ( pException != NULL ) {
		throw FDO_SAFE_ADDREF((FdoSchemaException*)pException);
    }

	if ( !update )
		scs->Add(sc);

    // Post the changes.
    scs->Commit();

	// Flag the change to force re-loads by all Schema Managers.
    mMutex.Enter();
    mCurrRevision++;
    mMutex.Leave();
}


void FdoSchemaManager::NewSchema( 
    FdoFeatureSchemaP pFeatSchema, 
    FdoPhysicalSchemaMappingP pOverrides,
    bool bIgnoreStates
)
{
	if ( GetLogicalPhysicalSchemas()->RefItem( pFeatSchema->GetName() ) ) 
		throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_222),
				pFeatSchema->GetName()
			)
		);

	FdoSmLpSchemaP pLpSchema = mLpSchemas->CreateSchema( 
        pFeatSchema, 
        pOverrides, 
        bIgnoreStates
    );
	mLpSchemas->Add(pLpSchema);
}

void FdoSchemaManager::DestroySchema( FdoFeatureSchemaP pFeatSchema  )
{
	GetLogicalPhysicalSchemas();

	FdoSmLpSchemaP pLpSchema = mLpSchemas->FindItem( pFeatSchema->GetName() );

	if ( pLpSchema ) {
		pLpSchema->SetElementState( FdoSchemaElementState_Deleted );
	}
}


void FdoSchemaManager::DestroySpatialContext( FdoString * spatialContextName )
{
    FdoSmLpSpatialContextsP scs = GetLpSpatialContexts();

    FdoSmLpSpatialContextP sc = scs->FindItem(spatialContextName);
    FdoInt32 indx = scs->IndexOf(spatialContextName);

    if (NULL == sc.p || indx < 0)
		// Spatial Context '%1$ls' not found
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(FDO_NLSID(FDOSM_322), spatialContextName));

	// Check no geometry property is using this spatial context
	FdoSmLpSchemasP pSchemas = GetLogicalPhysicalSchemas();

	for ( int i = 0; i < pSchemas->GetCount(); i++ ) {
		FdoSmLpSchemaP pSchema = mLpSchemas->GetItem(i);

		const FdoSmLpClassCollection *pClasses = pSchema->RefClasses();

		for ( int j = 0; j < pClasses->GetCount(); j++ ) {
			const FdoSmLpClassDefinition* pClass = pClasses->RefItem(j);
		
			if ( pClass->GetClassType() != FdoClassType_FeatureClass )
				continue;

			FdoSmLpPropertiesP	pProps = ((FdoSmLpFeatureClass *) pClass)->GetProperties();
			for ( int k = 0; k < pProps->GetCount(); k++ ) {
				FdoSmLpGeometricPropertyP pGeomProp = pProps->GetItem(k)->SmartCast<FdoSmLpGeometricPropertyDefinition>(true);

				if ( pGeomProp && ( wcscmp(spatialContextName, pGeomProp->GetSpatialContextAssociation()) == 0) ) 
					// Cannot delete Spatial Context '%1$ls'. It is associated with geometric property '%2$ls'
			        throw FdoSchemaException::Create(
						FdoSmError::NLSGetMessage(FDO_NLSID(FDOSM_414), spatialContextName, (FdoString *) pGeomProp->GetQName()));
			}
		}
	}

    // Mark the spatial context for deletion.
    sc->SetElementState(FdoSchemaElementState_Deleted);

	// Check if there were errors and throw exception if any.
	FdoSchemaException* pException = scs->Errors2Exception();
	if ( pException != NULL ) 
		throw pException;

    // Post the change.
    scs->Commit();

    // Remove in-memory entry.
    scs->RemoveAt(indx);

	// Flag the change to force re-loads by all Schema Managers.
    mMutex.Enter();
    mCurrRevision++;
    mMutex.Leave();
}


void FdoSchemaManager::UpdateSchema( 
    FdoFeatureSchemaP pFeatSchema, 
    FdoPhysicalSchemaMappingP pOverrides,
    bool bIgnoreStates
)
{
	GetLogicalPhysicalSchemas();

	FdoSmLpSchemaP pLpSchema = mLpSchemas->FindItem( pFeatSchema->GetName() );

	if ( pLpSchema ) {
		pLpSchema->Update( 
            pFeatSchema, 
            bIgnoreStates ? FdoSchemaElementState_Modified : pFeatSchema->GetElementState(),
            pOverrides, 
            bIgnoreStates 
        );
	}
	else {
		throw FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_223),
				pFeatSchema->GetName()
			)
		);
	}
}

void FdoSchemaManager::SynchRevision()
{
	// These are different when a schema has been updated since the last
	// time this Schema Manager loaded its data.
	//
	// This is a bit conservative since the update might have been to 
	// a different database but schema updates won't be very frequent.
    mMutex.Enter();
	if ( mRevision != mCurrRevision ) 
		Clear();
    mMutex.Leave();

	mRevision = mCurrRevision;
}

void FdoSchemaManager::Clear(bool bClearAll)
{
	mLpSchemas = NULL;
	mPhysicalSchema->Clear();
    mSpatialContexts = NULL;

    if ( bClearAll ) {
        mMutex.Enter();
		mCurrRevision++;
        mMutex.Leave();
    }

}


FdoSmLpSchemasP FdoSchemaManager::GetLogicalPhysicalSchemas()
{
	// Reload LogicalPhysical schemas if Physical Schema has changed.
	SynchRevision();

	FdoSmPhMgrP physMgr = GetPhysicalSchema();

    if ( physMgr && (!mLpSchemas) ) {
		mLpSchemas = CreateLogicalPhysicalSchemas(physMgr, GetLpSpatialContexts());
        mLpSchemas->Load();
    }

	return mLpSchemas;
}

FdoSmLpSpatialContextsP FdoSchemaManager::GetLpSpatialContexts()
{
	// Reload LogicalPhysical spatial contexts if Physical Schema has changed.
	SynchRevision();

	FdoSmPhMgrP physMgr = GetPhysicalSchema();

    if ( physMgr && (!mSpatialContexts) ) {
		mSpatialContexts = CreateLpSpatialContexts(physMgr);
        mSpatialContexts->Load();
    }

	return mSpatialContexts;
}

FdoSmLpSpatialContextsP FdoSchemaManager::CreateLpSpatialContexts( FdoSmPhMgrP physMgr)
{
	return new FdoSmLpSpatialContextCollection(physMgr);
}
