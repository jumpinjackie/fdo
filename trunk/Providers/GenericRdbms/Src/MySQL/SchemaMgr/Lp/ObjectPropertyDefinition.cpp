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
#include <Sm/Lp/ClassDefinition.h>
#include "ObjectPropertyDefinition.h"
#include "ObjectPropertyClass.h"
#include "../../../SchemaMgr/Lp/PropertyMappingConcrete.h"
#include "../../../SchemaMgr/Lp/PropertyMappingSingle.h"
#include <Rdbms/Override/MySQL/MySqlOvPropertyMappingConcrete.h>
#include <Rdbms/Override/MySQL/MySqlOvPropertyMappingSingle.h>
#include <Rdbms/Override/MySQL/MySqlOvObjectPropertyDefinition.h>
#include <Rdbms/Override/MySQL/MySqlOvTable.h>
#include <Rdbms/Override/MySQL/MySqlOvClassDefinition.h>
#include <Rdbms/Override/MySQL/MySqlOvStorageEngineType.h>
#include "../Ph/Table.h"
#include "Schema.h"

FdoSmLpMySqlObjectPropertyDefinition::FdoSmLpMySqlObjectPropertyDefinition(
    FdoSmPhClassPropertyReaderP propReader, 
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpGrdObjectPropertyDefinition(propReader, parent)
{
    ReadPrefix( propReader, NewMappingOverrides() );
}

FdoSmLpMySqlObjectPropertyDefinition::FdoSmLpMySqlObjectPropertyDefinition(
    FdoObjectPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpGrdObjectPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
}

FdoSmLpMySqlObjectPropertyDefinition::FdoSmLpMySqlObjectPropertyDefinition(
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

FdoSmLpMySqlObjectPropertyDefinition::~FdoSmLpMySqlObjectPropertyDefinition(void)
{
}

bool FdoSmLpMySqlObjectPropertyDefinition::AddSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    bool bHasMappings = false;
    FdoMySQLOvClassDefinition* mqlClassMapping = (FdoMySQLOvClassDefinition*)(FdoPhysicalClassMapping*) classMapping;  

    // Get the mappings for this property
    FdoPhysicalPropertyMappingP propMapping = GetSchemaMappings( bIncludeDefaults );
    FdoMySQLOvPropertyDefinition* mqlPropMapping = 
        dynamic_cast<FdoMySQLOvPropertyDefinition*>(propMapping.p);

    // Add them to the given class.
    if ( mqlPropMapping ) {
        FdoMySQLOvPropertiesP(mqlClassMapping->GetProperties())->Add( mqlPropMapping );
        bHasMappings = true;
    }

    return bHasMappings;
}

FdoPhysicalPropertyMappingP FdoSmLpMySqlObjectPropertyDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    FdoMySQLOvObjectPropertyDefinition* propMapping = FdoMySQLOvObjectPropertyDefinition::Create( GetName() );
    FdoPhysicalPropertyMappingP retSchemaMapping = (FdoPhysicalPropertyMapping*) propMapping;

    ((FdoSmLpMySqlObjectPropertyDefinition*)this)->Finalize();
    bool bHasMappings = SetSchemaMappings( retSchemaMapping, bIncludeDefaults );

    // Return NULL if no mappings.
    if ( !bHasMappings ) 
        retSchemaMapping = NULL;

    return retSchemaMapping;
}

FdoSmLpObjectPropertyClassP FdoSmLpMySqlObjectPropertyDefinition::NewClass(
    FdoSmLpObjectPropertyDefinition* pParent, 
	FdoSmLpClassDefinition* pParentType,
	FdoSmLpPropertyMappingDefinition* pMapping,
	FdoSmLpPropertyMappingType mappingType,
    FdoPhysicalClassMapping* pOverrides
)
{
    return (FdoSmLpObjectPropertyClass*) new FdoSmLpMySqlObjectPropertyClass(
        pParent, 
        pParentType, 
        pMapping, 
        mappingType, 
        pOverrides
    );
}

FdoSmLpObjectPropertyClassP FdoSmLpMySqlObjectPropertyDefinition::NewClass(
   	FdoSmLpObjectPropertyClassP pBase,
	FdoSmLpObjectPropertyDefinition* pParent, 
	FdoSmLpClassDefinition* pParentType,
	FdoSmLpPropertyMappingDefinition* pMapping,
	FdoSmLpPropertyMappingType mappingType, 
    FdoPhysicalClassMapping* pOverrides
)
{
    return (FdoSmLpObjectPropertyClass*) new FdoSmLpMySqlObjectPropertyClass(
        pBase,
        pParent, 
        pParentType, 
        pMapping, 
        mappingType, 
        pOverrides
    );
}

void FdoSmLpMySqlObjectPropertyDefinition::FinalizeTableOverrides()
{
}

FdoSmLpPropertyP FdoSmLpMySqlObjectPropertyDefinition::NewInherited( FdoSmLpClassDefinition* pSubClass ) const
{
    return new FdoSmLpMySqlObjectPropertyDefinition( FDO_SAFE_ADDREF((FdoSmLpObjectPropertyDefinition*) this), pSubClass, L"", L"", true );
}

FdoSmLpPropertyP FdoSmLpMySqlObjectPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* pPropOverrides
) const
{
    return
        new FdoSmLpMySqlObjectPropertyDefinition(
            FDO_SAFE_ADDREF((FdoSmLpObjectPropertyDefinition*) this),
            pTargetClass,
            logicalName,
            physicalName,
            false,
            pPropOverrides
        );
}

FdoRdbmsOvObjectPropertyDefinition* FdoSmLpMySqlObjectPropertyDefinition::NewMappingOverrides()
{
    return (FdoRdbmsOvObjectPropertyDefinition*) FdoMySQLOvObjectPropertyDefinition::Create( GetName() );
}

FdoSmPhDbObjectP FdoSmLpMySqlObjectPropertyDefinition::NewTable( 
    FdoSmPhOwnerP owner,
    FdoString* tableName
)
{
    // Let superclass do its job:
	FdoSmPhDbObjectP phDbObject = FdoSmLpGrdObjectPropertyDefinition::NewTable(owner, tableName);

    // Pass MySql-specific overrides to physical object:
	FdoSmPhMySqlTable* phMySqlTable = dynamic_cast<FdoSmPhMySqlTable*>(phDbObject.p);
	const FdoSmLpMySqlClassDefinition* lpMySqlClass = dynamic_cast<const FdoSmLpMySqlClassDefinition*>(RefTargetClass());
	if (lpMySqlClass)
		((FdoSmLpMySqlClassDefinition*)lpMySqlClass)->GetOverridesFromClass(phMySqlTable);

    // Return it:
    return phDbObject;
}
