/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include <Sm/Lp/ClassDefinition.h>
#include "ObjectPropertyDefinition.h"
#include "ObjectPropertyClass.h"
#include "../../../SchemaMgr/Lp/PropertyMappingConcrete.h"
#include "../../../SchemaMgr/Lp/PropertyMappingSingle.h"
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvClassDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvTextInRowOption.h>
#include "../Ph/Table.h"
#include "Schema.h"


FdoSmLpSqsObjectPropertyDefinition::FdoSmLpSqsObjectPropertyDefinition(
    FdoSmPhClassPropertyReaderP propReader, 
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpGrdObjectPropertyDefinition(propReader, parent)
{
    ReadPrefix( propReader, NewMappingOverrides() );
}

FdoSmLpSqsObjectPropertyDefinition::FdoSmLpSqsObjectPropertyDefinition(
    FdoObjectPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpGrdObjectPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
}

FdoSmLpSqsObjectPropertyDefinition::FdoSmLpSqsObjectPropertyDefinition(
    FdoPtr<FdoSmLpObjectPropertyDefinition> pBaseProperty, 
    FdoSmLpClassDefinition* pTargetClass, 
    FdoStringP logicalName, 
    FdoStringP physicalName, 
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
	FdoSmLpGrdObjectPropertyDefinition(
        pBaseProperty, 
        pTargetClass, 
        logicalName, 
        physicalName, 
        bInherit,
        pPropOverrides
    )
{
}

FdoSmLpSqsObjectPropertyDefinition::~FdoSmLpSqsObjectPropertyDefinition(void)
{
}

bool FdoSmLpSqsObjectPropertyDefinition::AddSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    bool bHasMappings = false;
    FdoSqlServerOvClassDefinition* sqsClassMapping = (FdoSqlServerOvClassDefinition*)(FdoPhysicalClassMapping*) classMapping;  

    // Get the mappings for this property
    FdoPhysicalPropertyMappingP propMapping = GetSchemaMappings( bIncludeDefaults );
    FdoSqlServerOvPropertyDefinition* sqsPropMapping = 
        dynamic_cast<FdoSqlServerOvPropertyDefinition*>(propMapping.p);

    // Add them to the given class.
    if ( sqsPropMapping ) {
        FdoSqlServerOvPropertiesP(sqsClassMapping->GetProperties())->Add( sqsPropMapping );
        bHasMappings = true;
    }

    return bHasMappings;
}

FdoPhysicalPropertyMappingP FdoSmLpSqsObjectPropertyDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    FdoSqlServerOvObjectPropertyDefinition* propMapping = FdoSqlServerOvObjectPropertyDefinition::Create( GetName() );
    FdoPhysicalPropertyMappingP retSchemaMapping = (FdoPhysicalPropertyMapping*) propMapping;

    ((FdoSmLpSqsObjectPropertyDefinition*)this)->Finalize();
    bool bHasMappings = SetSchemaMappings( retSchemaMapping, bIncludeDefaults );

    // Return NULL if no mappings.
    if ( !bHasMappings ) 
        retSchemaMapping = NULL;

    return retSchemaMapping;
}

FdoSmLpObjectPropertyClassP FdoSmLpSqsObjectPropertyDefinition::NewClass(
    FdoSmLpObjectPropertyDefinition* pParent, 
	FdoSmLpClassDefinition* pParentType,
	FdoSmLpPropertyMappingDefinition* pMapping,
	FdoSmLpPropertyMappingType mappingType,
    FdoPhysicalClassMapping* pOverrides
)
{
    return (FdoSmLpObjectPropertyClass*) new FdoSmLpSqsObjectPropertyClass(
        pParent, 
        pParentType, 
        pMapping, 
        mappingType, 
        pOverrides
    );
}

FdoSmLpObjectPropertyClassP FdoSmLpSqsObjectPropertyDefinition::NewClass(
   	FdoSmLpObjectPropertyClassP pBase,
	FdoSmLpObjectPropertyDefinition* pParent, 
	FdoSmLpClassDefinition* pParentType,
	FdoSmLpPropertyMappingDefinition* pMapping,
	FdoSmLpPropertyMappingType mappingType, 
    FdoPhysicalClassMapping* pOverrides
)
{
    return (FdoSmLpObjectPropertyClass*) new FdoSmLpSqsObjectPropertyClass(
        pBase,
        pParent, 
        pParentType, 
        pMapping, 
        mappingType, 
        pOverrides
    );
}


void FdoSmLpSqsObjectPropertyDefinition::FinalizeTableOverrides()
{
/* TODO
    FdoSmLpGrdObjectPropertyDefinition::FinalizeTableOverrides();

    // Check if physical overrides specify the table name, primary key, or tablespace.
    if ( mOpOvClass ) {
        FdoSqlServerOvTableP tableOverrides = mOpOvClass->GetTable();
        if ( tableOverrides && (wcslen(tableOverrides->GetName()) > 0) ) {
            targetTablespace = FdoStringP(tableOverrides->RefTablespace()).Upper();
        }
   }
*/
}

FdoSmLpPropertyP FdoSmLpSqsObjectPropertyDefinition::NewInherited( FdoSmLpClassDefinition* pSubClass ) const
{
    return new FdoSmLpSqsObjectPropertyDefinition( FDO_SAFE_ADDREF((FdoSmLpObjectPropertyDefinition*) this), pSubClass, L"", L"", true );
}

FdoSmLpPropertyP FdoSmLpSqsObjectPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* pPropOverrides
) const
{
    return
        new FdoSmLpSqsObjectPropertyDefinition(
            FDO_SAFE_ADDREF((FdoSmLpObjectPropertyDefinition*) this),
            pTargetClass,
            logicalName,
            physicalName,
            false,
            pPropOverrides
        );
}

FdoRdbmsOvObjectPropertyDefinition* FdoSmLpSqsObjectPropertyDefinition::NewMappingOverrides()
{
    return (FdoRdbmsOvObjectPropertyDefinition*) FdoSqlServerOvObjectPropertyDefinition::Create( GetName() );
}

FdoSmPhDbObjectP FdoSmLpSqsObjectPropertyDefinition::NewTable( 
    FdoSmPhOwnerP owner,
    FdoString* tableName
)
{
    // Let superclass do its job:
	FdoSmPhDbObjectP phDbObject = FdoSmLpGrdObjectPropertyDefinition::NewTable(owner, tableName);

    // Pass Sqs-specific overrides to physical object:
	FdoSmPhSqsTable* phSqsTable = dynamic_cast<FdoSmPhSqsTable*>(phDbObject.p);
	const FdoSmLpSqsClassDefinition* lpSqsClass = dynamic_cast<const FdoSmLpSqsClassDefinition*>(RefTargetClass());
	if (lpSqsClass)
		((FdoSmLpSqsClassDefinition*)lpSqsClass)->GetOverridesFromClass(phSqsTable);

    // Return it:
    return phDbObject;
}
