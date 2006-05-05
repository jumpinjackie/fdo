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
#include "ObjectPropertyClass.h"
#include "PropertyMappingConcrete.h"
#include "PropertyMappingSingle.h"
#include "ObjectPropertyDefinition.h"

FdoSmLpGrdObjectPropertyDefinition::FdoSmLpGrdObjectPropertyDefinition(
    FdoSmPhClassPropertyReaderP propReader, 
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpObjectPropertyDefinition(propReader, parent),
    mFixedDefaultName(false)
{
}

FdoSmLpGrdObjectPropertyDefinition::FdoSmLpGrdObjectPropertyDefinition(
    FdoObjectPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpObjectPropertyDefinition(pFdoProp, bIgnoreStates, parent),
    mFixedDefaultName(false)
{
}

FdoSmLpGrdObjectPropertyDefinition::FdoSmLpGrdObjectPropertyDefinition(
    FdoSmLpObjectPropertyP pBaseProperty, 
    FdoSmLpClassDefinition* pTargetClass, 
    FdoStringP logicalName, 
    FdoStringP physicalName, 
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
	FdoSmLpObjectPropertyDefinition(
        pBaseProperty, 
        pTargetClass, 
        logicalName, 
        physicalName, 
        bInherit,
        pPropOverrides
    ),
    mFixedDefaultName(false)
{
    // Apply the Schema Overrides.
    Update( pPropOverrides );
}

FdoSmLpGrdObjectPropertyDefinition::~FdoSmLpGrdObjectPropertyDefinition(void)
{
}

FdoSmLpObjectPropertyClassP FdoSmLpGrdObjectPropertyDefinition::NewClass(
    FdoSmLpObjectPropertyDefinition* pParent, 
	FdoSmLpClassDefinition* pParentType,
	FdoSmLpPropertyMappingDefinition* pMapping,
	FdoSmLpPropertyMappingType mappingType,
    FdoPhysicalClassMapping* pOverrides
)
{
    return (FdoSmLpObjectPropertyClass*) new FdoSmLpGrdObjectPropertyClass(
        pParent, 
        pParentType, 
        pMapping, 
        mappingType, 
        pOverrides
    );
}

FdoSmLpObjectPropertyClassP FdoSmLpGrdObjectPropertyDefinition::NewClass(
   	FdoSmLpObjectPropertyClassP pBase,
	FdoSmLpObjectPropertyDefinition* pParent, 
	FdoSmLpClassDefinition* pParentType,
	FdoSmLpPropertyMappingDefinition* pMapping,
	FdoSmLpPropertyMappingType mappingType, 
    FdoPhysicalClassMapping* pOverrides
)
{
    return (FdoSmLpObjectPropertyClass*) new FdoSmLpGrdObjectPropertyClass(
        pBase,
        pParent, 
        pParentType, 
        pMapping, 
        mappingType, 
        pOverrides
    );
}

void FdoSmLpGrdObjectPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
    FdoRdbmsOvPropertyP pPropOverrides;

    // Get property overrides
    if ( pClassOverrides ) {
        FdoRdbmsOvClassDefinition* pRdbmsOverrides = (FdoRdbmsOvClassDefinition*) pClassOverrides;
        pPropOverrides = FdoRdbmsOvReadOnlyPropertiesP(pRdbmsOverrides->GetProperties())->FindItem( GetName() );
    }

    // Update from property overrides
    Update( pFdoProp, elementState, pPropOverrides, bIgnoreStates );
}

void FdoSmLpGrdObjectPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
    // Do base update first.
    FdoSmLpObjectPropertyDefinition::Update( pFdoProp, elementState, pPropOverrides, bIgnoreStates );
    // Update from Overrides second
    Update( pPropOverrides );
}

void FdoSmLpGrdObjectPropertyDefinition::Update(
    FdoPhysicalPropertyMapping* pPropOverrides
)
{
    FdoRdbmsOvObjectPropertyDefinition* pObjectPropOverrides = 
        dynamic_cast<FdoRdbmsOvObjectPropertyDefinition*>(pPropOverrides);

    // Error if overrides are not for an object property
    if ( pPropOverrides && !pObjectPropOverrides ) 
        AddWrongOverrideTypeError();

    // Extract the physical overrides
    if ( pObjectPropOverrides ) {
        mMappingOverrides = FDO_SAFE_ADDREF(pObjectPropOverrides->GetMappingDefinition());

        FdoRdbmsOvPropertyMappingSingleP singleMapping =
            FDO_SAFE_ADDREF(dynamic_cast<FdoRdbmsOvPropertyMappingSingle*>(mMappingOverrides.p));

        if ( singleMapping ) {
			SetTableMapping( FdoSmLpPropertyMappingType_Single );
        } 
        else {
            FdoRdbmsOvPropertyMappingConcreteP concreteMapping =
                FDO_SAFE_ADDREF(dynamic_cast<FdoRdbmsOvPropertyMappingConcrete*>(mMappingOverrides.p));

            if ( concreteMapping ) {
    			SetTableMapping( FdoSmLpPropertyMappingType_Concrete );
                mOpOvClass = concreteMapping->GetInternalClass();
            }
        }
    }

    // Check if physical overrides specify the table name.
    if ( mOpOvClass ) {
        FdoRdbmsOvTableP tableOverrides = mOpOvClass->GetTable();

        if ( tableOverrides ) {
            mOvTableName = FdoStringP(tableOverrides->GetName());
        }
    }

    // Table name is always the fixed default name if not overridden and
    // this object originated from an FDO Config doc.
    if ( (mOvTableName == L"") && (GetElementState() == FdoSchemaElementState_Unchanged) && GetIsFromFdo() ) {
        mFixedDefaultName = true;
    }

}

FdoStringP FdoSmLpGrdObjectPropertyDefinition::GetOvTableName()
{
    if ( mFixedDefaultName ) 
        return this->GetDefaultDbObjectName();
    else
        return mOvTableName;
}

void FdoSmLpGrdObjectPropertyDefinition::ReadPrefix( 
    FdoSmPhClassPropertyReaderP propReader,
    FdoRdbmsOvObjectPropertyP propOverride
)
{
    FdoStringP prefix = propReader->GetColumnName();

    // Set the single table mapping prefix. If prefix is 'n/a' then this property
    // has concrete mapping
    if ( (prefix.GetLength() > 0) && (prefix.ICompare(L"N/A") != 0) ) {
        FdoRdbmsOvPropertyMappingSingleP pOverrides = propOverride->CreateSingleMapping(false);
        pOverrides->SetPrefix( prefix );
        SetMappingOverrides( FDO_SAFE_ADDREF(pOverrides.p) );

        SetTableMapping( FdoSmLpPropertyMappingType_Single );
    }
}

FdoSmPhDbObjectP FdoSmLpGrdObjectPropertyDefinition::NewTable( 
    FdoSmPhOwnerP owner,
    FdoString* tableName
)
{
    FdoStringP pkeyName;


    FdoSmPhDbObjectP dbObject = FdoSmLpObjectPropertyDefinition::NewTable(owner, tableName );

    // Check if physical overrides specify the primary key name.
    if ( mOpOvClass ) {
        FdoRdbmsOvTableP tableOverrides = mOpOvClass->GetTable();

        if ( tableOverrides ) {
            FdoSmPhTableP table = dbObject->SmartCast<FdoSmPhTable>();
            pkeyName = FdoStringP(tableOverrides->GetPKeyName());
            table->SetPkeyName(pkeyName);
        }
    }

    return dbObject;
}

void FdoSmLpGrdObjectPropertyDefinition::SetSingleMapping()
{
    FdoSmLpPropertyMappingSingleP			    pMapping;
	const FdoSmLpObjectPropertyDefinition*	    pBaseProperty = NULL;
	const FdoSmLpPropertyMappingSingle*	        pBaseMapping = NULL;
    FdoRdbmsOvPropertyMappingSingle*            pMappingOverrides = 
        dynamic_cast<FdoRdbmsOvPropertyMappingSingle*>(mMappingOverrides.p);

	// Check if there is a base property with a mapping definition.
    pBaseProperty = FdoSmLpObjectPropertyDefinition::Cast( this->RefBaseProperty() );
	if ( pBaseProperty ) 
        pBaseMapping = FdoSmLpPropertyMappingSingle::Cast( pBaseProperty->RefMappingDefinition() );

	if ( pBaseMapping ) {
		// There is a base mapping definition so create a copy
		pMapping = NewPropertyMappingSingle( pBaseMapping, pMappingOverrides );
	}
	else {
		// No base mapping definition so create a new one.
		pMapping = NewPropertyMappingSingle( pMappingOverrides );
	}

	SetMappingDefinition( pMapping.p->SmartCast<FdoSmLpPropertyMappingDefinition>() );

}

FdoRdbmsOvObjectPropertyDefinition* FdoSmLpGrdObjectPropertyDefinition::NewMappingOverrides()
{
    return (FdoRdbmsOvObjectPropertyDefinition*) NULL;
}

FdoRdbmsOvPropertyMappingP FdoSmLpGrdObjectPropertyDefinition::GetMappingOverrides()
{
    return mMappingOverrides;
}

void FdoSmLpGrdObjectPropertyDefinition::SetMappingOverrides( FdoRdbmsOvPropertyMappingP mappingOverrides )
{
    mMappingOverrides = mappingOverrides;
}

void FdoSmLpGrdObjectPropertyDefinition::SetConcreteMapping()
{
	FdoSmLpPropertyMappingConcreteP			    pMapping;
	const FdoSmLpObjectPropertyDefinition*	    pBaseProperty = NULL;
	const FdoSmLpPropertyMappingConcrete*	    pBaseMapping = NULL;
    FdoRdbmsOvPropertyMappingConcrete*          pMappingOverrides = 
        dynamic_cast<FdoRdbmsOvPropertyMappingConcrete*>(mMappingOverrides.p);

	// Check if there is a base property with a mapping definition.
	pBaseProperty = FdoSmLpGrdObjectPropertyDefinition::Cast( this->RefBaseProperty() );
	if ( pBaseProperty ) 
		pBaseMapping = FdoSmLpPropertyMappingConcrete::Cast( pBaseProperty->RefMappingDefinition() );

    if ( pBaseMapping ) {
		// There is a base mapping definition so create a copy
		pMapping = NewPropertyMappingConcrete( pBaseMapping, pMappingOverrides );
	}
	else {
		// No base mapping definition so create a new one.
		pMapping = NewPropertyMappingConcrete( pMappingOverrides );
	}

	SetMappingDefinition( pMapping.p->SmartCast<FdoSmLpPropertyMappingDefinition>() );

	// Mapping definition calculates local id property.
	SetIdentityProperty( ((FdoSmLpGrdObjectPropertyClass*) (pMapping->RefTargetClass()))->GetLocalIdProperty() );
	if ( GetIdentityProperty() ) 
		SetIdentityPropertyName( GetIdentityProperty()->GetName() );
}

FdoSmLpPropertyMappingSingleP FdoSmLpGrdObjectPropertyDefinition::NewPropertyMappingSingle(
    const FdoSmLpPropertyMappingSingle* pBaseMapping,
    FdoRdbmsOvPropertyMappingSingle* pMappingOverrides
)
{
    FdoSmLpPropertyMappingSingleP			    pMapping;
    FdoSmLpClassDefinitionP opClass = GetClass();

	pMapping = new FdoSmLpGrdPropertyMappingSingle( pBaseMapping, this, opClass, pMappingOverrides );

    return pMapping;
}

FdoSmLpPropertyMappingSingleP FdoSmLpGrdObjectPropertyDefinition::NewPropertyMappingSingle(
    FdoRdbmsOvPropertyMappingSingle* pMappingOverrides
)
{
    FdoSmLpPropertyMappingSingleP			    pMapping;
    FdoSmLpClassDefinitionP opClass = GetClass();

	pMapping = new FdoSmLpGrdPropertyMappingSingle( this, opClass, pMappingOverrides );

    return pMapping;
}

FdoSmLpPropertyMappingConcreteP FdoSmLpGrdObjectPropertyDefinition::NewPropertyMappingConcrete(
    const FdoSmLpPropertyMappingConcrete* pBaseMapping,
    FdoRdbmsOvPropertyMappingConcrete* pMappingOverrides
)
{
    FdoSmLpPropertyMappingConcreteP			    pMapping;
    FdoSmLpClassDefinitionP opClass = GetClass();

	pMapping = new FdoSmLpGrdPropertyMappingConcrete( pBaseMapping, this, opClass, pMappingOverrides );

    return pMapping;
}

FdoSmLpPropertyMappingConcreteP FdoSmLpGrdObjectPropertyDefinition::NewPropertyMappingConcrete(
    FdoRdbmsOvPropertyMappingConcrete* pMappingOverrides
)
{
    FdoSmLpPropertyMappingConcreteP			    pMapping;
    FdoSmLpClassDefinitionP opClass = GetClass();

	pMapping = new FdoSmLpGrdPropertyMappingConcrete( this, opClass, pMappingOverrides );

    return pMapping;
}

bool FdoSmLpGrdObjectPropertyDefinition::SetSchemaMappings( FdoPhysicalPropertyMappingP propMapping, bool bIncludeDefaults ) const
{
    bool bHasMappings = false;

    // Add the schema mappings for the particular type of mapping definition.
    if ( RefMappingDefinition() ) {
        bHasMappings = RefMappingDefinition()->AddSchemaMappings( propMapping, bIncludeDefaults );
    }

    return bHasMappings;
}

