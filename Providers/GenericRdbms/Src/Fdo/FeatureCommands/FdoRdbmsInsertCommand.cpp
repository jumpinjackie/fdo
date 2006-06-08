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
 */

#include "stdafx.h"
#include "FdoRdbmsSchemaUtil.h"
#include <malloc.h>

// FdoRdbmsInsertCommand
#include "FdoRdbmsException.h"
#include "FdoRdbmsInsertCommand.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsLobUtility.h"
#include "Inc/Common/Io/ByteStreamReader.h"
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/PropertyMappingSingle.h>
#include "FdoRdbmsFeatureInfoReader.h"
#include "Inc/ut.h"
#include "SpatialManager/FdoRdbmsSpatialManager.h"


static char* TRANSACTION_NAME = "FdoRdbmsInsertCommand::Execute";


#define INSERT_CLEANUP \
    try { \
        if (bBeginTransaction == true) \
        { \
            bBeginTransaction = false; \
            mConnection->GetGdbiCommands()->tran_rolbk(); \
        } \
    } catch ( ... ) { } // Do not mask the original exception


FdoRdbmsInsertCommand::FdoRdbmsInsertCommand () :
    mConnection( NULL ),
    mPropertyValues(NULL),
    mAutoGenPropertyValues(NULL),
    mBatchValues(NULL),
    m_ClassName( NULL ),
    mCurrentClass(NULL),
	mPvcProcessor( NULL )
{
 
}

FdoRdbmsInsertCommand::FdoRdbmsInsertCommand (FdoIConnection *connection) :
    mPropertyValues(NULL),
    mAutoGenPropertyValues(NULL),
    m_ClassName( NULL ),
    mBatchValues(NULL),
    FdoRdbmsCommand<FdoIInsert>(connection),
    mCurrentClass(NULL),
	mPvcProcessor( NULL )
{
    FdoRdbmsConnection *conn = static_cast<FdoRdbmsConnection*>(connection);
    if( conn )
        mConnection = conn->GetDbiConnection();

	mPvcProcessor = new FdoRdbmsPvcProcessor( conn );
}

FdoRdbmsInsertCommand::~FdoRdbmsInsertCommand()
{
	if( mPvcProcessor )
		delete mPvcProcessor;

    if (mCurrentClass)
        delete [] mCurrentClass;
    FDO_SAFE_RELEASE(mPropertyValues);
    FDO_SAFE_RELEASE(mAutoGenPropertyValues);
    FDO_SAFE_RELEASE(mBatchValues);
    FDO_SAFE_RELEASE(m_ClassName);
}

