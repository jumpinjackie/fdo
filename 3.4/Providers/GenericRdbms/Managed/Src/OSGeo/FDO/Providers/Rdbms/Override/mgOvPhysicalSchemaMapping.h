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
public __gc class PhysicalElementMapping;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

public __gc class OvReadOnlyClassCollection;
public __gc class OvSchemaAutoGeneration;

///<summary>Abstract base class for RDBMS type provider schema 
///override sets.</summary>
public __gc class OvPhysicalSchemaMapping : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping
{
public private:
    FdoRdbmsOvPhysicalSchemaMapping* GetImpObj();
	
public protected:
    OvPhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete);   

protected:
    OvPhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* schemaMapping, System::Boolean autoDelete);

public:
    ///<summary>Gets the collection of class overrides for this schema 
    ///override set.</summary>
    /// <returns>Returns OvReadOnlyClassCollection</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection* get_Classes();

    ///<summary>Gets the table mapping</summary>
    ///<returns>Returns the table mapping</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType get_TableMapping();

    ///<summary>Sets the table mapping</summary>
    /// <param name="mappingType">Input the mapping type</param>
    ///<returns>Returns nothing</returns>
    __property System::Void set_TableMapping(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType mappingType);

    ///<summary>Gets the default geometric column type for newly-created geometric properties</summary>
    ///<returns>Returns the default geometric column type</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType get_GeometricColumnType();

    ///<summary>Sets default geometric column type for newly-created geometric properties</summary>
    /// <param name="columnType">Input the default geometric column type</param>
    ///<returns>Returns nothing</returns>
    __property System::Void  set_GeometricColumnType(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType columnType);

    ///<summary>Gets the default geometric content type for newly-created geometric properties</summary>
    ///<returns>Returns the default geometric content type</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType get_GeometricContentType();

    ///<summary>Sets default geometric content type for newly-created geometric properties</summary>
    /// <param name="contentType">Input the default geometric content type</param>
    ///<returns>Returns nothing</returns>
    __property System::Void  set_GeometricContentType(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType contentType);

    ///<summary>Gets the settings for AutoGenerating classes for this feature
    ///schema from the physical schema of the connected datastore.
    ///</summary>
    ///<returns>Returns OvSchemaAutoGeneration</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration* get_AutoGeneration();

    ///<summary>Sets class autogeneration settings.</summary>
    /// <param name="autoGeneration">
    /// Input the autogeneration settings. If NULL then no schema autogeneration
    /// takes place.
    /// </param>
    ///<returns>Returns nothing</returns>
    __property System::Void set_AutoGeneration(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration* autoGeneration);

    __property System::Void set_Parent(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping* value);   
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE
