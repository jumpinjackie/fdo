#ifndef FDOSMLPGRDCLASSDEFINITION_H
#define FDOSMLPGRDCLASSDEFINITION_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/ClassDefinition.h>
#include <Rdbms/Override/RdbmsOvClassDefinition.h>
#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>
#include "Utilities/SchemaMgr/Overrides/TableMappingTypeMapper.h"

class FdoSmLpGrdClassDefinition : public virtual FdoSmLpClassBase
{
public:
    virtual ~FdoSmLpGrdClassDefinition(void);

	//Update from generic schema override set
    //
    // Parameters:
    //      pFdoClass: the FDO class.
    //      elementState: the modification state for this class definition.
    //      pOverrides: Fdo to physical mapping overrides.
    //      bIgnoreStates: true if the element state on the FDO class is 
    //          to be ignored.
	virtual void Update(
        FdoClassDefinition* pFdoClass,
        FdoSchemaElementState elementState,
        FdoPhysicalSchemaMapping* pOverrides,
        bool bIgnoreStates
    ); 

	//Update from generic class overrides
    //
    // Parameters:
    //      pFdoClass: the FDO class.
    //      elementState: the modification state for this class definition.
    //      pOverrides: Fdo to physical mapping overrides.
    //      bIgnoreStates: true if the element state on the FDO class is 
    //          to be ignored.
    virtual void Update(
        FdoClassDefinition* pFdoClass,
        FdoSchemaElementState elementState,
        FdoPhysicalClassMapping* pClassOverrides,
        bool bIgnoreStates
    );

protected:
    // unused constructor needed only to build Linux
    FdoSmLpGrdClassDefinition() {}

    // Constructs an instance of a ClassDefinition from a class reader
    FdoSmLpGrdClassDefinition(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent);

    // Constructs an instance of a ClassDefinition from an FDO element
    // and Fdo to physical mapping overrides.
    //
    // Parameters:
	//		pFdoClass: the FDO class.
    //      pClassOverrides: Fdo to physical mapping overrides.
    //      bIgnoreStates: true if the element state on the FDO class is 
    //          to be ignored.
	//		parent: the parent element ( usually an FdoSmLpSchema ).
    FdoSmLpGrdClassDefinition(
        FdoClassDefinition* pFdoClass, 
        bool bIgnoreStates,
        FdoSmLpSchemaElement* parent
    );

	// Constructs an instance of a ClassDefinition from the given attributes.
	//
	// Parameters:
	//		name: class name
	//		description: class description
	//		parent: class objects parent ( usually an FdoSmLpSchema object )
	//		pSrcClass: When this is an object property class, pSrcClass represents the 
	//			object property class for the associated object property's base property.
	//		tableMapping: property to table mapping type
	//		tableName: class table name
	//		table: class table. Null if class has no table or table
	//			does not exist.

	FdoSmLpGrdClassDefinition(
        FdoString* name,
        FdoString* description,
        FdoSmLpSchemaElement* parent,
        FdoPtr<FdoSmLpClassDefinition> pSrcClass,
        FdoSmOvTableMappingType tableMapping,
        FdoString* tableName,
        FdoSmPhDbObjectP table,
        FdoSchemaElementState elementState
    );

	// Sets the generic RDBMS physical mappings from this class to the
    // given class mapping
	//
	// Parameters:
	//		classMapping: add the physical mappings to this class mapping.
	//		bIncludeDefaults - 
    //          true: include all physical mappings.
    //          false: include only non-default mappings.
    // Returns:
    //      true: if mappings were added to classMappings
    //      false: no mappings were added.

    virtual bool SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const;

    // Add generic physical overrides to write to MetaSchema
    virtual void SetPhysicalAddWriter( FdoSmPhClassWriterP writer );

    // Handles updates from generic parts of table overrides
    virtual void UpdateTable( 
        FdoStringP database,            // database instance name, L"" if current instance
        FdoStringP owner,               // owner (datastore) name, L"" if current owner
        FdoRdbmsOvTable* tableOverrides // table overrides (table name and primary key name).
    );

    // Checks if the class LT or Locking modes are FDO and sets the
    // class db object's lt and locking mode settings accordingly.
    virtual void PostFinalize();

    // Adds LtId to the primary key when this class has the LtId property.
    virtual void NewPkey( FdoSmPhTableP table );

};

typedef FdoPtr<FdoSmLpGrdClassDefinition> FdoSmLpGrdClassDefinitionP;

#endif




