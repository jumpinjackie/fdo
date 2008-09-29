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

#ifndef SHPLPPROPERTYDEFINITION_H_
#define SHPLPPROPERTYDEFINITION_H_     1

#ifdef _WIN32
#pragma once
#endif

#include <SHP/Override/FdoShpPropertyDefinition.h>
#include <../ShpRead/ColumnInfo.h>
#include <ShpLpClassDefinition.h>

class ShpLpClassDefinition;

class ShpLpPropertyDefinition :
    public FdoDisposable
{
public:
    ShpLpPropertyDefinition(ShpLpClassDefinition *parentLpClass, int iPhysicalColumnIndex, FdoPropertyDefinition *configLogicalProperty, FdoShpOvPropertyDefinition* configPropertyMapping, bool bPhysicalToLogical);
    ~ShpLpPropertyDefinition(void);

    void ConvertPhysicalToLogical (FdoPropertyDefinition* configLogicalProperty, FdoShpOvPropertyDefinition* propertyMapping);
    void ConvertLogicalToPhysical (int physicalColumnIndex, FdoPropertyDefinition* logicalProperty, FdoShpOvPropertyDefinition* configPropertyMapping);


    // To please FdoNamedCollection:
    FdoString* GetName(void);
    virtual FdoBoolean CanSetName() { return true; }

    // Returns the physical column name:
    FdoString* GetPhysicalColumnName(void);

    // Generate a new copy of the schema mappings for this property, optionally including default mappings
    FdoShpOvPropertyDefinition* GetSchemaMappings(bool bIncludeDefaults);


protected:
	ShpLpPropertyDefinition() {};
    
private:
    // Parent LpClass:
    ShpLpClassDefinition* m_parentLpClass; // weak reference

    // Logical layer:
    FdoPtr<FdoDataPropertyDefinition> m_logicalProperty;

    // Physical layer:
    int m_physicalColumnIndex;
};


class ShpLpPropertyDefinitionCollection : public FdoNamedCollection<ShpLpPropertyDefinition, FdoException>
{
public:
    ShpLpPropertyDefinitionCollection(void) {}

protected:
    virtual ~ShpLpPropertyDefinitionCollection(void) {}

    /// <summary>Dispose this object instance.</summary>
    /// <returns>Returns nothing</returns>
    virtual void Dispose() 
    { 
        delete this; 
    }
};


#endif // SHPLPPROPERTYDEFINITION_H_

