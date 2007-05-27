//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"
#include "PostGisProvider.h"
#include "PgUtility.h"
// std
#include <cassert>
#include <string>
// boost
#include <boost/format.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace
{
    const int PgVarHeaderSize = 4;
}

namespace fdo { namespace postgis { namespace details {

FdoDataType FdoTypeFromPgTypeName(std::string const& typeName)
{
    using boost::algorithm::iequals;
    
    FdoDataType fdoType;

    if (iequals(typeName, "bool") || iequals(typeName, "boolean"))
    {
        fdoType = FdoDataType_Boolean;
    }
    else if (iequals(typeName, "\"char\""))
    {
        // Single-character internal type in PostgreSQL.
        // The type "char" (note the quotes) is different from char(1) in that it
        // only uses one byte of storage.
        // It is internally used in the system catalogs as a poor-man's enumeration type.
        fdoType = FdoDataType_Byte;
    }
    else if (iequals(typeName, "int2"))
    {
        // Returned also for types: smallint
        fdoType = FdoDataType_Int16;
    }
    else if (iequals(typeName, "int4") || iequals(typeName, "int"))
    {
        // Returned also for types: integer, serial, serial4
        fdoType = FdoDataType_Int32;
    }
    else if (iequals(typeName, "int8"))
    {
        // Returned also for types: bigint, bigserial, serial8
        fdoType = FdoDataType_Int64;
    }
    else if (iequals(typeName, "float4"))
    {
        // Returned also for types: real
        fdoType = FdoDataType_Single;
    }
    else if (iequals(typeName, "float8"))
    {
        // Returned also for types: double precision
        fdoType = FdoDataType_Double;
    }
    else if (iequals(typeName, "numeric"))
    {
        fdoType = FdoDataType_Decimal;
    }
    else if (iequals(typeName, "char") || iequals(typeName, "bpchar")
        || iequals(typeName, "varchar") || iequals(typeName, "text")
        || iequals(typeName, "cstring") || iequals(typeName, "name"))
    {
        // The type char(n) is different than "char", see note for FdoDataType_Byte.
        // Type 'name' is equivalent of VARCHAR(60).
        fdoType = FdoDataType_String;
    }
    else if (iequals(typeName, "abstime") || iequals(typeName, "reltime")
        || iequals(typeName, "date") || iequals(typeName, "time")
        || iequals(typeName, "timestamp") || iequals(typeName, "timestampz") 
        || iequals(typeName, "timetz"))
    {
        fdoType = FdoDataType_DateTime;
    }
    else if (iequals(typeName, "bytea"))
    {
        assert(!"TYPE MAPPING TO BE REVIEWED");
        fdoType = FdoDataType_BLOB;
    }
    else if (iequals(typeName, "oid"))
    {
        // OID size is 4 bytes
        fdoType = FdoDataType_Int32;
    }
    else if (iequals(typeName, "money"))
    {
        // IMPORTANT: The money type is deprecated. Use numeric or decimal instead,
        //            in combination with the to_char function.
        assert(!"MONEY TYPE IS DEPRECATED");
        fdoType = FdoDataType_Decimal;
    }
    else
    {
        if (iequals(typeName, "geometry"))
        {
            // We should NEVER get here, because SQL queries in PgTableColumnsReader
            // does NOT query for geometry columns at all.
            // If by any chance we get here from PgTableColumnsReader, then we have 
            // somewhere a serious BUG.
            assert(!"GEOMETRY DOES NOT MAP TO FdoDataType");
        }
        
        // TODO: Add mapping of FdoDataType_Byte and FdoDataType_CLOB
        
        FdoStringP tmp(typeName.c_str());
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_UNSUPPORTED_DATATYPE,
            "The '%1$ls' data type is not supported by PostGIS provider.",
            static_cast<FdoString*>(tmp)));
    }

    return fdoType;
}

int GetTypeMaxSize(int const& modifier)
{
    return (modifier - PgVarHeaderSize);
}

int GetTypePrecision(int const& modifier)
{
    return ((modifier - PgVarHeaderSize) >> 16);
}

int GetTypeScale(int const& modifier)
{
    return ((modifier - PgVarHeaderSize) & 0xffff);
}

std::string PgTypeFromFdoProperty(FdoPtr<FdoPropertyDefinition> prop)
{
    std::string sqlType;

    if (FdoPropertyType_DataProperty == prop->GetPropertyType())
    {
        //FdoPtr<FdoDataPropertyDefinition> dataDef(static_cast<FdoDataPropertyDefinition*>(prop.p));
        FdoDataPropertyDefinition* dataDef = static_cast<FdoDataPropertyDefinition*>(prop.p);
        switch (dataDef->GetDataType())
        {
        case FdoDataType_String:
            {
                FdoInt32 length = dataDef->GetLength();
                if (length <= 0)
                {
                    length = ePgTypeTextMaxSize;
                }
                sqlType = str(boost::format("%s(%d)") % "character varying" % length);
            }
            break;
        case FdoDataType_Decimal:
            {
                FdoInt32 const precision = dataDef->GetPrecision();
                FdoInt32 const scale = dataDef->GetScale();
                sqlType = str(boost::format("%s(%d,%d)") % "numeric" % precision % scale);
            }
            break;
        case FdoDataType_Byte:
            sqlType = "character(1)";
            break;
        case FdoDataType_Single:
            sqlType = "real";
            break;
        case FdoDataType_Double:
            sqlType = "double precision";
            break;
        case FdoDataType_DateTime:
            sqlType = "timestamp";
            break;
        case FdoDataType_Boolean:
            sqlType = "boolean";
            break;
        case FdoDataType_Int16:
            sqlType = "smallint";
            break;
        case FdoDataType_Int32:
            sqlType = "integer";
            break;
        case FdoDataType_Int64:
            sqlType = "bigint";
            break;
        }
    }
    else if (FdoPropertyType_GeometricProperty == prop->GetPropertyType())
    {
    }

    return sqlType;
}

}}} // namespace fdo::postgis::details
