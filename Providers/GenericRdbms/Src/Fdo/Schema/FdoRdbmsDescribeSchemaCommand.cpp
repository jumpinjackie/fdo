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

// FdoRdbmsInsertCommand
#include "FdoRdbmsDescribeSchemaCommand.h"
#include "Parse/Parse.h"
#include <Sm/Lp/FeatureClass.h>
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/Schema.h>
#include <Sm/Lp/AssociationPropertyDefinition.h>

#include "FdoRdbmsSchemaUtil.h"

#ifndef ASSERT
#ifdef _DEBUG
    #include <CrtDbg.h>
    #define ASSERT  _ASSERT
#else
    #define ASSERT(x) {}
#endif
#endif

//////////////////////////////////////////////////////////////////////////
// Class FdoRdbmsDescribeSchemaCommand

/*  Do not implement the copy constructor to avoid a lot of problems. BK
FdoRdbmsDescribeSchemaCommand::FdoRdbmsDescribeSchemaCommand(const FdoRdbmsDescribeSchemaCommand &right)
{
}
*/

FdoRdbmsDescribeSchemaCommand::FdoRdbmsDescribeSchemaCommand() :
    mRdbmsConnection( NULL )
{
}

FdoRdbmsDescribeSchemaCommand::FdoRdbmsDescribeSchemaCommand(FdoIConnection *connection)
{
	mFdoConnection = connection;
	FDO_SAFE_ADDREF(mFdoConnection.p);

    FdoRdbmsConnection *conn  = static_cast<FdoRdbmsConnection*>(connection);
    if( conn )
        mRdbmsConnection = conn->GetDbiConnection();
}

FdoRdbmsDescribeSchemaCommand::FdoRdbmsDescribeSchemaCommand(DbiConnection *connection)
{
    mRdbmsConnection = connection;
}

FdoRdbmsDescribeSchemaCommand::~FdoRdbmsDescribeSchemaCommand()
{
}

const wchar_t* FdoRdbmsDescribeSchemaCommand::GetSchemaName()
{
    return mSchemaName;
}


void FdoRdbmsDescribeSchemaCommand::SetSchemaName(const wchar_t* value)
{
    mSchemaName = value;
}

FdoFeatureSchemaCollection* FdoRdbmsDescribeSchemaCommand::Execute()
{
    if( NULL == mRdbmsConnection )
    {
        throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));
    }

    try
    {
        FdoSchemaManagerP      pschemaManager = mRdbmsConnection->GetSchemaManager();
        const FdoSmLpSchemaCollection* pLpSchemaColl = pschemaManager->RefLogicalPhysicalSchemas();
        SchemaCollection            aTodo;

        /////////////////////////////////////////////
        // Setup

        mMappingClass.Clear();
        mMappingPropDef.Clear();

        if ( wcslen(GetSchemaName()) > 0)
        {
            // Just want a specific schema (and referenced schemas)
            const FdoSmLpSchema*     pLpSchema = pLpSchemaColl->RefItem(GetSchemaName());

            if (pLpSchema)
            {
                aTodo.Add((FdoSmLpSchema*)pLpSchema);    // drop const
            }
            else
            {
                throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_182, "Unable to find schema"));
            }
        }
        else
        {
            // Want all schemas
            for (int iSchema=0; iSchema < pLpSchemaColl->GetCount(); iSchema++)
            {
                const FdoSmLpSchema*     pLpSchema = pLpSchemaColl->RefItem(iSchema);
                // retrieve metaclass schema only if explicitly asked for.
                if ( wcscmp(pLpSchema->GetName(), FdoSmLpSchema::mMetaClassSchemaName) != 0 )
                    aTodo.Add((FdoSmLpSchema*)pLpSchema);    // drop const
            }
        }

        /////////////////////////////////////////////
        // Process schemas

        FdoFeatureSchemaCollection* pfscResult = FdoFeatureSchemaCollection::Create(NULL);
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
    catch (FdoSchemaException *ex)
    {
        // Catch and rethrow FdoSchemaExceptions here so the following
        // catch for FdoExceptions doesn't get them.
        throw ex;
    }
    catch (FdoException *ex)
    {
        throw FdoSchemaException::Create(ex->GetExceptionMessage(), ex);
    }
}

FdoFeatureSchema* FdoRdbmsDescribeSchemaCommand::ConvertSchema(const FdoSmLpSchema* pLpSchema, SchemaCollection& aReferenced)
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

		// Populate constraints
		ConvertConstraints(pLpClassDef, pFdoClassDef);

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