FdoIFeatureReader* FdoRdbmsInsertCommand::Execute ()
{
    gdbi_feat_info_def   feat_info;
    int                 insert_position = -1; // end of feature
    int                 gid = -1;
    FdoIdentifier*      className;
    int                 feat_open = FALSE;
    int                 feat_created = FALSE;
    FdoPtr<FdoPropertyValueCollection>featInfoCol = FdoPropertyValueCollection::Create(); // Used to create the FeatureIndoReader
    bool                bBeginTransaction = false;
    const FdoSmLpClassDefinition *classDefinition = NULL;
    bool                containsObjectProperties = false;
    bool                isFeatIdAutoincremented = false;
    bool                handleForeignAutoincrementedId = false;

    if( NULL == mConnection )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    className = this->GetClassNameRef();
    if (!className)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_35, "Class is null"));

    if ( !mPropertyValues )
        FdoPtr<FdoPropertyValueCollection> propValues = GetPropertyValues(); // Prime the propertyValues and autoGeneratedPropertyValues members

    try
    {
        bool addedIndentProperties = false;  // Used to indicate if we added any property to the user populated property value collection

        classDefinition = mConnection->GetSchemaUtil()->GetClass(className->GetText());

        // Make sure no unexpected properties are added to the property value collection
        mConnection->GetSchemaUtil()->SanitizePropertyValues( classDefinition, mPropertyValues, &containsObjectProperties );

        if (mFdoConnection->GetIsTransactionStarted() == false)
        {
            mConnection->GetGdbiCommands()->tran_begin(TRANSACTION_NAME);
            bBeginTransaction = true;
        }

        if( classDefinition != NULL && classDefinition->GetClassType() == FdoClassType_FeatureClass )
        {
            // check Feat Id property
            const FdoSmLpDataPropertyDefinition *lpFeatIdProp = classDefinition->RefFeatIdProperty();
            if( lpFeatIdProp != NULL ) {

                const wchar_t *featId = lpFeatIdProp->GetName();
                if ( (featId == NULL) || (wcslen(featId) ==0) )
                    throw FdoCommandException::Create( NlsMsgGet( FDORDBMS_15, "Feature ID does not exist") );

                FdoPtr<FdoPropertyValue>FeatIdProp =  mPropertyValues->FindItem( featId );
                if (FeatIdProp == NULL)
                {
                    FdoStringP sequenceName = GetSequenceName(featId, L"", classDefinition);
                    feat_info.feat_num = 0L;
                    feat_info.classid = (long)classDefinition->GetId();
                    feat_info.changeseq = 0L;

			        // If Featid not autoincremented, then generate a number for it.
			        isFeatIdAutoincremented = IsFeatIdAutoincremented( classDefinition );

			        if( !isFeatIdAutoincremented )
				        feat_info.feat_num = mConnection->GetGdbiCommands()->NextSequenceNumber( (char*)(const char *)sequenceName);
        			
                    const FdoSmLpFeatureClass*  feat = static_cast<const FdoSmLpFeatureClass *>( classDefinition );

                    feat_created = TRUE;

                    FeatIdProp = FdoPropertyValue::Create(  );
                    FeatIdProp->SetName( featId );

                    mPropertyValues->Add( FeatIdProp );
                    
                    addedIndentProperties = true;

                    FdoPtr<FdoPropertyValue>classId = FdoPropertyValue::Create();
                    FdoPtr<FdoDataValue>clidValue = FdoDataValue::Create((FdoInt64) feat_info.classid );
                    classId->SetValue( clidValue );
                    classId->SetName( L"ClassId" );
                    mPropertyValues->Add( classId );

                    FdoPtr<FdoPropertyValue>revNum = FdoPropertyValue::Create();
                    FdoPtr<FdoDataValue>revValue = FdoDataValue::Create((FdoInt64) 0 );
                    revNum->SetValue( revValue );
                    revNum->SetName( L"RevisionNumber" );
                    mPropertyValues->Add( revNum );

			        FdoPtr<FdoPropertyValue>featIdProp = FdoPropertyValue::Create();
                    FdoPtr<FdoIdentifier>featIdIdentifier = FeatIdProp->GetName();
                    featIdProp->SetName( featIdIdentifier );
			        featInfoCol->Add(featIdProp);

			        FdoPtr<FdoPropertyValue>chgSeqProp = FdoPropertyValue::Create();
                    FdoPtr<FdoDataValue>chgSeqValue = FdoDataValue::Create((FdoInt64) feat_info.changeseq );
                    chgSeqProp->SetValue( chgSeqValue );
                    chgSeqProp->SetName( L"RevisionNumber" );
                    featInfoCol->Add(chgSeqProp);

                    //
                    // Set the featID value in the Property value collections
                    if( FeatIdProp.p != NULL && !isFeatIdAutoincremented)
                    {
                        FdoPtr<FdoDataValue>featNum = FdoDataValue::Create((FdoInt64) feat_info.feat_num );
                        FeatIdProp->SetValue( featNum );
				        featIdProp->SetValue( featNum );
                    }
                }
            }
        }

        if( featInfoCol->GetCount() == 0 && classDefinition != NULL)
        {
            // The above processing did not find any information to return.  This
            // would be because either the class isn't a feature class, or there 
            // wasn't anything labelled as a "FeatId".  The FeatId is an implementation
            // detail which is being phased out -- it's currently only true if there is
            // a metaschema.
            //
            // Let's also check to see if there is a numeric, autogenerated primary key.
            // This would be the case for foreign schema (no FDO metaschema), or for
            // RDBMS providers (like ODBC) which do not use FDO metaschema.
            //
            // At this level, we could look for multiple autogenerated keys, but there
            // is no way to get their values in the supporting code.

            const FdoSmLpDataPropertyDefinitionCollection * idPropDefs = classDefinition->RefIdentityProperties();
            FdoInt32 numPropDefs = idPropDefs->GetCount();
            bool found=false;
            for (FdoInt32 i=0;  !found && i < numPropDefs;  i++)
            {
                const FdoSmLpDataPropertyDefinition * idPropDef = idPropDefs->RefItem(i);
                if (idPropDef->GetIsAutoGenerated())
                {
			        FdoPtr<FdoPropertyValue>autoIdProp = FdoPropertyValue::Create();
                    FdoString * idPropName = idPropDef->GetName();
                    autoIdProp->SetName( idPropName );
			        featInfoCol->Add(autoIdProp);
                    found = true;
                    handleForeignAutoincrementedId = true;
                }
            }
        }

        // Set values for any autogenerated properties.
        SetAutoGeneratedValues();

        // Get the full list of properties (merge of autogenerated and non-autogenerated properties).
        FdoPtr<FdoPropertyValueCollection> allPropertyValues = GetAllPropertyValues();

		const FdoRdbmsPvcProcessor* ltPvcProcessor = NULL;
		FdoPtr<FdoRdbmsLongTransactionManager> ltManager = mFdoConnection->GetLongTransactionManager();
		if( ltManager.p != NULL && classDefinition->GetCapabilities()->SupportsLongTransactions() )
			ltPvcProcessor = ltManager->GetPvcProcessor();
        //
        // Insert the properties associated with this feature
        FdoPtr<FdoRdbmsCollection<FdoRdbmsPvcOperation> >classes = mPvcProcessor->RefactorPvc(allPropertyValues, classDefinition, false );

        for (int i=0; i<classes->GetCount(); i++)
        {
            FdoPtr<FdoRdbmsPvcOperation>oneClass = classes->GetItem(i);
            if( oneClass->IsLinked() )
            {
                continue;  // Handled by the original property list
            }
            FdoPtr<FdoPropertyValueCollection>properties = oneClass->GetProperties();
            try
            {
				// Need to initialize the object properties id property.
				// In the case the feature id is not autoincremented, the feat id would have been initialized by RefactorPvc
				if ( isFeatIdAutoincremented && i != 0 )
					InitObjectPropertyAutoGenProp(oneClass->GetClass(), properties, featInfoCol  );
				
                if( ltPvcProcessor == NULL )
				{
					FdoRdbmsPvcHandler	*pvcHandler = (FdoRdbmsPvcHandler	*)oneClass->GetPvcHandler();
					if( pvcHandler )
						pvcHandler->Execute( oneClass->GetClass(), properties, false, handleForeignAutoincrementedId );
				}
				else 
				{
					FdoPtr<FdoRdbmsCollection<FdoRdbmsPvcOperation> >ltOps = ltPvcProcessor->RefactorPvc(properties, oneClass->GetClass(), false );
					for (int j=0; j<ltOps->GetCount(); j++)
					{
						FdoPtr<FdoRdbmsPvcOperation>ltPvcOp = ltOps->GetItem(j);
						FdoPtr<FdoPropertyValueCollection>ltProperties = ltPvcOp->GetProperties();
						// for the second and subsequent pvcs, set the id value for the autogenerated property. The id was generated by the first pvc
						if( isFeatIdAutoincremented && j!= 0 )
							InitObjectPropertyAutoGenProp(ltPvcOp->GetClass(), ltProperties, featInfoCol  );
						
						FdoRdbmsPvcHandler	*ltPvcHandler = (FdoRdbmsPvcHandler	*)ltPvcOp->GetPvcHandler();
						if( ltPvcHandler )
						{
							FdoPtr<FdoRdbmsLongTransactionInfo>ltInfo;
							ltPvcHandler->Execute( ltPvcOp->GetClass(), ltProperties, false, false );
							if( featInfoCol->GetCount() != 0 && !handleForeignAutoincrementedId )
							{
								FdoPtr<FdoValueExpression>exp = FdoPtr<FdoPropertyValue>(featInfoCol->GetItem(0))->GetValue();
								if( exp.p == NULL )
									FetchAutoincrementedFeatIdValues( ltPvcOp->GetClass(), featInfoCol );
							}
							ltManager->GetActive( &ltInfo );
							if( ltInfo && ltInfo->GetLtId() != 0 /* root LT */ )
							{
								const FdoSmLpDbObject* table = ltPvcOp->GetClass()->RefDbObject();

								ltManager->AddLtInTableInfo( mConnection->GetSchema(), table->GetName() );
							}
						}
					}
				}
				
               // Get the autoincremented Id values if not already set
				if( featInfoCol->GetCount() != 0 )
				{
					FdoPtr<FdoValueExpression>exp = FdoPtr<FdoPropertyValue>(featInfoCol->GetItem(0))->GetValue();
					if( exp.p == NULL )
                        if (handleForeignAutoincrementedId)
    						FetchAutoincrementedIdValues( oneClass->GetClass(), featInfoCol );
                        else
    						FetchAutoincrementedFeatIdValues( oneClass->GetClass(), featInfoCol );
				}
            }
            catch( FdoRdbmsException *exp )
            {
                // Sometimes we get an RDBMS error about missing not null values. This is a result of the
                // user not providing the not null property values. We check here as opposed to the beginning of
                // the function to optimize the success path.
                FdoRdbmsException  *newExp = CheckForNotNullProperties( oneClass->GetClass(), properties);
                if( newExp != NULL )
                    exp->Release();
                else
                    newExp = exp;

                throw newExp;
            }
        }

        if (bBeginTransaction == true)
        {
            bBeginTransaction = false;
            mConnection->GetGdbiCommands()->tran_end(TRANSACTION_NAME);
        }
        // Remove the identity properties we added internally
        if( addedIndentProperties )
        {
            const FdoSmLpDataPropertyDefinition *lpFeatIdProp = classDefinition->RefFeatIdProperty();
            FdoPtr<FdoPropertyValue>propVal = mPropertyValues->FindItem( lpFeatIdProp->GetName() );
            if( propVal != NULL )
                mPropertyValues->Remove( propVal );
            propVal = mPropertyValues->FindItem( L"RevisionNumber" );
            if( propVal != NULL )
                mPropertyValues->Remove( propVal );
            propVal = mPropertyValues->FindItem( L"ClassId" );
            if( propVal != NULL )
                mPropertyValues->Remove( propVal );
        }
    }

    catch (FdoCommandException *ex)
    {
        ex;
        INSERT_CLEANUP;
        throw;
    }
    catch (FdoException *ex)
    {
        INSERT_CLEANUP;
        FdoCommandException *exp = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
        ex->Release();
        throw exp;

    }
    catch( ... )
    {
        INSERT_CLEANUP;
        throw;

    }

    FdoRdbmsFeatureInfoReader *reader = NULL;
    if( classDefinition != NULL &&  
        classDefinition->GetClassType() == FdoClassType_FeatureClass &&
        featInfoCol->GetCount() > 0 
    )
        reader = new FdoRdbmsFeatureInfoReader( featInfoCol, classDefinition );
    return reader;
}

