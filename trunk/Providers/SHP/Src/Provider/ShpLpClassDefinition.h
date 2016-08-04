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

#ifndef SHPLPCLASSDEFINITION_H_
#define SHPLPCLASSDEFINITION_H_     1

#ifdef _WIN32
#pragma once
#endif


#include <SHP/Override/FdoShpClassDefinition.h>
#include <ShpLpPropertyDefinition.h>
#include <ShpLpFeatureSchema.h>

class ShpLpFeatureSchema;

class ShpConnection;

class ShpLpClassDefinition :
    public FdoDisposable
{
    friend class ShpLpFeatureSchema;

public:
    ShpLpClassDefinition(void);
    ShpLpClassDefinition(ShpLpFeatureSchema *parentLpSchema, ShpConnection *connection, ShpFileSet *pFileSet, FdoClassDefinition* classDef, FdoShpOvClassDefinition* classMapping);
    ~ShpLpClassDefinition(void);

    void ConvertPhysicalToLogical(ShpConnection * connection, FdoClassDefinition * configLogicalClass, FdoShpOvClassDefinition * configClassMapping);
    void ConvertLogicalToPhysical(ShpConnection * connection, FdoClassDefinition * logicalClass, FdoShpOvClassDefinition * classMapping);

    // Convert physical identity 'column' to/from logical identity property:
    static FdoDataPropertyDefinition* ConvertPhysicalToLogicalIdentityProperty(FdoClassDefinition* configLogicalClass);

    // Convert physical geometry 'column' to/from logical geometry property;
    // NOTE: by design there is no override for geometry properties:
    static FdoGeometricPropertyDefinition* ConvertPhysicalToLogicalGeometryProperty(ShpConnection *connection, FdoClassDefinition* configLogicalClass, ShpFileSet *physicalFileSet);

    // Get LogicalPhysical properties owned by this class:
    ShpLpPropertyDefinitionCollection* GetLpProperties() const;

    // Get physical fileset:
    ShpFileSet* GetPhysicalFileSet(void);

    // Get physical column info:
    ColumnInfo* GetPhysicalColumnInfo (void);

    // Get logical class:
    FdoClassDefinition* GetLogicalClass(void);

    // Set the parent logical/physical schema
    void SetParent (ShpLpFeatureSchema* parent);

    // Permanently delete the Physical, LogicalPhysical and Logical counterparts of this class:
    void Delete(ShpConnection* connection);

    // Generate a new copy of the schema mappings for this class, optionally including default mappings
    FdoShpOvClassDefinition* GetSchemaMappings(bool bIncludeDefaults);

    // To please FdoNamedCollection:
    FdoString* GetName();
    virtual FdoBoolean CanSetName() { return true; }


private:
    ShpConnection* m_connection;  // NOTE: weak reference

    // Logical layer:
    FdoPtr<FdoClassDefinition> m_logicalClassDefinition;

    // LogicalPhysical layer:
    ShpLpFeatureSchema*                       m_parentLpSchema; // weak reference
    FdoPtr<ShpLpPropertyDefinitionCollection> m_LpProperties;

    // Physical layer:
    ShpFileSet *m_physicalFileSet;
    ColumnInfo* m_physicalColumns;
};



class ShpLpClassDefinitionCollection : public FdoNamedCollection<ShpLpClassDefinition, FdoException>
{
public:
    ShpLpClassDefinitionCollection(void) {}

protected:
    virtual ~ShpLpClassDefinitionCollection(void) {}

    /// <summary>Dispose this object instance.</summary>
    /// <returns>Returns nothing</returns>
    virtual void Dispose() 
    { 
        delete this; 
    }
};


#endif // SHPLPCLASSDEFINITION_H_

