#ifndef FDOSMLPSCHEMA_H
#define FDOSMLPSCHEMA_H		1
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

#include <Sm/Lp/SchemaElement.h>
#include <Sm/Lp/ClassCollection.h>
#include <Sm/Lp/Class.h>
#include <Sm/Lp/FeatureClass.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/SchemaReader.h>
#include <Sm/Ph/ClassReader.h>
#include <Sm/Ph/SchemaWriter.h>
#include <Sm/Lp/QClassCollection.h>

class FdoSmLpSchemaCollection;

// Root object for a LogicalPhysical schema. A LogicalPhysical schema consists
// of a Feature Schema plus info on how the Feature Schema elements 
// correspond to elements in the Physical Schema.
class FdoSmLpSchema : public FdoSmLpSchemaElement
{
public:

    virtual ~FdoSmLpSchema(void);

    /// Returns a ClassCollection that contains all of the classes defined in
    /// the LogicalPhysical schema.
    const FdoSmLpClassCollection* RefClasses() const;

    /// Get the Physical Schema object for accessing the Database
    /// schema.
    const FdoSmPhMgr* RefPhysicalSchema() const;

    /// Get a read-only Physical Schema manager for accessing and updating
    /// the Database schema.
    FdoSmPhMgrP GetPhysicalSchema();

    /// Get the collection that contains this schema.
    const FdoSmLpSchemaCollection* RefSchemas() const;
    FdoSmLpSchemaCollection* GetSchemas();

    /// Get the Schema Attribute Dictionary.
    virtual const FdoSmLpSAD* RefSAD() const;

    /// Get the Schema Mappings for this feature schema
    ///
    /// Parameters:    
    ///      bIncludeDefaults - 
    ///          true: include all schema mappings.
    ///          false: include only non-default mappings.
    /// Returns:
    ///      The Schema Mappings.
    ///      NULL if there are no mappings.

    virtual FdoPhysicalSchemaMappingP GetSchemaMappings( bool bIncludeDefaults ) const;

    /// Get the default database link for tables in this feature schema
    FdoString* GetDatabase() const
    {
        return mDatabase;
    }

    /// Get the default owner (Oracle schema) for tables in this feature schema
    FdoString* GetOwner() const
    {
        return mOwner;
    }

    /// Get the default class-table mapping type for this feature schema.
    FdoSmOvTableMappingType GetTableMapping() const
    {
        return mTableMapping;
    }

    /// Find a class.
    ///
    /// Parameters:
    /// 	className: the class to find. When this is a qualified name
    /// 		( [<schemaname]:[classname] ) then this schema passes it
    /// 		on to the indicated schema. 
    const FdoSmLpClassDefinition* FindClass( FdoStringP className ) const;

    /// Given a table, retrieves the list of classes for which this table
    /// is the class table.
    ///
    /// Parameters:
    ///      class: add the found classes to this collection.
    /// 	tableName: the table to check
    /// 	ownerName: the datastore where table resides,
    ///          default is current datastore.
    /// 	databaseName: the database where table resides,
    ///          default is current database.

    void TableToClasses( 
        FdoSmLpQClassesP& classes,
        FdoStringP tableName, 
        FdoStringP ownerName = L"",
        FdoStringP databaseName = L""
    ) const;    

    /// Copy updates from corresponding FDO feature schema. 
    ///
    /// Parameters
    ///      pFeatSchema: the FDO feature schema.
    ///      elementState: the modification state for this feature schema
    ///      pOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO feature schema is 
    ///          to be ignored.
    virtual void Update(
        FdoFeatureSchema* pFeatSchema, 
        FdoSchemaElementState elementState,
        FdoPhysicalSchemaMapping* pOverrides,
        bool bIgnoreStates
    ); 

    /// Synchronize the class table and columns with the classes in this schema
    ///
    /// Parameters:
    //
    ///      bRollbackOnly - 
    ///          true - synchronize only classes that have had changes rolled back since
    ///              the previous synchronization.
    ///          false - synchronize all classes.
    virtual void SynchPhysical(bool bRollbackOnly = true);

    /// Set the modification state of this element
    virtual void SetElementState(FdoSchemaElementState elementState);

    /// Post outstanding modifications to the current database.
    virtual void Commit( bool fromParent = false );

    /// Constructs an instance of a Class Definition from a class reader.
    ///
    /// Parameters:
    /// 	classReader: reader to get Class attributes from.
    /// 	parent: the parent element ( usually an FdoSmLpSchema ).
    FdoSmLpClassDefinitionP CreateClassDefinition( FdoSmPhClassReaderP classReader );

