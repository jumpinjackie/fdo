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
#include "Utilities/SchemaMgr/Overrides/GeometricContentTypeMapper.h"
#include <Fdo/Commands/CommandException.h>

FdoSmOvGeometricContentType FdoSmOvGeometricContentTypeMapper::String2Type( FdoString* contentTypeString, bool* pbFound )
{
    FdoStringP pString;

    if ( pbFound )
        (*pbFound) = true;

    if ( wcscmp( contentTypeString, L"Default" ) == 0 )
        return FdoSmOvGeometricContentType_Default;

    if ( wcscmp( contentTypeString, L"BuiltIn" ) == 0 )
        return FdoSmOvGeometricContentType_BuiltIn;

    if ( wcscmp( contentTypeString, L"Fgf" ) == 0 )
        return FdoSmOvGeometricContentType_Fgf;

    if ( wcscmp( contentTypeString, L"FgfText" ) == 0 )
        return FdoSmOvGeometricContentType_FgfText;

    if ( wcscmp( contentTypeString, L"Wkb" ) == 0 )
        return FdoSmOvGeometricContentType_Wkb;

    if ( wcscmp( contentTypeString, L"Wkt" ) == 0 )
        return FdoSmOvGeometricContentType_Wkt;

    if ( wcscmp( contentTypeString, L"Ordinates" ) == 0 )
        return FdoSmOvGeometricContentType_Ordinates;

    if ( pbFound ) {
        (*pbFound) = false;
    }
    else {
        //TODO: proper exception
        throw FdoCommandException::Create(
            FdoStringP::Format(
                L"Invalid FdoRdbms Schema Mapping geometric column mapping type '%ls'",
                contentTypeString
            )
        );
    }

    // Get to here only if type not found and pbFound
    // was set.
    // Return value is actually meaningless in this
    // case but need to return something to prevent a
    // compilation warning.
    return(FdoSmOvGeometricContentType_Default);
}

FdoStringP FdoSmOvGeometricContentTypeMapper::Type2String( FdoSmOvGeometricContentType contentType )
{

    switch ( contentType ) {
    case FdoSmOvGeometricContentType_Default:
        return FdoStringP(L"Default");
    case FdoSmOvGeometricContentType_BuiltIn:
        return FdoStringP(L"BuiltIn");
    case FdoSmOvGeometricContentType_Fgf:
        return FdoStringP(L"Fgf");
    case FdoSmOvGeometricContentType_FgfText:
        return FdoStringP(L"FgfText");
    case FdoSmOvGeometricContentType_Wkb:
        return FdoStringP(L"Wkb");
    case FdoSmOvGeometricContentType_Wkt:
        return FdoStringP(L"Wkt");
    case FdoSmOvGeometricContentType_Ordinates:
        return FdoStringP(L"Ordinates");
    default:
        //TODO: proper exception
        throw FdoSchemaException::Create();
    }
}


