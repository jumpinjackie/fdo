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
#include "FdoRdbmsSchemaUtil.h"
#include <malloc.h>
#include "LockUtility.h"
#include "FdoRdbmsLobUtility.h"
#include "FdoRdbmsUpdateCommand.h"
#include "FdoRdbmsInsertCommand.h"
#include "FdoRdbmsFilterProcessor.h"
#include "FdoRdbmsFilterUtil.h"
#include "Inc/Common/Io/ByteStreamReader.h"
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/PropertyMappingSingle.h>
#ifndef _WIN32
#define  wcsicmp wcscasecmp
#endif
#include "../../Gdbi/GdbiStatement.h"

static char* TRANSACTION_NAME = "FdoRdbmsUpdateCommand::Execute";

#define UPDATE_CLEANUP \
            if( innerSelect ) {\
                delete[] innerSelect; innerSelect = NULL;} \
            if( query != NULL ) \
            { \
                try { \
                query->Close(); \
                delete query; \
                query = NULL; \
                } catch (...){ query = NULL; } \
            } \
            if (bBeginTransaction == true) \
            { \
                bBeginTransaction = false; \
                mConnection->GetGdbiCommands()->tran_rolbk(); \
            }


#define UPDATE_CLEANUP2 \
        if ( statement != NULL ) \
        { \
            delete statement; \
            statement = NULL; \
        } \
        if ( duplicate != NULL ) \
        { \
            delete [] duplicate; \
            duplicate = NULL; \
        }


FdoRdbmsUpdateCommand::FdoRdbmsUpdateCommand ():
    mConnection( NULL ),
    mPropertyValues( NULL ),
    mCurrentClass(NULL),
	mPvcProcessor( NULL )
{
    mLockConflictReader = NULL;
}

FdoRdbmsUpdateCommand::FdoRdbmsUpdateCommand (FdoIConnection *connection):
    FdoRdbmsFeatureCommand<FdoIUpdate>(connection),
    mConnection( NULL ),
    mPropertyValues( NULL ),
    mCurrentClass(NULL)
{
    FdoRdbmsConnection *conn = static_cast<FdoRdbmsConnection*>(connection);
    if( conn )
        mConnection = conn->GetDbiConnection();
    mLockConflictReader = NULL;

	mPvcProcessor = new FdoRdbmsPvcProcessor( conn );
}

FdoRdbmsUpdateCommand::~FdoRdbmsUpdateCommand()
{
    if (mCurrentClass)
        delete [] mCurrentClass;

    FDO_SAFE_RELEASE(mPropertyValues);
    FDO_SAFE_RELEASE(mLockConflictReader);

	if( mPvcProcessor )
		delete mPvcProcessor;
}

