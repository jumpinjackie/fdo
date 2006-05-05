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

public __gc class OvGeometricColumn;

/// <summary>Abstract class defining physical schema overrides for a geometric property
/// definition.</summary>
public __gc class OvGeometricPropertyDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition
{
public private:
    inline FdoRdbmsOvGeometricPropertyDefinition* GetImpObj();

public protected:
    OvGeometricPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    /// <summary>Gets the column override</summary>
    /// <returns>Returns OvGeometricColumn</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumn* get_Column();

    /// <summary>Gets the geometric column type</summary>
    /// <returns>Returns the default geometric column type</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType get_GeometricColumnType();

    /// <summary>Sets the geometric column type</summary>
    /// <param name="mappingType">Input the geometric column type</param>
    /// <returns>Returns nothing</returns>
    __property System::Void set_GeometricColumnType(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType columnType);

    /// <summary>Gets the geometric content type</summary>
    /// <returns>Returns the geometric content type</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType get_GeometricContentType();

    /// <summary>Sets the content type</summary>
    /// <param name="mappingType">Input the content type</param>
    /// <returns>Returns nothing</returns>
    __property System::Void set_GeometricContentType(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType contentType);

    /// <summary>Gets the ordinate column name for the X ordinate.
    /// This only applies when GeometricContentType==OvGeometricContentType_Ordinates.
    /// Default value is "X".
    /// </summary>
    /// <returns>Returns the ordinate column name for the X ordinate</returns>
    __property System::String * get_XColumnName();

    /// <summary>Sets the ordinate column name for the X ordinate</summary>
    /// <param name="mappingType">Input the X ordinate column name</param>
    /// <returns>Returns nothing</returns>
    __property System::Void set_XColumnName(System::String * columnName);

    /// <summary>Gets the ordinate column name for the Y ordinate.
    /// This only applies when GeometricContentType==OvGeometricContentType_Ordinates.
    /// Default value is "Y".
    /// </summary>
    /// <returns>Returns the ordinate column name for the Y ordinate</returns>
    __property System::String * get_YColumnName();

    /// <summary>Sets the ordinate column name for the Y ordinate</summary>
    /// <param name="mappingType">Input the Y ordinate column name</param>
    /// <returns>Returns nothing</returns>
    __property System::Void set_YColumnName(System::String * columnName);

    /// <summary>Gets the ordinate column name for the Z ordinate.
    /// This only applies when GeometricContentType==OvGeometricContentType_Ordinates
    /// and the property definition's "HasElevation" attribute is "true".
    /// Default value is "Z".
    /// </summary>
    /// <returns>Returns the ordinate column name for the Z ordinate</returns>
    __property System::String * get_ZColumnName();

    /// <summary>Sets the ordinate column name for the Z ordinate</summary>
    /// <param name="mappingType">Input the Z ordinate column name</param>
    /// <returns>Returns nothing</returns>
    __property System::Void set_ZColumnName(System::String * columnName);
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE
