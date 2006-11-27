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
#include <Parse/Parse.h>
#include <Sm/Lp/SchemaCollection.h>
#include <Sm/Lp/FeatureClass.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include <Sm/Error.h>

#ifndef ASSERT
#ifdef _DEBUG
    #include <CrtDbg.h>
    #define ASSERT  _ASSERT
#else
    #define ASSERT(x) {}
#endif
#endif

FdoSmLpSchemaCollection::FdoSmLpSchemaCollection(FdoSmPhMgrP physicalSchema, FdoSmLpSpatialContextsP spatialContexts) :
	mPhysicalSchema(physicalSchema),
    mSpatialContexts(spatialContexts),
    mSchemasLoaded(false)
{
}

FdoSmLpSchemaCollection::~FdoSmLpSchemaCollection(void)
{
}
/*
FdoSmLpSchemaP CreateSchema( FdoSmPhSchemaReaderP rdr )
{
    return new (rdr, mPhysicalSchema, FDO_SAFE_ADDREF(this));
}
*/
const FdoSmLpClassDefinition* FdoSmLpSchemaCollection::FindClass( FdoStringP schemaName, FdoStringP className ) const
{
	const FdoSmLpSchema* pSchema = NULL;
	const FdoSmLpClassDefinition* pFoundClass = NULL;

	if ( schemaName.GetLength() > 0 ) {
		// Check the given schema if schema name specified
		pSchema = RefItem(schemaName);
		if ( pSchema ) 
            pFoundClass = pSchema->RefClass(className);
	}
	else {
		// Otherwise, find the first class in any schema.
		const FdoSmLpClassDefinition* pClass = NULL;

		for ( int i = 0; i < GetCount(); i++ ) {
			pSchema = RefItem(i);

			if ( pSchema ) {
				pClass = pSchema->RefClasses()->RefItem(className);

				if ( pClass ) {

					if ( pFoundClass ) {
						// Found a second class so exception.

                        throw FdoSchemaException::Create( 
                            FdoSmError::NLSGetMessage(
                                FDO_NLSID(FDOSM_174), 
									(FdoString*) className, 
									pFoundClass->RefLogicalPhysicalSchema()->GetName(),
									pSchema->GetName()
                            )
                        );
					}

					pFoundClass = pClass;
				}
			}
		}
	}

	return(pFoundClass);
}

const FdoSmLpClassDefinition* FdoSmLpSchemaCollection::FindClass(FdoInt64 classId) const
{
	const FdoSmLpSchema* pSchema;
	const FdoSmLpClassDefinition* pClass = NULL;
	bool found = false;

	for (int i = 0; i < GetCount() && !found; i++)	{
		pSchema = RefItem(i);
		if (pSchema)	{
			for (int j = 0; j < pSchema->RefClasses()->GetCount() && !found; j++)	{
				pClass = pSchema->RefClasses()->RefItem(j);
				if (pClass->GetId() == classId)
					found = true;
			}
		}
	}
	return pClass;
}

FdoFeatureSchemasP FdoSmLpSchemaCollection::GetFdoSchemas( FdoStringP schemaName)
{
    SchemaCollection            aTodo;

    /////////////////////////////////////////////
    // Setup

    mMappingClass.Clear();
    mMappingPropDef.Clear();

    if (schemaName != L"")
    {
        // Just want a specific schema (and referenced schemas)
        const FdoSmLpSchema*     pLpSchema = RefItem(schemaName);

        if (pLpSchema)
        {
            aTodo.Add((FdoSmLpSchema*)pLpSchema);    // drop const
        }
        else
        {
            throw FdoSchemaException::Create( 
                FdoSmError::NLSGetMessage(
                    FDO_NLSID(FDOSM_26), 
					(FdoString*) schemaName
                )
            );
        }
    }
    else
    {
        // Want all schemas
        for (int iSchema=0; iSchema < GetCount(); iSchema++)
        {
            const FdoSmLpSchema*     pLpSchema = RefItem(iSchema);
            // retrieve metaclass schema only if explicitly asked for.
            if ( wcscmp(pLpSchema->GetName(), FdoSmPhMgr::mMetaClassSchemaName) != 0 )
                aTodo.Add((FdoSmLpSchema*)pLpSchema);    // drop const
        }
    }

    /////////////////////////////////////////////
    // Process schemas

    FdoFeatureSchemasP pfscResult = FdoFeatureSchemaCollection::Create(NULL);
    for (int iSchema=0; iSchema < aTodo.GetCount(); iSchema++)
    {
        SchemaCollection    aReferenced;
        FdoPtr<FdoSmLpSchema> pLpSchema = aTodo.GetItem(iSchema);
        FdoFeatureSchema*   pFdoFeatureSchema = ConvertSchema(pLpSchema, aReferenced);

        if (pFdoFeatureSchema)
        {
            pfscResult->Add(pFdoFeatureSchema);
            pFdoFeatureSchema->AcceptChanges(); // resets all ElementStates to Unchanged

            // Add any new schema's referenced by this 
            // one to the list of schemas to convert.
            // NOTE: New schemas are added to the end 
            //       of the list, so this is safe 
            //       during a for-loop.
            aTodo.Merge(aReferenced);
        }
        pFdoFeatureSchema->Release();
    }

    /////////////////////////////////////////////
    // Done!

    return pfscResult;    // returned with refcount == 1
}

