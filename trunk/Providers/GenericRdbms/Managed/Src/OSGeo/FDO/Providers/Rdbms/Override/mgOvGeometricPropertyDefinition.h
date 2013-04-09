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

class FdoRdbmsOvGeometricPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

ref class OvGeometricColumn;

/// <summary>Abstract class defining physical schema overrides for a geometric property
/// definition.</summary>
public ref class OvGeometricPropertyDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition
{
internal:
    inline FdoRdbmsOvGeometricPropertyDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

public protected:
    OvGeometricPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    /// <summary>Gets the column override</summary>
    /// <returns>Returns OvGeometricColumn</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumn^ Column
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumn^ get();
    }

    /// <summary>Gets the geometric column type</summary>
    /// <returns>Returns the default geometric column type</returns>
    /// <summary>Sets the geometric column type</summary>
    /// <param name="mappingType">Input the geometric column type</param>
    /// <returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType GeometricColumnType
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType columnType);
    }

    /// <summary>Gets the geometric content type</summary>
    /// <returns>Returns the geometric content type</returns>
    /// <summary>Sets the content type</summary>
    /// <param name="mappingType">Input the content type</param>
    /// <returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType GeometricContentType
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType contentType);
    }

    /// <summary>Gets the ordinate column name for the X ordinate.
    /// This only applies when GeometricContentType==OvGeometricContentType_Ordinates.
    /// Default value is "X".
    /// </summary>
    /// <returns>Returns the ordinate column name for the X ordinate</returns>
    /// <summary>Sets the ordinate column name for the X ordinate</summary>
    /// <param name="mappingType">Input the X ordinate column name</param>
    /// <returns>Returns nothing</returns>
    property System::String^ XColumnName
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// <summary>Gets the ordinate column name for the Y ordinate.
    /// This only applies when GeometricContentType==OvGeometricContentType_Ordinates.
    /// Default value is "Y".
    /// </summary>
    /// <returns>Returns the ordinate column name for the Y ordinate</returns>
    /// <summary>Sets the ordinate column name for the Y ordinate</summary>
    /// <param name="mappingType">Input the Y ordinate column name</param>
    /// <returns>Returns nothing</returns>
    property System::String^ YColumnName
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// <summary>Gets the ordinate column name for the Z ordinate.
    /// This only applies when GeometricContentType==OvGeometricContentType_Ordinates
    /// and the property definition's "HasElevation" attribute is "true".
    /// Default value is "Z".
    /// </summary>
    /// <returns>Returns the ordinate column name for the Z ordinate</returns>
    /// <summary>Sets the ordinate column name for the Z ordinate</summary>
    /// <param name="mappingType">Input the Z ordinate column name</param>
    /// <returns>Returns nothing</returns>
    property System::String^ ZColumnName
    {
        System::String^ get();
        System::Void set(System::String^ columnName);
    }
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE
