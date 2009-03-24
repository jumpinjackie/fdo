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

#pragma once

class FdoRdbmsOvPhysicalSchemaMapping;


BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
ref class PhysicalElementMapping;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

ref class OvReadOnlyClassCollection;
ref class OvSchemaAutoGeneration;

///<summary>Abstract base class for RDBMS type provider schema 
///override sets.</summary>
public ref class OvPhysicalSchemaMapping : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping
{
internal:
    FdoRdbmsOvPhysicalSchemaMapping* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
	
public protected:
    OvPhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete);   

protected:
    OvPhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ schemaMapping, System::Boolean autoDelete);

public:
    ///<summary>Gets the collection of class overrides for this schema 
    ///override set.</summary>
    /// <returns>Returns OvReadOnlyClassCollection</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection^ Classes
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection^ get();
    }

    ///<summary>Gets the table mapping</summary>
    ///<returns>Returns the table mapping</returns>
    ///<summary>Sets the table mapping</summary>
    /// <param name="mappingType">Input the mapping type</param>
    ///<returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType TableMapping
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType mappingType);
    }

    ///<summary>Gets the default geometric column type for newly-created geometric properties</summary>
    ///<returns>Returns the default geometric column type</returns>
    ///<summary>Sets default geometric column type for newly-created geometric properties</summary>
    /// <param name="columnType">Input the default geometric column type</param>
    ///<returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType GeometricColumnType
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType columnType);
    }

    ///<summary>Gets the default geometric content type for newly-created geometric properties</summary>
    ///<returns>Returns the default geometric content type</returns>
    ///<summary>Sets default geometric content type for newly-created geometric properties</summary>
    /// <param name="contentType">Input the default geometric content type</param>
    ///<returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType GeometricContentType
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType contentType);
    }

    ///<summary>Gets the settings for AutoGenerating classes for this feature
    ///schema from the physical schema of the connected datastore.
    ///</summary>
    ///<returns>Returns OvSchemaAutoGeneration</returns>
    ///<summary>Sets class autogeneration settings.</summary>
    /// <param name="autoGeneration">
    /// Input the autogeneration settings. If nullptr then no schema autogeneration
    /// takes place.
    /// </param>
    ///<returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration^ AutoGeneration
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration^ autoGeneration);
    }

    property NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^ Parent
    {
        System::Void set(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^ value);
    }
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE
