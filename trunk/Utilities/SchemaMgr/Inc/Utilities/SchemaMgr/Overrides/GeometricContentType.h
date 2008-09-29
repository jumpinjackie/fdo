#ifndef FDO_SMOVGEOMETRICCONTENTYPE_H
#define FDO_SMOVGEOMETRICCONTENTYPE_H

// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/// <library>
/// SchemaMgr_OVNew.lib
/// </library>

///<summary>FdoSmOvGeometricContentType is an enumeration of the possible formats in
/// which to store geometric property values.  For a given geometric property,
/// Values of this enumeration must be 
/// compatible with the corresponding FdoSmOvGeometricColumnType value..</summary>
/// <param name="FdoSmOvGeometricContentType_Default"> Provider's default. </param>
/// <param name="FdoSmOvGeometricContentType_BuiltIn"> Datastore's built-in type (if available)
/// </param>
/// <param name="FdoSmOvGeometricContentType_Fgf"> FGF (binary)
/// </param>
/// <param name="FdoSmOvGeometricContentType_FgfText"> FGF Text
/// </param>
/// <param name="FdoSmOvGeometricContentType_Wkb"> OpenGis WKB
/// </param>
/// <param name="FdoSmOvGeometricContentType_Wkt"> OpenGis WKT
/// </param>
/// <param name="FdoSmOvGeometricContentType_Ordinates"> Ordinate columns (default names "X", "Y" and optionally "Z") - column type must be FdoRdbmsOvGeometryColumnType_Double
/// </param>
enum FdoSmOvGeometricContentType
{
    FdoSmOvGeometricContentType_Default,
    FdoSmOvGeometricContentType_BuiltIn,
    FdoSmOvGeometricContentType_Fgf,
    FdoSmOvGeometricContentType_FgfText,
    FdoSmOvGeometricContentType_Wkb,
    FdoSmOvGeometricContentType_Wkt,
    FdoSmOvGeometricContentType_Ordinates
};

#endif