void FdoRdbmsInsertCommand::InitObjectPropertyAutoGenProp(const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection  *propValCollection, FdoPropertyValueCollection  *featInfoCol)
{
	const FdoSmLpPropertyDefinitionCollection *smLpProps = classDefinition->RefProperties();
							
	for(int i=0; i< smLpProps->GetCount(); i++ )
	{
		const FdoSmLpPropertyDefinition* smLpProp = smLpProps->RefItem(i);
		if( smLpProp->GetPropertyType() != FdoPropertyType_DataProperty )
			continue;

		const FdoSmLpDataPropertyDefinition* smLpDataProp = (const FdoSmLpDataPropertyDefinition*)smLpProp;

		// Make sure this property is copied from an autogenerated property
		const FdoSmLpDataPropertyDefinition* tmpProp = smLpDataProp;
		while( tmpProp->RefPrevProperty() ) tmpProp = (const FdoSmLpDataPropertyDefinition*)tmpProp->RefPrevProperty();
		if( tmpProp->GetIsAutoGenerated() )
		{
			FdoPtr<FdoPropertyValue>prop = propValCollection->FindItem( smLpDataProp->GetName() );
			if( prop != NULL )
				prop->SetValue( FdoPtr<FdoValueExpression>(FdoPtr<FdoPropertyValue>(featInfoCol->GetItem(0))->GetValue()) );
			else
			{
				FdoPtr<FdoPropertyValue>propertyValue = FdoPropertyValue::Create();
				propertyValue->SetName( smLpDataProp->GetName() );
				propertyValue->SetValue( FdoPtr<FdoValueExpression>(FdoPtr<FdoPropertyValue>(featInfoCol->GetItem(0))->GetValue()) );
				propValCollection->Add( propertyValue );
			}
		}
	}
}
FdoRdbmsException* FdoRdbmsInsertCommand::CheckForNotNullProperties( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection  *propValCollection )
{
    const FdoSmLpPropertyDefinitionCollection *properties = classDefinition->RefProperties();
    for(int i=0; i<properties->GetCount(); i++ )
    {
        const FdoSmLpPropertyDefinition *prop = properties->RefItem( i );
        if( prop && prop->GetPropertyType() == FdoPropertyType_DataProperty )
        {
            const FdoSmLpDataPropertyDefinition *dataProp = (FdoSmLpDataPropertyDefinition *)prop;
            if( ! dataProp->GetNullable() && ! dataProp->GetIsSystem() && ! dataProp->GetIsAutoGenerated() )
            {
               FdoPtr<FdoPropertyValue>propVal = propValCollection->FindItem( dataProp->GetName() );
               if( propVal != NULL )
               {
                   FdoPtr<FdoDataValue>val = (FdoDataValue*)propVal->GetValue();
                   if( ( val != NULL && ! val->IsNull() ) ||
                       ( dataProp->GetDataType() == FdoDataType_BLOB && propVal->GetStreamReader() ) )
                       continue;
               }

               return FdoRdbmsException::Create( NlsMsgGet1( FDORDBMS_392, "A value is required for the not nullable property %1$ls", dataProp->GetName() ) );
            }
        }
    }

    return NULL;
}


