#ifndef FDO_SMOVGEOMETRICCOLUMNTYPE_H
#define FDO_SMOVGEOMETRICCOLUMNTYPE_H

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

///<summary>FdoSmOvGeometricColumnType is an enumeration of the possible column types
/// for geometric properties.  It is expressed in terms of FDO logical types.</summary>
/// <param name="FdoSmOvGeometricColumnType_Default"> Provider's default. </param>
/// <param name="FdoSmOvGeometricColumnType_BuiltIn"> Datastore’s built-in type (if available)
/// </param>
/// <param name="FdoSmOvGeometricColumnType_Blob"> FDO "BLOB" (Binary Large OBject)
/// </param>
/// <param name="FdoSmOvGeometricColumnType_Clob"> FDO "CLOB" (Character Large OBject)
/// </param>
/// <param name="FdoSmOvGeometricColumnType_String"> FDO "String"
/// </param>
/// <param name="FdoSmOvGeometricColumnType_Double"> FDO "Double" (only for use with FdoRdbmsOvGeometryContentType_Ordinates)
/// </param>
enum FdoSmOvGeometricColumnType
{
    FdoSmOvGeometricColumnType_Default,
    FdoSmOvGeometricColumnType_BuiltIn,
    FdoSmOvGeometricColumnType_Blob,
    FdoSmOvGeometricColumnType_Clob,
    FdoSmOvGeometricColumnType_String,
    FdoSmOvGeometricColumnType_Double
};

#endif