/*
FdoSmLpJoinTreeNode* FdoSmLpSchemaCollection::GetJoinTree( FdoStringP schemaName, FdoStringP className, const FdoStringsP& properties ) const
{
	const FdoSmLpClassDefinition* pClass = FindClass( schemaName, className );

	if ( !pClass ) 
		return(NULL);

	return ( pClass->GetJoinTree( properties ) );
}
*/

FdoSmLpQClassesP FdoSmLpSchemaCollection::TableToClasses( 
    FdoStringP tableName, 
    FdoStringP ownerName,
    FdoStringP databaseName
) const
{
    FdoInt32            idx;
    FdoSmLpQClassesP    classes = new FdoSmLpQClassCollection();

    for ( idx = 0; idx < GetCount(); idx++ ) {
        RefItem(idx)->TableToClasses( classes, tableName, ownerName, databaseName );
    }

    return classes;
}

FdoSmLpSchemaP FdoSmLpSchemaCollection::CreateSchema(
	FdoFeatureSchema* pFeatSchema,
    FdoPhysicalSchemaMapping* pOverrides,
    bool bIgnoreStates
)
{
    FdoSmLpSchemaP schema = NewSchema( pFeatSchema, bIgnoreStates );

    schema->Update( 
        pFeatSchema, 
        bIgnoreStates ? FdoSchemaElementState_Added : pFeatSchema->GetElementState(), 
        pOverrides,
        bIgnoreStates 
    );

    return schema;
}

void FdoSmLpSchemaCollection::Commit()
{
	for ( int i = 0; i < GetCount(); i++ ) {
		FdoSmLpSchemaP schema = GetItem(i);
		schema->Commit( true );
	}
}

FdoSchemaExceptionP FdoSmLpSchemaCollection::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	FdoSchemaExceptionP pException = FDO_SAFE_ADDREF(pFirstException);

	for ( int i = 0; i < GetCount(); i++ )
		pException = RefItem(i)->Errors2Exception(pException);

	return pException;
}


void FdoSmLpSchemaCollection::Load()
{
    int i;

	if ( (mPhysicalSchema != NULL) && !mSchemasLoaded ) {
        mSchemasLoaded = true;

        // Check if there is a config doc
        FdoFeatureSchemasP schemas = mPhysicalSchema->GetConfigSchemas();
        FdoSchemaMappingsP mappings = mPhysicalSchema->GetConfigMappings();
        FdoStringP providerName = mPhysicalSchema->GetProviderName();

        if ( schemas ) {
            // There is a config doc so load schemas from it first.
            for ( i = 0; i < schemas->GetCount(); i++ ) {
                FdoFeatureSchemaP schema = schemas->GetItem(i);
                FdoPhysicalSchemaMappingP mapping;
                
                if ( mappings ) 
                    mapping = mappings->GetItem(
                        providerName, 
                        schema->GetName() 
                    );

                FdoSmLpSchemaP lpSchema = NewSchema( schema, true );

                // Elements from config doc are never modified so mark them as Unchanged.
                lpSchema->Update(
                    schema, 
                    FdoSchemaElementState_Unchanged, 
                    mapping,
                    true
                );

                Add( lpSchema );
            }
        }

        // Next, read schemas from datastore
        FdoSmPhSchemaReaderP rdr = mPhysicalSchema->CreateSchemaReader();

        while ( rdr->ReadNext() ) {
            FdoSmLpSchemaP rdrSchema = NewSchema(rdr);
            // Skip any schemas already loaded from config doc.
            // Config doc takes precedence.
            if ( !schemas || (!FindItem(rdrSchema->GetName())) )
    		    this->Add( NewSchema(rdr) );
	    }
	}
}


void FdoSmLpSchemaCollection::XMLSerialize( FdoString* sFileName ) const
{
	FILE* xmlFp = fopen( (const char*) FdoStringP(sFileName), "w" );

	fprintf( xmlFp, "<?xml version=\"1.0\" standalone=\"yes\"?>\n" );
    fprintf( xmlFp, "<schemas xmlns:xsi=\"http://www.w3.org/2001/XMLSchema\" xmlns=\"http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical\" >\n" );
	
	for ( int i = 0; i < GetCount(); i++ ) {
		RefItem(i)->XMLSerialize(xmlFp, 0);
	}
	
	fprintf( xmlFp, "</schemas>\n" );

	fclose(xmlFp);

}

//TODO: The following functions should be moved to the schema element class that they operate on.

