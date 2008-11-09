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

#include "stdafx.h"
#include "Utilities/SchemaMgr/Overrides/GeometricColumnTypeMapper.h"
#include <Fdo/Commands/CommandException.h>

FdoSmOvGeometricColumnType FdoSmOvGeometricColumnTypeMapper::String2Type( FdoString* columnTypeString, bool* pbFound )
{
    FdoStringP pString;

    if ( pbFound )
        (*pbFound) = true;

    if ( wcscmp( columnTypeString, L"Default" ) == 0 )
        return FdoSmOvGeometricColumnType_Default;

    if ( wcscmp( columnTypeString, L"BuiltIn" ) == 0 )
        return FdoSmOvGeometricColumnType_BuiltIn;

    if ( wcscmp( columnTypeString, L"Blob" ) == 0 )
        return FdoSmOvGeometricColumnType_Blob;

    if ( wcscmp( columnTypeString, L"Clob" ) == 0 )
        return FdoSmOvGeometricColumnType_Clob;

    if ( wcscmp( columnTypeString, L"String" ) == 0 )
        return FdoSmOvGeometricColumnType_String;

    if ( wcscmp( columnTypeString, L"Double" ) == 0 )
        return FdoSmOvGeometricColumnType_Double;

    if ( pbFound ) {
        (*pbFound) = false;
    }
    else {
        //TODO: proper exception
        throw FdoCommandException::Create(
            FdoStringP::Format(
                L"Invalid FdoRdbms Schema Mapping geometric column mapping type '%ls'",
                columnTypeString
            )
        );
    }

    // Get to here only if type not found and pbFound
    // was set.
    // Return value is actually meaningless in this
    // case but need to return something to prevent a
    // compilation warning.
    return(FdoSmOvGeometricColumnType_Default);
}

FdoStringP FdoSmOvGeometricColumnTypeMapper::Type2String( FdoSmOvGeometricColumnType columnType )
{

    switch ( columnType ) {
    case FdoSmOvGeometricColumnType_Default:
        return FdoStringP(L"Default");
    case FdoSmOvGeometricColumnType_BuiltIn:
        return FdoStringP(L"BuiltIn");
    case FdoSmOvGeometricColumnType_Blob:
        return FdoStringP(L"Blob");
    case FdoSmOvGeometricColumnType_Clob:
        return FdoStringP(L"Clob");
    case FdoSmOvGeometricColumnType_String:
        return FdoStringP(L"String");
    case FdoSmOvGeometricColumnType_Double:
        return FdoStringP(L"Double");
    default:
        //TODO: proper exception
        throw FdoSchemaException::Create();
    }
}


