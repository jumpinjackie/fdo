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

///<summary>OvGeometricContentType is an enumeration of the possible formats in
/// which to store geometric property values.  For a given geometric property,
/// Values of this enumeration must be 
/// compatible with the corresponding OvGeometricColumnType value..</summary>
/// <param name="OvGeometricContentType_Default"> Provider's default. </param>
/// <param name="OvGeometricContentType_BuiltIn"> Datastore抯 built-in type (if available)
/// </param>
/// <param name="OvGeometricContentType_Fgf"> FGF (binary)
/// </param>
/// <param name="OvGeometricContentType_FgfText"> FGF Text
/// </param>
/// <param name="OvGeometricContentType_Wkb"> OpenGis WKB
/// </param>
/// <param name="OvGeometricContentType_Wkt"> OpenGis WKT
/// </param>
/// <param name="OvGeometricContentType_Ordinates"> Ordinate columns (default names "X", "Y" and optionally "Z") ?column type must be OvGeometryColumnType_Double
/// </param>
public __value enum OvGeometricContentType
{
    OvGeometricContentType_Default = FdoSmOvGeometricContentType_Default,
    OvGeometricContentType_BuiltIn = FdoSmOvGeometricContentType_BuiltIn,
    OvGeometricContentType_Fgf = FdoSmOvGeometricContentType_Fgf,
    OvGeometricContentType_FgfText = FdoSmOvGeometricContentType_FgfText,
    OvGeometricContentType_Wkb = FdoSmOvGeometricContentType_Wkb,
    OvGeometricContentType_Wkt = FdoSmOvGeometricContentType_Wkt,
    OvGeometricContentType_Ordinates = FdoSmOvGeometricContentType_Ordinates
};


END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