FdoFeatureSchema* FdoSmLpSchemaCollection::ConvertSchema(const FdoSmLpSchema* pLpSchema, SchemaCollection& aReferenced)
{
    ASSERT(pLpSchema);
    FdoFeatureSchema*                   pFdoFeatureSchema = FdoFeatureSchema::Create(pLpSchema->GetName(), pLpSchema->GetDescription());
    FdoClassCollection*                 pFdoClassCollection = pFdoFeatureSchema->GetClasses();

    // Loop through classes
    const FdoSmLpClassCollection*    pLpClassDefColl = pLpSchema->RefClasses();
    for (int iClass=0; iClass < pLpClassDefColl->GetCount(); iClass++)
    {
        const FdoSmLpClassDefinition*    pLpClassDef = pLpClassDefColl->RefItem(iClass);
        FdoClassDefinition*                 pFdoClassDef = ConvertClassDefinition(pLpClassDef, aReferenced);

        if (pFdoClassDef)
        {
            pFdoClassCollection->Add(pFdoClassDef);   // Also sets the parentage for pFdoClassDef
            pFdoClassDef->Release();
        }
    }
    FDO_SAFE_RELEASE(pFdoClassCollection);

    ConvertSAD(pLpSchema, pFdoFeatureSchema);

    return pFdoFeatureSchema;
}