    /// Constructs an instance of a Class Definition from an FDO
    /// class definition
    ///
    /// Parameters:
    /// 	pFdoClass: the FDO class
    /// 	pClassOverrides: schema overrides for the class
    ///      bIgnoreStates: true if the element state on the FDO feature class is 
    ///          to be ignored.
    FdoSmLpClassDefinitionP CreateClassDefinition(
        FdoClassDefinition* pFdoClass, 
        FdoPhysicalSchemaMapping* pOverrides,        
        bool bIgnoreStates,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
    ///
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
    virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Functions for creating various types of properties from a property reader.
    /// Each provider must supply implementations.

    /// Create from property reader
    virtual FdoSmLpPropertyP CreateDataProperty( FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent ) = 0;
    virtual FdoSmLpPropertyP CreateGeometricProperty( FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent ) = 0;
    virtual FdoSmLpPropertyP CreateObjectProperty( FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent ) = 0;
    virtual FdoSmLpPropertyP CreateAssociationProperty( FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent ) = 0;

    /// create from FDO property definition
    virtual FdoSmLpPropertyP CreateDataProperty(
        FdoDataPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    ) = 0;
    virtual FdoSmLpPropertyP CreateGeometricProperty(
        FdoGeometricPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    ) = 0;
    virtual FdoSmLpPropertyP CreateGeometricProperty(
        FdoGeometricPropertyDefinition* pFdoProp, 
        FdoString * columnNameX,
        FdoString * columnNameY,
        FdoString * columnNameZ,
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    ) = 0;
    virtual FdoSmLpPropertyP CreateObjectProperty(
        FdoObjectPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    ) = 0;
    virtual FdoSmLpPropertyP CreateAssociationProperty(
        FdoAssociationPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    ) = 0;

    /// Serialize this schema to an XML file.
    /// Mainly for unit testing.
    virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

    const FdoSmLpClassDefinition* RefClass(FdoStringP className) const;


protected:
    /// Loads an instance of a LogicalPhysicalSchema from the FDO
    /// database.
    ///
    /// Parameters:
    /// 	rdr: get the schema attributes from this schema reader.
    /// 	physicalSchema: Physical Schema Manager. Used to retrieve the schema's 
    ///          classes.
    FdoSmLpSchema(
        FdoSmPhSchemaReaderP rdr, 
        FdoSmPhMgrP physicalSchema, 
        FdoSmLpSchemaCollection* schemas
    );

    /// Creates an instance of a LogicalPhysicalSchema from an FDO Feature Schema.
    ///
    /// Parameters:
    /// 	FeatSchema: the FDO schema.
    ///      pOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO schema is 
    ///          to be ignored.
    /// 	physicalSchema: Physical Schema Manager. Used to retrieve the schema's 
    ///          classes.
    FdoSmLpSchema(
        FdoFeatureSchema* pFeatSchema,
        bool bIgnoreStates,
        FdoSmPhMgrP physicalSchema, 
        FdoSmLpSchemaCollection* schemas
    );
	
    /// Gets a writer for adding this feature schema to the MetaSchema
    virtual FdoSmPhSchemaWriterP GetPhysicalAddWriter();

    /// Functions for creating various types of classes from a class reader.
    /// Each provider must supply implementations.
    virtual FdoSmLpClassDefinitionP CreateFeatureClass( FdoSmPhClassReaderP classReader ) = 0;
    virtual FdoSmLpClassDefinitionP CreateClass( FdoSmPhClassReaderP classReader ) = 0;

    virtual FdoSmLpClassDefinitionP CreateFeatureClass(
        FdoFeatureClass* pFdoClass, 
        bool bIgnoreStates
    ) = 0;
    virtual FdoSmLpClassDefinitionP CreateClass(
        FdoClass* pFdoClass, 
        bool bIgnoreStates
    ) = 0;

    /// Set the default database for class tables.
    void SetDatabase( FdoString* database )
    {
        mDatabase = database;
    }

    /// Set the default owning schema for class tables.
    void SetOwner( FdoString* owner )
    {
        mOwner = owner;
    }

    /// Set the default table mapping type for classes
    void SetTableMapping( FdoSmOvTableMappingType tableMapping )
    {
        mTableMapping = tableMapping;
    }

protected:
    FdoSmLpSchema();
    /// Loads the classes and SAD for this schema

private:
    void LoadSchema();
    /// Loads the classes for this schema
    void LoadClasses();

    FdoSmLpClassDefinitionP LoadClass(FdoStringP className, FdoString* schemaName=NULL);

    /// Destroys classes, relations and SAD.
    /// Just the in-memory objects, they are not removed
    /// from the current database.
    void DeleteSchema();

    /// Various error logging functions.
    void AddClassExistsError( FdoClassDefinition* pFdoClass );
    void AddClassNotExistsError( FdoString* className );

    FdoSmLpClassesP                mClasses;

    /// non-refcounted backpointer to schema collection.

    FdoStringP                      mDatabase;
    FdoStringP                      mOwner;
    FdoSmLpSchemaCollection*        mpSchemas;
    FdoSmPhMgrP                     mPhysicalSchema;

    FdoSmOvTableMappingType         mTableMapping;

    bool                            mbSchemaLoaded;

};

typedef FdoPtr<FdoSmLpSchema> FdoSmLpSchemaP;

#endif


