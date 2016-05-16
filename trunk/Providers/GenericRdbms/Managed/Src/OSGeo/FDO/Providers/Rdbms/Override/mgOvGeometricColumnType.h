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

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

///<summary>OvGeometricColumnType is an enumeration of the possible column types
/// for geometric properties.  It is expressed in terms of FDO logical types.</summary>
/// <param name="OvGeometricColumnType_Default"> Provider's default. </param>
/// <param name="OvGeometricColumnType_BuiltIn"> Datastore’s built-in type (if available)
/// </param>
/// <param name="OvGeometricColumnType_Blob"> FDO "BLOB" (Binary Large OBject)
/// </param>
/// <param name="OvGeometricColumnType_Clob"> FDO "CLOB" (Character Large OBject)
/// </param>
/// <param name="OvGeometricColumnType_String"> FDO "String"
/// </param>
/// <param name="OvGeometricColumnType_Double"> FDO "Double" (only for use with OvGeometryContentType_Ordinates)
/// </param>
public enum class OvGeometricColumnType
{
    OvGeometricColumnType_Default = FdoSmOvGeometricColumnType_Default,
    OvGeometricColumnType_BuiltIn = FdoSmOvGeometricColumnType_BuiltIn,
    OvGeometricColumnType_Blob = FdoSmOvGeometricColumnType_Blob,
    OvGeometricColumnType_Clob = FdoSmOvGeometricColumnType_Clob,
    OvGeometricColumnType_String = FdoSmOvGeometricColumnType_String,
    OvGeometricColumnType_Double = FdoSmOvGeometricColumnType_Double
};


END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