FdoInt32 FdoRdbmsUpdateCommand::Execute ()
{
    FdoIdentifier*      className;
    GdbiQueryResult     *query = NULL;
    int                 numberOfRows = 0;
    bool                bBeginTransaction = false;
    bool                isFeatClass = false;
    char                buffer[16];
    bool                lockConflictsChecked = false;
    bool                throw_exception      = true;
    bool                containsObjectProperties = false;

    if( NULL == mConnection )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    className = this->GetClassNameRef();
    if (!className)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_35, "Class is null"));

    if (!mPropertyValues)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_36, "Property values collection is not set"));

    FdoPtr<FdoRdbmsConnection> conn = static_cast<FdoRdbmsConnection*>(GetConnection());
    if (conn->GetIsTransactionStarted() == false)
    {
        mConnection->GetGdbiCommands()->tran_begin(TRANSACTION_NAME);
        bBeginTransaction = true;
    }
    conn = NULL;

    const FdoSmLpClassDefinition *classDefinition = mConnection->GetSchemaUtil()->GetClass(className->GetText());

    // Make sure no unexpected properties are added to the property value collection
    mConnection->GetSchemaUtil()->SanitizePropertyValues( classDefinition, mPropertyValues, &containsObjectProperties );

    isFeatClass = (classDefinition->GetClassType() == FdoClassType_FeatureClass);
    FdoPtr<FdoRdbmsFilterProcessor>flterProcessor = FdoPtr<FdoRdbmsConnection>((FdoRdbmsConnection*)GetConnection())->GetFilterProcessor();

    // The following sets up an identifier list containing the feature id property,
    // if the class is a feature class.
    // For feature classes, the innerSelect is embedded in an IN clause that must
    // select the feature id.
    // For non-feature classes, selecting the identity properties is fine.
    FdoPtr<FdoIdentifierCollection>innerSelProps = FdoRdbmsFilterUtil::GetFeatIdSelList( classDefinition );
    FdoRdbmsFilterUtilConstrainDef  filterConstrain;
    filterConstrain.selectedProperties = innerSelProps;
    wchar_t *tmpSelect = (wchar_t*)flterProcessor->FilterToSql( this->GetFilterRef(), className->GetText(), innerSelProps ? SqlCommandType_Select : SqlCommandType_Update, FdoCommandType_Update, &filterConstrain );
    wchar_t *innerSelect = new wchar_t[wcslen(tmpSelect)+1];
    wcscpy( innerSelect, tmpSelect);

    if( classDefinition == NULL  )
        return numberOfRows;

    if (!CheckLocks(true, true))
    {
        UPDATE_CLEANUP;
        return numberOfRows;
    }
    lockConflictsChecked = true;

    const FdoSmLpGeometricPropertyDefinition *pGeometryProp = NULL;
    FdoPtr<FdoByteArray> geometry;

    if( isFeatClass )
    {
        const FdoSmLpFeatureClass*  feat = static_cast<const FdoSmLpFeatureClass *>( classDefinition );
        pGeometryProp = feat->RefGeometryProperty();
        if( pGeometryProp != NULL )
        {
            FdoPtr<FdoPropertyValue> propertyValue;
            int i;
            for (i=mPropertyValues->GetCount()-1; i >= 0; i--)
            {
                propertyValue = mPropertyValues->GetItem(i);
                FdoPtr<FdoIdentifier> name = propertyValue->GetName();
                if (wcscmp(name->GetName(), pGeometryProp->GetName()) == 0)
                    break;
            }
            if ( i >= 0 )
            {
                FdoPtr<FdoValueExpression> literalExpression = propertyValue->GetValue();
                if ( literalExpression != NULL )
                    geometry = ((FdoGeometryValue*)literalExpression.p)->GetGeometry();
            }
        }
    }

    try
    {

        query = mConnection->GetGdbiConnection()->ExecuteQuery( flterProcessor->FilterToSql( this->GetFilterRef(), className->GetText(), SqlCommandType_Update, FdoCommandType_Update ) );

        int  featId;
        bool addedIndentProperties = false;  // Used to indicate if we added any property to the user populated property value collection
        const wchar_t *prmPrtName = L"FeatId";
        wchar_t *prmColName = NULL;
        const FdoSmLpDataPropertyDefinition *featIdProp = NULL;
        if ( isFeatClass ) {
            featIdProp = classDefinition->RefFeatIdProperty();
            if( featIdProp != NULL ) {
                const FdoSmPhColumn* featIdCol = featIdProp->RefColumn();

                if ( featIdCol != NULL ) {
                    const wchar_t *colName = featIdCol->GetName();
                    prmColName = (wchar_t*)alloca ((wcslen(colName) + 1)*sizeof( wchar_t ) );
                    wcscpy(prmColName, colName);
                }
                else {
                    featIdProp = NULL;
                }
            }
        }

		
		// Initialize the long transaction manager if we have one
		const FdoRdbmsPvcProcessor* ltPvcProcessor = NULL;
		FdoPtr<FdoRdbmsLongTransactionManager> ltManager = mFdoConnection->GetLongTransactionManager();
		if( ltManager.p != NULL && classDefinition->GetCapabilities()->SupportsLongTransactions() )
			ltPvcProcessor = ltManager->GetPvcProcessor();


        const FdoSmLpDataPropertyDefinitionCollection *properties = classDefinition->RefIdentityProperties();

        for( int i =0; properties!= NULL && i<properties->GetCount(); i++ )
        {
            // following needs to change to handle numeric columns.
            if ( !properties->RefItem(i)->GetIsFeatId() )
            {
                const wchar_t* name = properties->RefItem(i)->GetName();

                FdoPtr<FdoPropertyValue> propVal = mPropertyValues->FindItem( name );

                if( propVal != NULL )
                {
                    FdoPtr<FdoDataValue> value = (FdoDataValue*)propVal->GetValue();
                    int length;

                    // For non object property class, we should not accept an initialized identity property
                    // as the user may think that updating identity properties is allowed.
                    // For collection object properties, the user has to provide the identity values.

                    if( value != NULL && className->GetScope( length ) == NULL )
                    {
                        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_389, "Identity properties are not updateable" ));
                    }
                }
            }

        }

        while( query->ReadNext() )
        {
            for( int i =0; properties!= NULL && i<properties->GetCount(); i++ )
            {
                if ( properties->RefItem(i)->GetIsFeatId() )
                {
                    if ( isFeatClass && featIdProp )
                    {
                        //
                        // Update the FeatId property value.
                        featId = (int)query->GetInt64(prmColName, NULL, NULL );
                        FdoPtr<FdoDataValue> featNum = FdoDataValue::Create(mConnection->GetUtility()->Utf8ToUnicode(ut_itoa( featId, buffer) ) );
                        FdoPtr<FdoPropertyValue> propVal = mPropertyValues->FindItem( prmPrtName );
                        if (propVal == NULL)
                        {
                            propVal = FdoPropertyValue::Create(  );
                            propVal->SetName( prmPrtName );
                            mPropertyValues->Add( propVal );
                            addedIndentProperties = true;
                        }
                        propVal->SetValue( featNum );
                    }
                }
                else
                {
                    // following needs to change to handle numeric columns.
                    FdoPtr<FdoPropertyValue> propVal;
                    const wchar_t* name = properties->RefItem(i)->GetName();
                    propVal = mPropertyValues->FindItem( name );
                    if( propVal == NULL )
                    {
                        propVal = FdoPropertyValue::Create();
                        propVal->SetName( name );
                        mPropertyValues->Add( propVal );
                        addedIndentProperties = true;
                    }
                    const wchar_t* strVal = query->GetString(  properties->RefItem(i)->GetColumnName(), NULL, NULL );
                    FdoPtr<FdoDataValue> propData = FdoDataValue::Create( strVal );
                    propVal->SetValue( propData );
                }
            }
            //
            // Update the properties associated with this feature
            FdoRdbmsCollection<FdoRdbmsPvcOperation>* classes = mPvcProcessor->RefactorPvc(mPropertyValues, classDefinition, true);
            for (int i=0; i<classes->GetCount(); i++)
            {
                FdoPtr<FdoRdbmsPvcOperation>clas = classes->GetItem(i);
                if( clas->IsLinked() )
                    continue;  // Handled by the original property list

				FdoPtr<FdoPropertyValueCollection>properties = clas->GetProperties();

				if( ltPvcProcessor != NULL )
				{
					FdoRdbmsCollection<FdoRdbmsPvcOperation> *ltOps = ltPvcProcessor->RefactorPvc(properties, clas->GetClass(), true );
					for (int j=0; j<ltOps->GetCount(); j++)
					{
						FdoPtr<FdoRdbmsPvcOperation>ltPvcOp = ltOps->GetItem(j);
						FdoRdbmsPvcHandler	*ltPvcHandler = (FdoRdbmsPvcHandler	*)ltPvcOp->GetPvcHandler();
						if( ltPvcHandler )
						{
							FdoPtr<FdoRdbmsLongTransactionInfo>ltInfo; 
							// Note the RevisionNumber column update flag.
							// The value is increased in the new version row
							// or, if the active version row is updated.
							// There is no update of RevisionNumber if the NextLtId 
							// column is updated.
							numberOfRows += ltPvcHandler->Execute( ltPvcOp->GetClass(), FdoPtr<FdoPropertyValueCollection>(ltPvcOp->GetProperties()), 
																   /* flag to update Revision Number column */(j==0)? true:false, false);
							ltManager->GetActive( &ltInfo );
							if( ltInfo && ltInfo->GetLtId() != 0 /* root LT */ )
							{
								const FdoSmLpDbObject* table = ltPvcOp->GetClass()->RefDbObject();

								ltManager->AddLtInTableInfo( mConnection->GetSchema(), table->GetName() );
							}
						}
					}
					ltOps->Release();
				}
				else
				{
					FdoRdbmsPvcHandler	*pvcHandler = (FdoRdbmsPvcHandler	*)clas->GetPvcHandler();
					if( pvcHandler )
						numberOfRows += pvcHandler->Execute( clas->GetClass(), properties, /* RevisionNumber update flag */ true, false );
				}
            }
            classes->Release();
        }

        query->Close();
        delete query;
        query = NULL;
        if (bBeginTransaction == true)
        {
            bBeginTransaction = false;
            mConnection->GetGdbiCommands()->tran_end(TRANSACTION_NAME);
        }
        if( innerSelect )
        {
            delete[] innerSelect;
            innerSelect = NULL;
        }

        // Remove the identity properties we added internally
        for( int i =0; addedIndentProperties && properties!= NULL && i<properties->GetCount(); i++ )
        {
            FdoPtr<FdoPropertyValue>propVal;
            const wchar_t* name = properties->RefItem(i)->GetName();
            propVal = mPropertyValues->FindItem( name );
            if( propVal != NULL )
                mPropertyValues->Remove( propVal );
        }
    }
    catch (FdoCommandException *)
    {
        UPDATE_CLEANUP;
        throw;
    }
    catch (FdoException *ex)
    {
        UPDATE_CLEANUP;
        FdoCommandException *exp = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
        ex->Release();
        throw exp;
    }
    catch( ... )
    {
        UPDATE_CLEANUP;
        throw;
    }

    return numberOfRows;
}

// The function returns the handle to the lock conflicts. By doing this the user
// takes over ownership of the object and the internal pointer to the lock con-
// flict reader is removed. Hence, any subsequent request for the lock conflict
// reader will return a NULL pointer unless a new delete operation is executed.
FdoILockConflictReader* FdoRdbmsUpdateCommand::GetLockConflicts()
{
    FdoILockConflictReader *temp_lock_conflict_reader = mLockConflictReader;
    mLockConflictReader = NULL;
    return temp_lock_conflict_reader;
}

FdoPropertyValueCollection* FdoRdbmsUpdateCommand::GetPropertyValues()
{
    FdoIdentifier*      className;

    if( NULL == mConnection )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    className = this->GetClassNameRef();
    if (!className)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_35, "Class is null"));

    if (mCurrentClass == NULL ||
        wcscmp(mCurrentClass, className->GetText()) != 0)
    {
        FDO_SAFE_RELEASE(mPropertyValues);
        if (mCurrentClass)
            delete [] mCurrentClass;

        mPropertyValues = FdoPropertyValueCollection::Create();
        mCurrentClass = new wchar_t[wcslen(className->GetText())+1];
		wcscpy(mCurrentClass, className->GetText());
    }


    mPropertyValues->AddRef();

    return mPropertyValues;
}

