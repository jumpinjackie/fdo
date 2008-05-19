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
#include <Utilities/SchemaMgr/Overrides/TableMappingType.h>
#include <Utilities/SchemaMgr/Overrides/TableMappingTypeMapper.h>
#include <Fdo/Commands/CommandException.h>

FdoSmOvTableMappingType FdoSmOvTableMappingTypeMapper::String2Type( FdoString* dataTypeString, bool* pbFound )
{
    FdoStringP pString;

    if ( pbFound )
        (*pbFound) = true;

    if ( wcscmp( dataTypeString, L"Default" ) == 0 )
        return FdoSmOvTableMappingType_Default;

    if ( wcscmp( dataTypeString, L"Class" ) == 0 )
        return FdoSmOvTableMappingType_ClassTable;

    if ( wcscmp( dataTypeString, L"Concrete" ) == 0 )
        return FdoSmOvTableMappingType_ConcreteTable;

    if ( wcscmp( dataTypeString, L"Base" ) == 0 )
        return FdoSmOvTableMappingType_BaseTable;

    if ( pbFound ) {
        (*pbFound) = false;
    }
    else {
        //TODO: proper exception
        throw FdoCommandException::Create(
            FdoStringP::Format(
                L"Invalid Oracle Schema Mapping table mapping type '%ls'",
                dataTypeString
            )
        );
/*
            FdoCommandException::Create(
                NlsMsgGet1(
                    FDORDBMS_294,
                    "Invalid Oracle Schema Mapping table mapping type '%1$ls'",
                    dataTypeString
                )
            );
*/
    }

    // Get to here only if type not found and pbFound
    // was set.
    // Return value is actually meaningless in this
    // case but need to return something to prevent a
    // compilation warning.
    return(FdoSmOvTableMappingType_Default);
}

FdoStringP FdoSmOvTableMappingTypeMapper::Type2String( FdoSmOvTableMappingType dataType )
{

    switch ( dataType ) {
    case FdoSmOvTableMappingType_ClassTable:
        return FdoStringP(L"Class");
    case FdoSmOvTableMappingType_ConcreteTable:
        return FdoStringP(L"Concrete");
    case FdoSmOvTableMappingType_BaseTable:
        return FdoStringP(L"Base");
    case FdoSmOvTableMappingType_Default:
        return FdoStringP(L"Default");
    default:
        //TODO: proper exception
        throw FdoSchemaException::Create();
/*            FdoSchemaException::Create(
                NlsMsgGet1(
                    FDORDBMS_296,
                    "Invalid Oracle Schema Mapping table mapping type %1$d",
                    dataType
                )
            );*/
    }
}