FdoClassDefinition* FdoSmLpSchemaCollection::ConvertClassDefinition(const FdoSmLpClassDefinition* pLpClassDef, SchemaCollection& aReferenced)
{
    ASSERT(pLpClassDef);
    FdoClassDefinition* pFdoClassDef = (FdoClassDefinition*) mMappingClass.Map(pLpClassDef);

    if (!pFdoClassDef)
    {
        // Create an FDO version of the class
        switch (pLpClassDef->GetClassType())
        {
        case FdoClassType_Class:
            pFdoClassDef = FdoClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break;

        case FdoClassType_FeatureClass:
            // Create a FdoFeatureClass to match
            FdoFeatureClass*                pFdoFeatureClass;
            pFdoFeatureClass = FdoFeatureClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());

            const FdoSmLpFeatureClass*   pLpFeatureClass;
            pLpFeatureClass = (FdoSmLpFeatureClass*)pLpClassDef;
            if (pLpFeatureClass->RefGeometryProperty())
            {
                FdoGeometricPropertyDefinition*     pGeomProp = ConvertGeometricPropertyDefinition(pLpFeatureClass->RefGeometryProperty(), aReferenced);
                pFdoFeatureClass->SetGeometryProperty(pGeomProp);
                pGeomProp->Release();
            }

            pFdoClassDef = pFdoFeatureClass;
            break;

/* TODO:R2 - Support other classes as defined
        case FdoClassType_PointSegmentFeatureClass:
            pFdoClassDef = FdoPointSegmentFeatureClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            // TODO:R2 Populate pFdoPointSegFeatureClass->GetFeatureClasses();
            // TODO:R2 Populate pFdoPointSegFeatureClass->MeasureUnit(); (which doesn't exist yet)
            break;

        case FdoClassType_LineSegmentFeatureClass:
            // TODO:R2 pFdoClassDef = FdoLineSegmentFeatureClass::Create(pLpClassDef->GetName(), 
            //               pLpClassDef->GetDescription(), 
            //               FdoLineSegmentClass* value);
            // TODO:R2 Populate pFdoLineSegFeatureClass->GetFeatureClasses();
            // TODO:R2 Populate pFdoLineSegFeatureClass->MeasureUnit(); (which doesn't exist yet)
            break;

        case FdoClassType_LineSegmentClass:
            pFdoClassDef = FdoLineSegmentClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break;

        case FdoClassType_TopologyFeatureClass:
            pFdoClassDef = FdoTopologyFeatureClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break;

        case FdoClassType_TinFeatureClass:
            // TODO:R2 pFdoClassDef = FdoTinFeatureClass::Create(pLpClassDef->GetName(), 
            //              pLpClassDef->GetDescription(), 
            //              FdoTinPointClass* pointClass, 
            //              FdoTinEdgeClass* edgeClass););
            // TODO:R2 Populate the pFdoTinFeatureClass->GetCoverageProperties()->Add(...);
            break;

        case FdoClassType_TinPointClass:
            pFdoClassDef = FdoTinPointClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break;

        case FdoClassType_TinEdgeClass:
            pFdoClassDef = FdoTinEdgeClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break;

        case FdoClassType_AssociationFeatureClass:
            pFdoClassDef = FdoAssociationFeatureClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break; 
*/


        default:
            // this should never happen unless somebody adds a new FdoClassType_xxx
            ASSERT(false);
            return NULL;
        }

        // The following MUST be performed before calling ConvertClassDefinition()
        // again (directly or indirectly) to avoid recursive infinite loops.
        //
        mMappingClass.Add(pLpClassDef, pFdoClassDef);

        // copy base class
        const FdoSmLpClassDefinition*  pLpClassDefBase = pLpClassDef->RefBaseClass();
        if (pLpClassDefBase)
        {
            FdoClassDefinition*     pBaseClase = ConvertClassDefinition(pLpClassDefBase, aReferenced);
            pFdoClassDef->SetBaseClass(pBaseClase);
            pBaseClase->Release();
        }

        // copy abstract state
        pFdoClassDef->SetIsAbstract(pLpClassDef->GetIsAbstract());

        // populate properties
        //
        const FdoSmLpPropertyDefinitionCollection*   pLpPropDefColl = pLpClassDef->RefProperties();
        FdoPropertyDefinitionCollection*                pFdoBaseProperties = FdoPropertyDefinitionCollection::Create(NULL);
        FdoPropertyDefinitionCollection*                pFdoProperties = pFdoClassDef->GetProperties();
         for (int iProp=0; iProp < pLpPropDefColl->GetCount(); iProp++)
        {
            const FdoSmLpPropertyDefinition*     pLpPropDef = pLpPropDefColl->RefItem(iProp);

            if ( (!pLpClassDefBase) || (pLpPropDef->RefDefiningClass() == pLpClassDef) )
            {
                // Create an FDO version of the property.
                FdoPropertyDefinition*                  pFdoPropDef = NULL;
                switch (pLpPropDef->GetPropertyType())
                {
                case FdoPropertyType_DataProperty:
                    pFdoPropDef = ConvertDataPropertyDefinition((const FdoSmLpDataPropertyDefinition*)pLpPropDef, aReferenced);
                    break;

                case FdoPropertyType_ObjectProperty:
                    pFdoPropDef = ConvertObjectPropertyDefinition((const FdoSmLpObjectPropertyDefinition*)pLpPropDef, aReferenced);
                    break;

                case FdoPropertyType_GeometricProperty:
                    pFdoPropDef = ConvertGeometricPropertyDefinition((const FdoSmLpGeometricPropertyDefinition*)pLpPropDef, aReferenced);
                    break;

                case FdoPropertyType_AssociationProperty:
                    pFdoPropDef = ConvertAssociationPropertyDefinition((const FdoSmLpAssociationPropertyDefinition*)pLpPropDef, aReferenced);
                    break;

                default:
                    // this should be impossible to reach
                    ASSERT(false);
                    break;
                }

                if (pFdoPropDef)
                {
                    // Is this an inherited property?
                    if (pLpPropDef->RefDefiningClass() == pLpClassDef)
                    {
                        // Not inhereited, add to Properties collection
                        pFdoProperties->Add(pFdoPropDef);
                    }
                    else
                    {
                        // Inherited, add to BaseProperties collection.

                        bool addProp = true;

                        // Some simple system properties do not have a column in the
                        // class table. This is a legitimate case. For example,
                        // a foreign table often has no column for the
                        // RevisionNumber property.

                        // Skip properties that don't have a column in the
                        // class table. 
                        const FdoSmLpSimplePropertyDefinition* pLpSimplePropDef =
                            FdoSmLpSimplePropertyDefinition::Cast(pLpPropDef);

                        if ( pLpSimplePropDef ) {
                            const FdoSmPhColumn* pColumn = pLpSimplePropDef->RefColumn();
                            if ( !pColumn || !pColumn->GetParent() ) {
                                // Skip when no column or column is orphan
                                addProp = false;
                            }
                            else {
                                if ( FdoStringP(pColumn->GetParent()->GetName()).ICompare(pLpClassDef->GetDbObjectName()) != 0 )
                                    // Skip when column not in class table.
                                    addProp = false;
                            }
                        }

                        if ( addProp ) {
                            pFdoBaseProperties->Add(pFdoPropDef);

                            // Add a reference in case the class (inherited or not) comes
                            // from is in a different schema.
                            aReferenced.AddReference(pLpPropDef->RefDefiningClass()->RefLogicalPhysicalSchema());
                        }
                    }

                    pFdoPropDef->Release(); // we're done with it, and collections should have AddRef'ed
                }
            }
        }

        // Set the base property collection
        if (pFdoBaseProperties->GetCount() > 0)
            pFdoClassDef->SetBaseProperties(pFdoBaseProperties);
        FDO_SAFE_RELEASE(pFdoBaseProperties);
        FDO_SAFE_RELEASE(pFdoProperties);

        // Populate the Identity Properties
        // SSP? Can only base classes have identity properties?
        //
        if (!pLpClassDef->RefBaseClass())
        {
            const FdoSmLpDataPropertyDefinitionCollection*   pLpIdPropDefColl = pLpClassDef->RefIdentityProperties();
            FdoDataPropertyDefinitionCollection*                pFdoIdentityProperties = pFdoClassDef->GetIdentityProperties();

            for (int iProp=0; iProp < pLpIdPropDefColl->GetCount(); iProp++)
            {
                const FdoSmLpDataPropertyDefinition* pLpIdPropDef = pLpIdPropDefColl->RefItem(iProp);

                FdoDataPropertyDefinition*  pIdProp = ConvertDataPropertyDefinition(pLpIdPropDef, aReferenced);
                pFdoIdentityProperties->Add(pIdProp);
                pIdProp->Release();
            }
            FDO_SAFE_RELEASE(pFdoIdentityProperties);
        }

        // Populate class capabilities
        const FdoSmLpClassDefinition::Capabilities* pLpCap = pLpClassDef->GetCapabilities();

        if ( pLpCap ) {
            FdoClassCapabilitiesP capabilities = FdoClassCapabilities::Create( (*pFdoClassDef) );
            capabilities->SetSupportsLocking( pLpCap->SupportsLocking() );
            capabilities->SetSupportsLongTransactions( pLpCap->SupportsLongTransactions() );

            FdoInt32 lockTypeCount;
            const FdoLockType* lockTypes = pLpCap->GetLockTypes( lockTypeCount );
            capabilities->SetLockTypes( lockTypes, lockTypeCount );
            // capabilities->SetSupportsWrite(true); // TODO - Capability: How to determine whether true or false is to be set.
            capabilities->SetSupportsWrite( pLpCap->SupportsWrite() );

            pFdoClassDef->SetCapabilities( capabilities );
        }

        ConvertSAD(pLpClassDef, pFdoClassDef);

		// Populate constraints
		ConvertConstraints(pLpClassDef, pFdoClassDef);

        aReferenced.AddReference(pLpClassDef->RefLogicalPhysicalSchema());
    }
    else {
        pFdoClassDef->AddRef();
    }

    return pFdoClassDef;
}