// Depending on the given parameters the function applies a transaction lock on
// the qualified objects and/or checks for version and lock conflicts.
bool FdoRdbmsUpdateCommand::CheckLocks(bool placeTransactionLock, bool checkForConflicts)
{

    bool               lockConflicts = false;
    bool               executionStatus = FALSE;

    // If there is already a lock conflict reader, the reader is a remainder
    // of a previous operation and can therefore be removed.
    FDO_SAFE_RELEASE(mLockConflictReader);

    // Handle all lock related issues. This includes:
    //  - Applying a transaction lock on all qualified objects.
    //  - Checking for lock conflicts .
    //  - Preparing the lock conflict reader.
    mLockConflictReader = LockUtility::HandleLocks (mFdoConnection,
                                                    this->GetClassNameRef(),
                                                    this->GetFilterRef(),
                                                    placeTransactionLock,
                                                    &lockConflicts,
                                                    &executionStatus);

    if (!executionStatus)
        throw FdoCommandException::Create(
                NlsMsgGet(FDORDBMS_260,
                            "Unable to get exclusive access to one or more features"));

    // Return TRUE if the check did not discover lock conflicts, FALSE
    // otherwise.

    return (lockConflicts == false);
}

// The function returns an empty lock conflict reader.
void FdoRdbmsUpdateCommand::GetDefaultLockConflictReader()
{

    bool               executionStatus = FALSE;

    // If there is already a lock conflict reader, the reader is a remainder
    // of a previous operation and can therefore be removed.
    FDO_SAFE_RELEASE(mLockConflictReader);

    // Get the default lock conflict reader.
    mLockConflictReader =
        LockUtility::GetDefaultLockConflictReader (mFdoConnection,
                                                    this->GetClassNameRef(),
                                                    &executionStatus);

}

void FdoRdbmsUpdateCommand::SetFeatureClassName(FdoIdentifier* value)
{
    if (value != NULL)
        mConnection->GetSchemaUtil()->CheckClass(value->GetText());
    FdoRdbmsFeatureCommand<FdoIUpdate>::SetFeatureClassName(value);
}

void FdoRdbmsUpdateCommand::SetFeatureClassName(const wchar_t* value)
{
    mConnection->GetSchemaUtil()->CheckClass(value);
    FdoRdbmsFeatureCommand<FdoIUpdate>::SetFeatureClassName(value);
}
