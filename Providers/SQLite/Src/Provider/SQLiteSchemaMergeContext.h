// 
//  
//  Copyright (C) 2009 Autodesk Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#ifndef SQLITE_SCHEMA_MERGECONTEXT_H
#define SQLITE_SCHEMA_MERGECONTEXT_H

class SltConnection;
class TableReformatter;
class FdoSchemaMergeContext;

class SQLiteSchemaMergeContext : public FdoSchemaMergeContext
{
public:
    /// Constructs a context for merging a schema, passed to the ApplySchema command,
    /// into the current schemas
    static SQLiteSchemaMergeContext* Create( 
        SltConnection* connection,              // current connection
        FdoFeatureSchemaCollection* oldSchemas, // current schemas
        FdoFeatureSchema* newSchema,            // applied schema.
        bool ignoreStates                       // see FdoIApplySchema
    );

    // Schema element Modifications supported by the SQLite Provider
    virtual bool CanModElementDescription(FdoSchemaElement* element) { return true; }

    virtual bool CanAddSchema(FdoFeatureSchema* schema) { return false; }
    virtual bool CanDeleteSchema(FdoFeatureSchema* schema) { return false; }
    virtual bool CanModSchemaName(FdoFeatureSchema* schema) { return false; }

    virtual bool CanAddClass(FdoClassDefinition* classDef) { return true; }
    virtual bool CanDeleteClass(FdoClassDefinition* classDef) { return true; }
    virtual bool CanModClassName(FdoClassDefinition* classDef) { return true; }
    
    virtual bool CanAddProperty(FdoPropertyDefinition* prop);
    virtual bool CanDeleteProperty(FdoPropertyDefinition* prop);
    virtual bool CanModPropertyName(FdoPropertyDefinition* prop);

    virtual bool CanModDataType(FdoDataPropertyDefinition* prop);
    virtual bool CanModDefaultValue(FdoDataPropertyDefinition* prop);
    virtual bool CanModDataNullable(FdoDataPropertyDefinition* prop);
    virtual bool CanModDataLength(FdoDataPropertyDefinition* prop);
    virtual bool CanModDataConstraint(FdoDataPropertyDefinition* prop);
    virtual bool CanRestrictDataConstraint(FdoDataPropertyDefinition* prop);
    virtual bool ClassHasObjects(FdoClassDefinition* classDef );

    virtual bool CheckDeleteProperty( FdoPropertyDefinition* prop );

    bool TableHasObjects(FdoString* tableName);
protected:
    SQLiteSchemaMergeContext() {}
    SQLiteSchemaMergeContext(SltConnection* connection, FdoFeatureSchemaCollection* oldSchemas,
        FdoFeatureSchema* newSchema, bool ignoreStates);
    ~SQLiteSchemaMergeContext() {}

private:
    SltConnection* m_connection;
    std::map<std::wstring, bool> m_hasObjects;
};

#endif