void FdoRdbmsInsertCommand::SetAutoGeneratedValues()
{
    FdoIdentifier* className;
    FdoPtr<FdoIdentifier> propName;
    const FdoSmLpClassDefinition *classDefinition = NULL;
    const FdoSmLpClassDefinition* currentClass = NULL;
    const FdoSmLpPropertyDefinitionCollection *propDefs;
    FdoPtr<FdoPropertyValueCollection>  propValues = GetAutoGeneratedPropertyValues();
    FdoInt32                            i;
    long                 nextSeq;
    FdoStringP sequenceName;


    className = this->GetClassNameRef();
    //currentClass = GetClass(className);

    classDefinition = mConnection->GetSchemaUtil()->GetClass(className->GetText());
    propDefs = classDefinition->RefProperties();

    // Autogenerate a value for each autogenerated property.
    for ( i = 0; i < propValues->GetCount(); i++ )
    {

        FdoPtr<FdoPropertyValue>    propValue = propValues->GetItem(i);
        propName = propValue->GetName();


        sequenceName = GetSequenceName(propName->GetText(), L"", classDefinition);
        if (sequenceName == L"")
            nextSeq = mConnection->GetGdbiCommands()->NextSequenceNumber(DBI_FEATURE_SEQUENCE);
        else
            nextSeq = mConnection->GetGdbiCommands()->NextSequenceNumber((char *)(const char*) sequenceName);

        FdoPtr<FdoInt64Value> autoGenValue = FdoInt64Value::Create( (FdoInt64) nextSeq );
        propValue->SetValue( autoGenValue );
    }
}