FdoDataPropertyDefinition* FdoSmLpSchemaCollection::ConvertDataPropertyDefinition(const FdoSmLpDataPropertyDefinition* pLpDataPropDef, SchemaCollection& aReferenced)
{
    ASSERT(pLpDataPropDef);
    ASSERT(pLpDataPropDef->GetPropertyType() == FdoPropertyType_DataProperty);

    FdoDataPropertyDefinition*  pFdoDataPropDef = (FdoDataPropertyDefinition*) mMappingPropDef.Map(pLpDataPropDef);
    if (!pFdoDataPropDef)
    {
        pFdoDataPropDef = FdoDataPropertyDefinition::Create(pLpDataPropDef->GetName(), pLpDataPropDef->GetDescription());

        pFdoDataPropDef->SetDataType(pLpDataPropDef->GetDataType());
        pFdoDataPropDef->SetReadOnly(pLpDataPropDef->GetReadOnly());
        pFdoDataPropDef->SetLength(pLpDataPropDef->GetLength());
        pFdoDataPropDef->SetPrecision(pLpDataPropDef->GetPrecision());
        pFdoDataPropDef->SetScale(pLpDataPropDef->GetScale());
        pFdoDataPropDef->SetNullable(pLpDataPropDef->GetNullable());
        pFdoDataPropDef->SetDefaultValue(pLpDataPropDef->GetDefaultValue());
        pFdoDataPropDef->SetIsAutoGenerated(pLpDataPropDef->GetIsAutoGenerated());

        pFdoDataPropDef->SetIsSystem( pLpDataPropDef->GetIsSystem() );

        ConvertSAD(pLpDataPropDef, pFdoDataPropDef);

        aReferenced.AddReference(pLpDataPropDef->RefLogicalPhysicalSchema());
        mMappingPropDef.Add(pLpDataPropDef, pFdoDataPropDef);
    }
    else
        pFdoDataPropDef->AddRef();

    return pFdoDataPropDef;
}

FdoObjectPropertyDefinition* FdoSmLpSchemaCollection::ConvertObjectPropertyDefinition(const FdoSmLpObjectPropertyDefinition* pLpObjPropDef, SchemaCollection& aReferenced)
{
    ASSERT(pLpObjPropDef);
    ASSERT(pLpObjPropDef->GetPropertyType() == FdoPropertyType_ObjectProperty);

    FdoObjectPropertyDefinition*    pFdoObjPropDef = (FdoObjectPropertyDefinition*) mMappingPropDef.Map(pLpObjPropDef);
    if (!pFdoObjPropDef)
    {
        pFdoObjPropDef = FdoObjectPropertyDefinition::Create(pLpObjPropDef->GetName(), pLpObjPropDef->GetDescription());

        const FdoSmLpClassDefinition*    pLpClassDef = pLpObjPropDef->RefClass();
        FdoClassDefinition*                 pFdoClassDef = pLpClassDef ? ConvertClassDefinition(pLpClassDef, aReferenced) : NULL;
        pFdoObjPropDef->SetClass(pFdoClassDef);
        FDO_SAFE_RELEASE(pFdoClassDef);

        pFdoObjPropDef->SetObjectType(pLpObjPropDef->GetObjectType());

        pFdoObjPropDef->SetOrderType(pLpObjPropDef->GetOrderType());
        if (pLpObjPropDef->RefIdentityProperty())
        {
            // TODO: Should the following ASSERT become a part of the if()?
            ASSERT(pFdoObjPropDef->GetObjectType() == FdoObjectType_Collection || pFdoObjPropDef->GetObjectType() == FdoObjectType_OrderedCollection);
            FdoDataPropertyDefinition*  pIdentProp = ConvertDataPropertyDefinition(pLpObjPropDef->RefIdentityProperty(), aReferenced);
            pFdoObjPropDef->SetIdentityProperty(pIdentProp);
            pIdentProp->Release();
        }

        pFdoObjPropDef->SetIsSystem(pLpObjPropDef->GetIsSystem());

        ConvertSAD(pLpObjPropDef, pFdoObjPropDef);

        aReferenced.AddReference(pLpObjPropDef->RefLogicalPhysicalSchema());
        mMappingPropDef.Add(pLpObjPropDef, pFdoObjPropDef);
    }
    else
        pFdoObjPropDef->AddRef();

    return pFdoObjPropDef;
}



