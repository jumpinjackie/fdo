#ifndef FDOSMLPSQSCLASSDEFINITION_H
#define FDOSMLPSQSCLASSDEFINITION_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include "../../../SchemaMgr/Lp/ClassDefinition.h"
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPhysicalSchemaMapping.h>
#include <Fdo/Commands/Schema/PhysicalClassMapping.h>
#include "../Ph/Table.h"

class FdoSmLpSqsClassDefinition : public FdoSmLpGrdClassDefinition
{
public:
    virtual ~FdoSmLpSqsClassDefinition(void);

	//Update from class overrides
    //
    // Parameters:
    //      pFdoClass: the FDO class.
    //      elementState: the modification state for this class definition.
    //      pClassOverrides: Fdo to physical mapping overrides.
    //      bIgnoreStates: true if the element state on the FDO class is 
    //          to be ignored.

    virtual void Update(
        FdoClassDefinition* pFdoClass,
        FdoSchemaElementState elementState,
        FdoPhysicalClassMapping* pClassOverrides,
        bool bIgnoreStates
    );

	// Copy overrides from this class to given table:
	void GetOverridesFromClass(FdoSmPhSqsTable* phTable);

	// Simple accessor methods to get overrides:
	FdoStringP  GetTableIndexFilegroup() const { return mTableIndexFilegroup; };
    FdoStringP  GetTableFilegroup() const { return mTableFilegroup; };
    FdoStringP  GetTableTextFilegroup() const { return mTableTextFilegroup; };
    SqlServerOvTextInRowOption  GetTextInRow() const { return mTextInRow; };
    FdoStringP  GetIdentityPropertyName() const { return mIdentityPropertyName; };
    FdoInt32    GetIdentityIncrement() const { return mIdentityIncrement; };
    FdoInt32    GetIdentitySeed() const { return mIdentitySeed; };
    bool        GetIdentityIsGloballyUnique() const { return mIdentityIsGloballyUnique; };

    // The following return versions of the db object or root object names
    // that are good for building other names such as for object property
    // tables and columns. 
    // The db object or root name minus the user name part is returned.
    virtual FdoStringP  GetSubstDbObjectName() const;
    virtual FdoStringP  GetSubstRootDbObjectName() const;

protected:
    // Constructs an instance of a ClassDefinition from a class reader
    FdoSmLpSqsClassDefinition(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent);

    // Constructs an instance of a ClassDefinition from an FDO element
    // and Fdo to physical mapping overrides.
    //
    // Parameters:
	//		pFdoClass: the FDO class.
    //      bIgnoreStates: true if the element state on the FDO class is 
    //          to be ignored.
	//		parent: the parent element ( usually an FdoSmLpSchema ).
    FdoSmLpSqsClassDefinition(
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

	FdoSmLpSqsClassDefinition(
        FdoString* name,
        FdoString* description,
        FdoSmLpSchemaElement* parent,
        FdoPtr<FdoSmLpClassDefinition> pSrcClass,
        FdoSmOvTableMappingType tableMapping,
        FdoString* tableName,
        FdoSmPhDbObjectP table,
        FdoSchemaElementState elementState
    );

    // Add the physical mappings for this class to the given schema override set
    virtual bool AddSchemaMappings( FdoPhysicalSchemaMappingP schemaMapping, bool bIncludeDefaults ) const;

    // Generate the physical mappings for this class
    virtual FdoPhysicalClassMappingP GetSchemaMappings( bool bIncludeDefaults ) const;

    // Sets the SQL Server-specific physical mappings.
    virtual bool SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const;

    // Get a writer for adding this class to the MetaSchema
    virtual void SetPhysicalAddWriter( FdoSmPhClassWriterP writer );

    // Sets up SqlServer-specific overrides to be written to the 
    // f_schemaoptions table.
    virtual void SetPhysicalOptionsWriter( FdoSmPhClassWriterP classWriter );

    // Common initializer for all constructors:
    void Init();

    // Set class table name. Defaults the user to "dbo", by prepending
    // to object name, if object name does not contain a user name.
    virtual void SetDbObjectName( FdoStringP objectName );

    // Create a new physical table object:
    virtual FdoSmPhDbObjectP NewTable(FdoString* tableName, FdoString* pkeyName);

    virtual void PostFinalize();

private:

    FdoStringP  mTableIndexFilegroup;
    FdoStringP  mTableFilegroup;
    FdoStringP  mTableTextFilegroup;
    SqlServerOvTextInRowOption  mTextInRow;
    FdoStringP  mIdentityPropertyName;
    FdoInt32    mIdentityIncrement;
    FdoInt32    mIdentitySeed;
    bool        mIdentityIsGloballyUnique;
};

typedef FdoPtr<FdoSmLpSqsClassDefinition> FdoSmLpSqsClassDefinitionP;

#endif