FdoStringP FdoRdbmsInsertCommand::GetSequenceName(const wchar_t *propNameStr, const wchar_t* scope, const FdoSmLpClassDefinition *classDefinition)
{
    const FdoSmLpPropertyDefinitionCollection *propDefs;
    FdoInt32                            i;
    int                                   found = false;

    propDefs = classDefinition->RefProperties();
    FdoStringP sequenceName(L"");

    for (i = 0; i < propDefs->GetCount() && found == false; i++ )
    {
        const FdoSmLpPropertyDefinition *propDef = propDefs->RefItem(i);
        if (propDef == NULL)
            continue;

        switch(propDef->GetPropertyType())
        {
            case FdoPropertyType_DataProperty:
            {

                const FdoSmLpDataPropertyDefinition* dataProp = dynamic_cast<const FdoSmLpDataPropertyDefinition*>(propDef);
                wchar_t *tmpStr = mConnection->GetSchemaUtil()->makePropName( scope, dataProp->GetName() );

                if (wcscmp( propNameStr, tmpStr) == 0)
                {   sequenceName = dataProp->GetSequenceName();
                    found = true;
                }
            }
            break;

            case FdoPropertyType_ObjectProperty:
            {
                const FdoSmLpObjectPropertyDefinition *objPropDef = static_cast<const FdoSmLpObjectPropertyDefinition*>(propDef);
                if( objPropDef->GetObjectType() == FdoObjectType_Value )
                {
                    wchar_t *tmpStr = mConnection->GetSchemaUtil()->makePropName( scope, objPropDef->GetName());

                    wchar_t* newScope = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen(tmpStr) + 1));
                    wcscpy( newScope, tmpStr );
                    sequenceName = GetSequenceName( propNameStr, newScope, objPropDef->RefTargetClass());
                    if (sequenceName != NULL)
                        found = true;
                }
            }
                break;

            default:
                break;
        }
    }

     return sequenceName;
}

