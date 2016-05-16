#ifndef FDOSMLPMYSQLSCHEMA_H
#define FDOSMLPMYSQLSCHEMA_H      1
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

#include <Sm/Lp/SchemaCollection.h>
#include "../../../SchemaMgr/Lp/Schema.h"
#include <Rdbms/Override/MySQL/MySqlOvStorageEngineType.h>


// MySql Provider's LogicalPhysical schema definition
class FdoSmLpMySqlSchema : public FdoSmLpGrdSchema
{
public:

    // Creates a schema from a schema reader.
    //
    // Parameters:
    //      rdr: get the schema attributes from this schema reader.
    //      physicalSchema: Physical Schema Manager
    //      schemas: containing schema collection for this schema.
    FdoSmLpMySqlSchema(
        FdoSmPhSchemaReaderP rdr,
        FdoSmPhMgrP physicalSchema,
        FdoSmLpSchemaCollection* schemas
    );

    // Creates an instance of a LogicalPhysicalSchema from an FDO Feature Schema.
	//
	// Parameters:
	//		FeatSchema: the FDO schema.
    //      bIgnoreStates: true if the element state on the FDO schema is 
    //          to be ignored.
	//		physicalSchema: Physical Schema Manager. Used to retrieve the schema's 
	//          classes
    FdoSmLpMySqlSchema(
		FdoFeatureSchema* pFeatSchema,
        bool bIgnoreStates,
		FdoSmPhMgrP physicalSchema, 
		FdoSmLpSchemaCollection* schemas
	);
	
    // Generates the physical mappings for this feature schema
    virtual FdoPhysicalSchemaMappingP GetSchemaMappings( bool bIncludeDefaults ) const;
	
    // Copy updates from corresponding FDO feature schema. 
    //
    // Parameters
    //      pFeatSchema: the FDO feature schema.
    //      elementState: the modification state for this feature schema
    //      pPropOverrides: Fdo to physical mapping overrides.
    //      bIgnoreStates: true if the element state on the FDO feature schema is 
    //          to be ignored.
	virtual void Update(
        FdoFeatureSchema* pFeatSchema, 
        FdoSchemaElementState elementState,
        FdoPhysicalSchemaMapping* pOverrides,
        bool bIgnoreStates
    ); 

    // Get the default overrides for tables in this feature schema:
    FdoString* GetTableDataDirectory() const  { return mTableDataDirectory; }
    FdoString* GetIndexDataDirectory() const  { return mIndexDataDirectory; }
    FdoString* GetTableStorageEngine() const  { return mTableStorageEngine; }

    // Set the default overrides for tables in this feature schema:
    void SetTableDataDirectory(FdoString* tableDataDirectory)  { mTableDataDirectory = tableDataDirectory; }
    void SetIndexDataDirectory(FdoString* indexDataDirectory)  { mIndexDataDirectory = indexDataDirectory; }
    void SetTableStorageEngine(FdoString* tableStorageEngine)  { mTableStorageEngine = tableStorageEngine; }

    // Various property definition creator implementors.
    virtual FdoSmLpPropertyP CreateDataProperty( FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent  );
    virtual FdoSmLpPropertyP CreateGeometricProperty( FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent  );
    virtual FdoSmLpPropertyP CreateObjectProperty( FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent  );
    virtual FdoSmLpPropertyP CreateAssociationProperty( FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent  );

    virtual FdoSmLpPropertyP CreateDataProperty(
        FdoDataPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );
    virtual FdoSmLpPropertyP CreateGeometricProperty(
        FdoGeometricPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );
    virtual FdoSmLpPropertyP CreateGeometricProperty(
        FdoGeometricPropertyDefinition* pFdoProp, 
        FdoString * columnNameX,
        FdoString * columnNameY,
        FdoString * columnNameZ,
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );
    virtual FdoSmLpPropertyP CreateObjectProperty(
        FdoObjectPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );
    virtual FdoSmLpPropertyP CreateAssociationProperty(
        FdoAssociationPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );


protected:
    virtual ~FdoSmLpMySqlSchema(void);

    // Get a writer for adding this feature schema to the MetaSchema.
    virtual FdoSmPhSchemaWriterP GetPhysicalAddWriter();

    // Various class definition creator implementors.
    virtual FdoSmLpClassDefinitionP CreateFeatureClass( FdoSmPhClassReaderP classReader );
    virtual FdoSmLpClassDefinitionP CreateClass( FdoSmPhClassReaderP classReader );

    virtual FdoSmLpClassDefinitionP CreateFeatureClass(
        FdoFeatureClass* pFdoClass, 
        bool bIgnoreStates
    );
    virtual FdoSmLpClassDefinitionP CreateClass(
        FdoClass* pFdoClass, 
        bool bIgnoreStates
    );

private:
    FdoStringP                      mTableDataDirectory;
    FdoStringP                      mIndexDataDirectory;
    FdoStringP                      mTableStorageEngine;
};

typedef FdoPtr<FdoSmLpMySqlSchema> FdoSmLpMySqlSchemaP;

#endif
