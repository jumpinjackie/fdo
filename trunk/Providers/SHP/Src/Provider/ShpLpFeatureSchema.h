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

#ifndef SHPLPFEATURESCHEMA_H_
#define SHPLPFEATURESCHEMA_H_     1

#ifdef _WIN32
#pragma once
#endif

#include <Fdo/Commands/Schema/PhysicalSchemaMapping.h>
#include <SHP/Override/FdoShpPhysicalSchemaMapping.h>
#include <ShpPhysicalSchema.h>
#include <ShpConnection.h>
#include <ShpLpClassDefinition.h>
#include <SHP/Override/FdoShpClassDefinition.h>

class ShpConnection;
class ShpLpFeatureSchemaCollection;

// A bridge between logical and physical schema for FdoFeatureSchema.
class ShpLpFeatureSchema
    : public FdoDisposable
{
public:
    ShpLpFeatureSchema(void); // 0-argument constructor to please FdoPtr::operator-> 

    ShpLpFeatureSchema(
        ShpLpFeatureSchemaCollection * parentLpSchemas,
        ShpConnection * connection,
        ShpPhysicalSchema* physicalSchema,
		FdoFeatureSchema * configLogicalSchema,
		FdoShpOvPhysicalSchemaMapping * configSchemaMapping,
        bool ignore_states = false);

    // For FdoNamedCollection:
    FdoString * GetName() const;
    bool CanSetName() const;

    // Returns the FDO schema object.
    FdoFeatureSchema * GetLogicalSchema() const;

    // Returns a collection of Logical/Physical classes in this schema.
    ShpLpClassDefinitionCollection* GetLpClasses();

    // Returns the physical schema object for this schema.
    ShpPhysicalSchema* GetPhysicalSchema() const;

    //  Modify the schema according to given information:
    void Modify(ShpConnection * connection, FdoFeatureSchema* logicalSchema, FdoShpOvPhysicalSchemaMapping* schemaMapping, bool ignore_states);
    void add_class (ShpConnection * connection, FdoClassDefinition* cls, FdoShpOvClassDefinition* classMapping);
    void delete_class (ShpConnection * connection, FdoClassDefinition* cls);
    void modify_class (ShpConnection * connection, FdoClassDefinition* cls, FdoShpOvClassDefinition* classMapping);

    // Permanently delete the Logical/LogicalPhysical/Physical counterparts to this schema, and all the classes it contains:
    void Delete(ShpConnection * connection);

    // Generate a new copy of the schema mappings for the feature schema, optionally including default mappings
    FdoShpOvPhysicalSchemaMapping* GetSchemaMappings(bool bIncludeDefaults);

protected:
    // Convert the physical schema to a logical schema:
    void ConvertPhysicalToLogical(
		ShpConnection * connection,
		FdoFeatureSchema * configLogicalSchema,
	    FdoShpOvPhysicalSchemaMapping * configSchemaMapping);

    // Convert the Logical schema to a Physical schema:
    void ConvertLogicalToPhysical(
    	ShpConnection * connection,
    	FdoFeatureSchema * configLogicalSchema,
    	FdoShpOvPhysicalSchemaMapping * configSchemaMapping,
        bool ignore_states);


    virtual ~ShpLpFeatureSchema(void);

private:
    ShpLpFeatureSchemaCollection*           m_parentLpSchemas; // weak reference
    FdoPtr<ShpPhysicalSchema>               m_physicalSchema;  // Physical (SHP) schema
    FdoPtr<ShpLpClassDefinitionCollection>  m_lpClasses;       // Logical/Physical classes
    FdoPtr<FdoFeatureSchema>                m_logicalSchema;   // Logical (FDO) schema
};


class ShpLpFeatureSchemaCollection : public FdoNamedCollection<ShpLpFeatureSchema, FdoException>
{
public:
    ShpLpFeatureSchemaCollection(void);

    ShpLpFeatureSchemaCollection(
        ShpConnection * connection,
        ShpPhysicalSchema * physicalSchema,
		FdoFeatureSchemaCollection * configLogicalSchemas,
		FdoPhysicalSchemaMappingCollection * configSchemaMappings);

    // Return pointer to cached logical schema collection:
    FdoFeatureSchemaCollection* GetLogicalSchemas();

protected:
    // Convert physical to logical schema collection:
    void ConvertPhysicalToLogical(ShpConnection* connection, ShpPhysicalSchema* physicalSchema,
        FdoFeatureSchemaCollection* configLogicalSchemas, FdoPhysicalSchemaMappingCollection* configSchemaMappings);

    // Convert logical to physical schema collection:
    void ConvertLogicalToPhysical (ShpConnection* connection,
        FdoFeatureSchemaCollection* configLogicalSchemas, FdoPhysicalSchemaMappingCollection* configSchemaMappings);

    // Destructor:
    virtual ~ShpLpFeatureSchemaCollection(void) {}

    /// <summary>Dispose this object instance.</summary>
    /// <returns>Returns nothing</returns>
    virtual void Dispose() 
    { 
        delete this; 
    }

private:
    FdoPtr<FdoFeatureSchemaCollection> m_logicalSchemas;
};

typedef FdoPtr<ShpLpFeatureSchemaCollection> ShpLpFeatureSchemaCollectionP;


#endif // SHPLPFEATURESCHEMA_H_