FdoPropertyValueCollection* FdoRdbmsInsertCommand::GetPropertyValues()
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
        FDO_SAFE_RELEASE(mAutoGenPropertyValues);
        if (mCurrentClass)
            delete [] mCurrentClass;
        mPropertyValues = FdoPropertyValueCollection::Create();
        mAutoGenPropertyValues = mConnection->GetSchemaUtil()->GetPropertyValues( className->GetText(), true );
        mCurrentClass = new wchar_t[wcslen(className->GetText())+1];
        wcscpy(mCurrentClass, className->GetText());
    }

    mPropertyValues->AddRef();

    return mPropertyValues;
}

FdoPropertyValueCollection* FdoRdbmsInsertCommand::GetAutoGeneratedPropertyValues()
{
    // Prime the propertyValues and autoGeneratedPropertyValues members
    mAutoGenPropertyValues->AddRef();

    return mAutoGenPropertyValues;
}

bool FdoRdbmsInsertCommand::IsFeatIdAutoincremented( const FdoSmLpClassDefinition *classDefinition )
{
	bool featIdAutoincremented = false;

    const FdoSmLpPropertyDefinition *featIdProp = classDefinition->RefFeatIdProperty();

	if ( featIdProp ) 
	{
		const FdoSmLpSimplePropertyDefinition* simpleProp =
			static_cast<const FdoSmLpSimplePropertyDefinition*>(featIdProp);

		const FdoSmPhColumn *column = simpleProp->RefColumn();
		featIdAutoincremented = (column && column->GetAutoincrement() );
	}
	return featIdAutoincremented;
}