FdoClassDefinition* FdoRdbmsDescribeSchemaCommand::ConvertClassDefinition(const FdoSmLpClassDefinition* pLpClassDef, SchemaCollection& aReferenced)
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

            // When class has base class, skip inherited properties.
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
            capabilities->SetSupportsWrite(true);

            FdoInt32 lockTypeCount;
            const FdoLockType* lockTypes = pLpCap->GetLockTypes( lockTypeCount );
            capabilities->SetLockTypes( lockTypes, lockTypeCount );

            pFdoClassDef->SetCapabilities( capabilities );
        }

        ConvertSAD(pLpClassDef, pFdoClassDef);

        aReferenced.AddReference(pLpClassDef->RefLogicalPhysicalSchema());
    }
    else {
        pFdoClassDef->AddRef();
    }

    return pFdoClassDef;
}

FdoDataPropertyDefinition* FdoRdbmsDescribeSchemaCommand::ConvertDataPropertyDefinition(const FdoSmLpDataPropertyDefinition* pLpDataPropDef, SchemaCollection& aReferenced)
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

FdoObjectPropertyDefinition* FdoRdbmsDescribeSchemaCommand::ConvertObjectPropertyDefinition(const FdoSmLpObjectPropertyDefinition* pLpObjPropDef, SchemaCollection& aReferenced)
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


FdoGeometricPropertyDefinition* FdoRdbmsDescribeSchemaCommand::ConvertGeometricPropertyDefinition(const FdoSmLpGeometricPropertyDefinition* pLpGeomPropDef, SchemaCollection& aReferenced)
{
    ASSERT(pLpGeomPropDef);
    ASSERT(pLpGeomPropDef->GetPropertyType() == FdoPropertyType_GeometricProperty);

    FdoGeometricPropertyDefinition* pFdoGeomPropDef = (FdoGeometricPropertyDefinition*) mMappingPropDef.Map(pLpGeomPropDef);

    if (!pFdoGeomPropDef)
    {
        pFdoGeomPropDef = FdoGeometricPropertyDefinition::Create(pLpGeomPropDef->GetName(), pLpGeomPropDef->GetDescription());

        pFdoGeomPropDef->SetGeometryTypes(pLpGeomPropDef->GetGeometryTypes());
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

FdoAssociationPropertyDefinition* FdoRdbmsDescribeSchemaCommand::ConvertAssociationPropertyDefinition(const FdoSmLpAssociationPropertyDefinition* pLpAssocPropDef, SchemaCollection& aReferenced )
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

void FdoRdbmsDescribeSchemaCommand::ConvertSAD(const FdoSmLpSchemaElement* pLpElement, FdoSchemaElement* pFdoElement)
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

void FdoRdbmsDescribeSchemaCommand::ConvertConstraints(const FdoSmLpClassDefinition* pLpClassDef, FdoClassDefinition* pFdoClassDef)
{
	///////////////////////////////////////////////////////////////////////////
	// UNIQUE() constraints : Load from the database
	///////////////////////////////////////////////////////////////////////////

	FdoSmLpUniqueConstraintCollection*		uLpUKeys = (FdoSmLpUniqueConstraintCollection *)((FdoSmLpClassBase *)pLpClassDef)->RefUniqueConstraints();
	FdoPtr<FdoUniqueConstraintCollection>	pFdoUKeys = pFdoClassDef->GetUniqueConstraints();
	FdoPropertiesP							pFdoProps = pFdoClassDef->GetProperties();

	// All the providers support UNIQUE.
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

	FdoSmLpCheckConstraintCollection	*pLpCKeys = (FdoSmLpCheckConstraintCollection *)((FdoSmLpClassBase *)pLpClassDef)->RefCheckConstraints();

	FdoCommonParse* parser = new FdoCommonParse();

	// For each constraint, try to find a matching data property
	for ( int j = 0; j < pLpCKeys->GetCount(); j++ ) {

		FdoSmLpCheckConstraintP		pLpCKey = pLpCKeys->GetItem(j);			
		FdoDataPropertyP			pDataFdoProp = (FdoDataPropertyDefinition *)pFdoProps->FindItem( pLpCKey->GetPropertyName() );

		// Attach the constraint to the data property
		if ( pDataFdoProp ) {
            FdoPropertyValueConstraint* pConstr = NULL;

            try {
                pConstr = parser->ParseConstraint( pLpCKey->GetClause() );
            }
            catch ( FdoExpressionException* ex ) {
                // The LogicalPhysical Schema also sends back constraints defined
                // outside of FDO, including ones not FDO-supported. Silently skip
                // the unsupported constraints.
                pConstr = NULL;
                ex->Release();
            }

            if ( pConstr ) 
    			pDataFdoProp->SetValueConstraint( pConstr );
		}
	}
	delete parser; 
}