FdoGeometricPropertyDefinition* FdoSmLpSchemaCollection::ConvertGeometricPropertyDefinition(const FdoSmLpGeometricPropertyDefinition* pLpGeomPropDef, SchemaCollection& aReferenced)
{
    ASSERT(pLpGeomPropDef);
    ASSERT(pLpGeomPropDef->GetPropertyType() == FdoPropertyType_GeometricProperty);

    FdoGeometricPropertyDefinition* pFdoGeomPropDef = (FdoGeometricPropertyDefinition*) mMappingPropDef.Map(pLpGeomPropDef);

    if (!pFdoGeomPropDef)
    {
        pFdoGeomPropDef = FdoGeometricPropertyDefinition::Create(pLpGeomPropDef->GetName(), pLpGeomPropDef->GetDescription());

        pFdoGeomPropDef->SetGeometryTypes(pLpGeomPropDef->GetGeometryTypes());

        FdoGeometryType geomTypes[MAX_GEOMETRY_TYPE_SIZE];
        FdoInt32 geomTypeCount;

        FdoCommonGeometryUtil::GeometryTypesToArray( pLpGeomPropDef->GetSpecificGeometryTypes(), geomTypes, geomTypeCount );
        pFdoGeomPropDef->SetSpecificGeometryTypes( geomTypes, geomTypeCount );

        pFdoGeomPropDef->SetReadOnly(pLpGeomPropDef->GetReadOnly());
        pFdoGeomPropDef->SetHasMeasure(pLpGeomPropDef->GetHasMeasure());
        pFdoGeomPropDef->SetHasElevation(pLpGeomPropDef->GetHasElevation());
        pFdoGeomPropDef->SetSpatialContextAssociation(pLpGeomPropDef->GetSpatialContextAssociation());
        pFdoGeomPropDef->SetIsSystem(pLpGeomPropDef->GetIsSystem());
        ConvertSAD(pLpGeomPropDef, pFdoGeomPropDef);

        aReferenced.AddReference(pLpGeomPropDef->RefLogicalPhysicalSchema());
        mMappingPropDef.Add(pLpGeomPropDef, pFdoGeomPropDef);
    }
    else
        pFdoGeomPropDef->AddRef();

    return pFdoGeomPropDef;
}

FdoAssociationPropertyDefinition* FdoSmLpSchemaCollection::ConvertAssociationPropertyDefinition(const FdoSmLpAssociationPropertyDefinition* pLpAssocPropDef, SchemaCollection& aReferenced )
{
    ASSERT(pLpAssocPropDef);
    ASSERT(pLpAssocPropDef->GetPropertyType() == FdoPropertyType_AssociationProperty);

    if( pLpAssocPropDef->GetReadOnly() ) // We do not convert the readonly as they will be added automatically by FDO code.
        return NULL;

    FdoAssociationPropertyDefinition*    pFdoAssocPropDef = (FdoAssociationPropertyDefinition*) mMappingPropDef.Map(pLpAssocPropDef);
    if (!pFdoAssocPropDef)
    {
        pFdoAssocPropDef = FdoAssociationPropertyDefinition::Create(pLpAssocPropDef->GetName(), pLpAssocPropDef->GetDescription());

        const FdoSmLpClassDefinition*    pLpAssocClassDef = pLpAssocPropDef->RefAssociatedClass();
        FdoClassDefinition*                 pFdoAssocClassDef = pLpAssocClassDef ? ConvertClassDefinition(pLpAssocClassDef, aReferenced) : NULL;
        pFdoAssocPropDef->SetIsReadOnly( pLpAssocPropDef->GetReadOnly() );
        pFdoAssocPropDef->SetReverseName( pLpAssocPropDef->GetReverseName() );
        pFdoAssocPropDef->SetAssociatedClass( pFdoAssocClassDef );
        pFdoAssocPropDef->SetDeleteRule( pLpAssocPropDef->GetDeleteRule() );
        pFdoAssocPropDef->SetMultiplicity( pLpAssocPropDef->GetMultiplicity() );
        pFdoAssocPropDef->SetReverseMultiplicity( pLpAssocPropDef->GetReverseMultiplicity() );
        pFdoAssocPropDef->SetLockCascade( pLpAssocPropDef->GetCascadeLock() );

        //
        // Determine if we need to populate the Identity and reverse identity properties
        // We should not populate the 2 collection if the PH 
        if( pFdoAssocClassDef )
        {
            const FdoStringsP identProps = pLpAssocPropDef->GetIdentityProperties();
            for(int i=0; i<identProps->GetCount(); i++ )
            {
                FdoPtr<FdoDataPropertyDefinition> fdoProp = (FdoDataPropertyDefinition*)FdoPtr<FdoPropertyDefinitionCollection>(pFdoAssocClassDef->GetProperties())->GetItem( (const wchar_t *)identProps->GetString(i) );
                if( fdoProp )
                    FdoPtr<FdoDataPropertyDefinitionCollection>(pFdoAssocPropDef->GetIdentityProperties())->Add( fdoProp );
            }
        }
        if( pLpAssocPropDef )
        {
            const FdoStringsP identProps = pLpAssocPropDef->GetReverseIdentityProperties();
            for(int i=0; i<identProps->GetCount(); i++ )
            {
                const FdoSmLpPropertyDefinition* pLpPropDef = pLpAssocPropDef->RefParentClass()->RefProperties()->RefItem( (const wchar_t *)identProps->GetString(i) );
                if( pLpPropDef == NULL || pLpPropDef->GetPropertyType() !=  FdoPropertyType_DataProperty )
                    continue;
                FdoPtr<FdoDataPropertyDefinition> fdoProp = ConvertDataPropertyDefinition( (const FdoSmLpDataPropertyDefinition*)pLpPropDef, aReferenced);
                if( fdoProp )
                    FdoPtr<FdoDataPropertyDefinitionCollection>(pFdoAssocPropDef->GetReverseIdentityProperties())->Add( fdoProp );
            }
        }
        
        FDO_SAFE_RELEASE(pFdoAssocClassDef);
        ConvertSAD(pLpAssocPropDef, pFdoAssocPropDef );

        aReferenced.AddReference(pLpAssocPropDef->RefLogicalPhysicalSchema());
        mMappingPropDef.Add(pLpAssocPropDef, pFdoAssocPropDef);
    }
    else
        pFdoAssocPropDef->AddRef();

    return pFdoAssocPropDef;
}