void FdoRdbmsInsertCommand::FetchAutoincrementedFeatIdValues( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection* featInfoCol )
{
	if ( !IsFeatIdAutoincremented( classDefinition ) )
		return;

	FdoPtr<FdoPropertyValue>featIdInfoProp = featInfoCol->GetItem(0);

	FdoInt64  newNumber = mConnection->GetGdbiCommands()->NextSequenceNumber( DBI_FEATURE_SEQUENCE );
 
	FdoPtr<FdoDataValue>featIdValue = FdoDataValue::Create( newNumber );
	featIdInfoProp->SetValue( featIdValue );
}

bool FdoRdbmsInsertCommand::IsPropertyValueAutoincremented( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValue * propertyValue )
{
	bool isAutoincremented = false;

    FdoPtr<FdoIdentifier>identifier = propertyValue->GetName();
    const FdoSmPhColumn *columnDef = NULL;
    const FdoSmLpPropertyDefinitionCollection * propertyDefs = classDefinition->RefProperties();
    const FdoSmLpPropertyDefinition * propertyDef = propertyDefs->RefItem(identifier->GetName());
	const FdoSmLpSimplePropertyDefinition* simplePropDef =
		static_cast<const FdoSmLpSimplePropertyDefinition*>(propertyDef);
     if ( simplePropDef != NULL )
        columnDef = simplePropDef->RefColumn();
     if (columnDef != NULL && columnDef->GetAutoincrement())
         isAutoincremented = true;

	return isAutoincremented;
}

void FdoRdbmsInsertCommand::FetchAutoincrementedIdValues( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection* featInfoCol )
{
    // Only a single autogenerated identity column is supported.
	FdoPtr<FdoPropertyValue>infoProp = featInfoCol->GetItem(0);
  
    if ( !IsPropertyValueAutoincremented( classDefinition, infoProp ) )
        return;

	FdoInt64  newNumber = mConnection->GetGdbiCommands()->NextSequenceNumber( DBI_FEATURE_SEQUENCE );
 
	FdoPtr<FdoDataValue>idValue = FdoDataValue::Create( newNumber );
	infoProp->SetValue( idValue );
}

FdoPropertyValueCollection* FdoRdbmsInsertCommand::GetAllPropertyValues()
{
    FdoPropertyValueCollection* allValues = FdoPropertyValueCollection::Create();
    FdoInt32 i,j;

    // Merge the autogenerate and non-autogenerated property values into one list
    FdoPtr<FdoPropertyValueCollection> propValues = GetPropertyValues();
    FdoPtr<FdoPropertyValueCollection> autoGenPropValues = GetAutoGeneratedPropertyValues();

    // Add the non-readonly properties
    for ( i = 0; i < propValues->GetCount(); i++ )
    {
        FdoPtr<FdoPropertyValue> propValue = propValues->GetItem(i);
        allValues->Add( propValue );
    }

    // Add the autogenrated properties
    for ( i = 0; i < autoGenPropValues->GetCount(); i++ )
    {
        FdoPtr<FdoPropertyValue>    autoGenPropValue = autoGenPropValues->GetItem(i);
        FdoPtr<FdoIdentifier>       autoGenPropName  = autoGenPropValue->GetName();
        bool                        found = false;

        // The writable and autogenerated lists should be mutually exclusive
        // but do the following anyway just in case there is a property
        // in both lists.
        for ( j = 0; j < propValues->GetCount(); j++ )
        {
            FdoPtr<FdoPropertyValue>    propValue = propValues->GetItem(j);
            FdoPtr<FdoIdentifier>       propName = propValue->GetName();

            if ( wcscmp(propName->GetText(), autoGenPropName->GetText()) == 0 )
            {
                found = true;
                break;
            }
        }

        // Add the read-only autogenerated property values.
        if ( !found )
            allValues->Add( autoGenPropValue );
    }

    return allValues;
}