void FdoSmLpSchemaCollection::ConvertSAD(const FdoSmLpSchemaElement* pLpElement, FdoSchemaElement* pFdoElement)
{
    const FdoSmLpSAD*  pLpSAD = pLpElement->RefSAD();

    if (pLpSAD && pLpSAD->GetCount() > 0)
    {
        FdoPtr<FdoSchemaAttributeDictionary>  pFdoSAD = pFdoElement->GetAttributes();

        for (int i=0; i < pLpSAD->GetCount(); i++)
        {
            const FdoSmLpSADElement* pLpSADElement = pLpSAD->RefItem(i);

            if (pLpSADElement)
                pFdoSAD->Add(pLpSADElement->GetName(), pLpSADElement->GetValue());
        }
    }
}

void FdoSmLpSchemaCollection::ConvertConstraints(const FdoSmLpClassDefinition* pLpClassDef, FdoClassDefinition* pFdoClassDef)
{
	///////////////////////////////////////////////////////////////////////////
	// UNIQUE() constraints : Load from the database
	///////////////////////////////////////////////////////////////////////////

	FdoSmLpUniqueConstraintCollection*			uLpUKeys = (FdoSmLpUniqueConstraintCollection *)((FdoSmLpClassBase *)pLpClassDef)->RefUniqueConstraints();
	FdoPtr<FdoUniqueConstraintCollection>		pFdoUKeys = pFdoClassDef->GetUniqueConstraints();
	FdoPropertiesP								pFdoProps = pFdoClassDef->GetProperties();

	for ( int i = 0; i < uLpUKeys->GetCount(); i++ ) {
		FdoSmLpUniqueConstraintP	pLpUniqueC = uLpUKeys->GetItem(i);
		FdoSmLpDataPropertiesP		pLpProps = pLpUniqueC->GetProperties();

		// New constraints
		FdoPtr<FdoUniqueConstraint>	pFdoUniqueC = FdoUniqueConstraint::Create();
		FdoDataPropertiesP			pFdoUniqueProps = pFdoUniqueC->GetProperties();

		for ( int j = 0; j < pLpProps->GetCount(); j++ ) {
			FdoSmLpDataPropertyP	pLpProp = pLpProps->GetItem(j);
			
			// Find the Fdo data property
			FdoDataPropertyP   pFdoProp = (FdoDataPropertyDefinition *)pFdoProps->FindItem(pLpProp->GetName());
					
			// Match the names and add to collection
			if ( pFdoProp && ( wcscmp(pLpProp->GetName(), pFdoProp->GetName()) == 0 ) )
				pFdoUniqueProps->Add( pFdoProp );
		}
		if ( pFdoUniqueProps->GetCount() != 0 )
			pFdoUKeys->Add(pFdoUniqueC);
	}

	///////////////////////////////////////////////////////////////////////////
	// CHECK() constraints : Load from the database
	///////////////////////////////////////////////////////////////////////////

	FdoSmLpCheckConstraintCollection*	pLpCKeys = (FdoSmLpCheckConstraintCollection *)((FdoSmLpClassBase *)pLpClassDef)->RefCheckConstraints();


	// For each constraint, try to find a matching data property
	for ( int j = 0; j < pLpCKeys->GetCount(); j++ ) {

		FdoSmLpCheckConstraintP		pLpCKey = pLpCKeys->GetItem(j);			
		FdoDataPropertyP			pDataFdoProp = (FdoDataPropertyDefinition *)pFdoProps->FindItem( pLpCKey->GetPropertyName() );

		// Attach the constraint to the data property
		if ( pDataFdoProp && (pLpCKey->GetClause() != L"") ) {
			FdoPtr<FdoPropertyValueConstraint> pConstr;
            FdoCommonParse      			   *parser  = NULL;
			FdoPtr<FdoDataValue>			   val;

            try {
				parser = new FdoCommonParse();
				if (parser)
					pConstr = parser->ParseConstraint( pLpCKey->GetClause() );
            }
            catch ( FdoExpressionException* ex ) {
                // The LogicalPhysical Schema also sends back constraints defined
                // outside of FDO, including ones not FDO-supported. Silently skip
                // the unsupported constraints.
                pConstr = NULL;
                ex->Release();
            }

			if ( pConstr ) {
				
				FdoDataType	propType = pDataFdoProp->GetDataType();

				// Check if the constraint type matches the property data type since the Parser cannot guess.
				if ( pConstr->GetConstraintType() == FdoPropertyValueConstraintType_Range ) 
				{
					FdoPropertyValueConstraintRange*	pConstrR = static_cast<FdoPropertyValueConstraintRange*>((FdoPropertyValueConstraint*)pConstr);
					
					pConstrR->SetMinValue( FixDataValueType( FdoPtr<FdoDataValue>(pConstrR->GetMinValue()), propType ));
					pConstrR->SetMaxValue( FixDataValueType( FdoPtr<FdoDataValue>(pConstrR->GetMaxValue()), propType ));
				} 
				else if ( pConstr->GetConstraintType() == FdoPropertyValueConstraintType_List ) 
				{
					FdoPropertyValueConstraintList	*pConstrL = static_cast<FdoPropertyValueConstraintList*>((FdoPropertyValueConstraint*)pConstr);
					FdoPtr<FdoDataValueCollection>	vals = pConstrL->GetConstraintList();

					for ( int i = 0; i < vals->GetCount(); i++ )
					{
						vals->SetItem( i, FixDataValueType(vals->GetItem(i), propType) );
					}	
				}

				// Attach the constraint to property
    			pDataFdoProp->SetValueConstraint( pConstr );
			}
			delete parser;
		}
	}
}

FdoPtr<FdoDataValue> FdoSmLpSchemaCollection::FixDataValueType( FdoPtr<FdoDataValue> val, FdoDataType propType )
{
	FdoInt32		valInt32;
	FdoInt64		valInt64;
	FdoDouble		valDouble;

	FdoPtr<FdoDataValue>	ret = val;

	// Fast return if nothing to do
	if ( val == NULL )
		return ret;

	FdoDataType	constrType = val->GetDataType();

	if ( constrType != propType ) 
	{
		FdoDataValue	*newData = NULL;

        switch (constrType) {
        case FdoDataType_Int32:
			valInt32 = ((FdoInt32Value*)(FdoDataValue*)val)->GetInt32();

            switch (propType) {
            case FdoDataType_Byte:
			    newData = FdoDataValue::Create( (FdoByte)valInt32 );
                break;

            case FdoDataType_Int16:
			    newData = FdoDataValue::Create( (FdoInt16)valInt32 );
                break;

            case FdoDataType_Int64:
			    newData = FdoDataValue::Create( (FdoInt64)valInt32);
                break;

            case FdoDataType_Single:
			    newData = FdoDataValue::Create( (FdoFloat)valInt32);
                break;

            case FdoDataType_Double:
            case FdoDataType_Decimal:
    			newData = FdoDataValue::Create( (FdoDouble) valInt32, propType );
                break;
            }

            break;

        case FdoDataType_Int64:
			valInt64 = ((FdoInt64Value*)(FdoDataValue*)val)->GetInt64();

            switch (propType) {
            case FdoDataType_Byte:
			    newData = FdoDataValue::Create( (FdoByte)valInt64 );
                break;

            case FdoDataType_Int16:
			    newData = FdoDataValue::Create( (FdoInt16)valInt64 );
                break;

            case FdoDataType_Int32:
			    newData = FdoDataValue::Create( (FdoInt32)valInt64);
                break;

            case FdoDataType_Single:
			    newData = FdoDataValue::Create( (FdoFloat)valInt64);
                break;

            case FdoDataType_Double:
            case FdoDataType_Decimal:
    			newData = FdoDataValue::Create( (FdoDouble) valInt64, propType );
                break;
            }

            break;

        case FdoDataType_Double:
			valDouble = ((FdoDoubleValue*)(FdoDataValue*)val)->GetDouble();

            switch (propType) {
            case FdoDataType_Byte:
			    newData = FdoDataValue::Create( (FdoByte)valDouble );
                break;

            case FdoDataType_Int16:
			    newData = FdoDataValue::Create( (FdoInt16)valDouble );
                break;

            case FdoDataType_Int32:
			    newData = FdoDataValue::Create( (FdoInt32)valDouble );
                break;

            case FdoDataType_Int64:
			    newData = FdoDataValue::Create( (FdoInt64)valDouble);
                break;

            case FdoDataType_Single:
			    newData = FdoDataValue::Create( (FdoFloat)valDouble);
                break;

            case FdoDataType_Decimal:
    			newData = FdoDataValue::Create( valDouble, propType );
                break;
            }

            break;

        }

        ret = newData;
	}
	return ret;
}
