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
#include "SchemaCapabilities.h"

#include <cassert>

namespace fdo { namespace postgis {

SchemaCapabilities::SchemaCapabilities()
{
}

SchemaCapabilities::~SchemaCapabilities()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void SchemaCapabilities::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoISchemaCapabilities interface
///////////////////////////////////////////////////////////////////////////////

FdoClassType* SchemaCapabilities::GetClassTypes(FdoInt32& size)
{
    static FdoClassType types[] =
    {
        FdoClassType_FeatureClass,
        FdoClassType_Class
    };

    size = (sizeof(types) / sizeof(FdoClassType));
    return types;
}

FdoDataType* SchemaCapabilities::GetDataTypes(FdoInt32& size)
{
    static FdoDataType types[] =
    {
        FdoDataType_Boolean,
        FdoDataType_Byte,
        FdoDataType_DateTime,
        FdoDataType_Decimal,
        FdoDataType_Double,
        FdoDataType_Int16,
        FdoDataType_Int32,
        FdoDataType_Int64,
        FdoDataType_Single,
        FdoDataType_String

        // TODO: Types not supported
        //FdoDataType_BLOB 
        //FdoDataType_CLOB 
    };

    size = (sizeof(types) / sizeof(FdoDataType));
    return types;
}

FdoInt64 SchemaCapabilities::GetMaximumDataValueLength(FdoDataType type)
{
    // Maximum data length is interpreted as maximum storage size of
    // particular data type.

    FdoInt64 length = -1;

    switch(type)
    {
    case FdoDataType_String:
        // The longest possible character string that can be stored is about 1 GB
        length = -1;
        break;
    case FdoDataType_BLOB:
    case FdoDataType_CLOB:
        // Variable length
        length = -1;
        break;
    case FdoDataType_Decimal:
        // Variable length
        length = GetMaximumDecimalPrecision();
        break;
    case FdoDataType_Boolean:
        length = sizeof(FdoBoolean);
        break;
    case FdoDataType_Byte:
        length = sizeof(FdoByte);
        break;
    case FdoDataType_Int16:
        length = sizeof(FdoInt16);
        break;
    case FdoDataType_Int32:
        length = sizeof(FdoInt32);
        break;
    case FdoDataType_Int64:
        length = sizeof(FdoInt64);
        break;
    case FdoDataType_Single:
        // 4 bytes
        length = sizeof(FdoFloat);
        break;
    case FdoDataType_Double:
        // 8 bytes
        length = sizeof(FdoDouble);
        break;
    case FdoDataType_DateTime:
        length = sizeof(FdoDateTime);
        break;
    }

    return length;
}

FdoInt32 SchemaCapabilities::GetMaximumDecimalPrecision()
{
    // From PostgreSQL documentation, 8.1.2. Arbitrary Precision Numbers:
    // The type numeric can store numbers with up to 1000 digits of precision and perform calculations exactly
    // NOTE: If left unspecified, the precision will default to 30 digits, and scale to 6 digits. 

    return 1000;
}

FdoInt32 SchemaCapabilities::GetMaximumDecimalScale()
{
    // There is no maximum scale defiend in PostgreSQL.
    // Possible scale value to insert depends on precision.
    // An overflow error will be issued if you attempt to insert a number
    // that is larger than the allotted precision range.

    // PostgreSQL defines both max precision and scale equal to 1000
    // File: pgsql/src/interfaces/ecpg/include/pgtypes_numeric.h

    // TODO: Does value -1 fit the required semantic better?
    return 1000;
}

FdoInt32 SchemaCapabilities::GetNameSizeLimit(FdoSchemaElementNameType name)
{
    FdoInt32 limit = 0;

    switch (name)
    {
    case FdoSchemaElementNameType_Datastore:
    case FdoSchemaElementNameType_Schema:
    case FdoSchemaElementNameType_Class:
    case FdoSchemaElementNameType_Property:
        // It's length of 'name' data type that is defined
        // as 64 bytes (including \0 terminator)

#if defined(PG_VERSION_NUM) && (PG_VERSION_NUM >= 80301)
		limit = 64 - 1;
#else
        limit = NAMEDATALEN - 1; // declared in postgres_ext.h
#endif
        break;
    case FdoSchemaElementNameType_Description:
        // Description is stored as a COMMENT associated with database object.
        // COMMENT stores data of 'text' type, without any size limit.
        // We set the limit arbitraily to 300 characters

        limit = 300;
        break;
    }

    return limit;
}

FdoString* SchemaCapabilities::GetReservedCharactersForName()
{
    return NULL;
}

FdoDataType* SchemaCapabilities::GetSupportedAutoGeneratedTypes(FdoInt32& size)
{
    static FdoDataType types [] =
    {
        FdoDataType_Int32,
        FdoDataType_Int64
    };

    size = (sizeof(types) / sizeof(FdoDataType));
    return types;
}

FdoDataType* SchemaCapabilities::GetSupportedIdentityPropertyTypes(FdoInt32& size)
{
    static FdoDataType types[] =
    {
        FdoDataType_Boolean,
        FdoDataType_Byte,
        FdoDataType_DateTime,
        FdoDataType_Decimal,
        FdoDataType_Double,
        FdoDataType_Int16,
        FdoDataType_Int32,
        FdoDataType_Int64,
        FdoDataType_Single,
        FdoDataType_String
    };

    size = (sizeof(types) / sizeof(FdoDataType));
    return types;
}

bool SchemaCapabilities::SupportsAssociationProperties()
{
    return false;
}

bool SchemaCapabilities::SupportsAutoIdGeneration()
{
    return true;
}

bool SchemaCapabilities::SupportsCompositeId()
{
    // NOTE: We assume that composite ID means multiple column PRIMARY KEY
    //       PostgreSQL does support it, so we can turn it on.

    return true;
}

bool SchemaCapabilities::SupportsCompositeUniqueValueConstraints()
{
    return true;
}

bool SchemaCapabilities::SupportsDataStoreScopeUniqueIdGeneration()
{
    return false;
}

bool SchemaCapabilities::SupportsDefaultValue()
{
    return true;
}

bool SchemaCapabilities::SupportsExclusiveValueRangeConstraints()
{
    return false;
}

bool SchemaCapabilities::SupportsInclusiveValueRangeConstraints()
{
    return false;
}

bool SchemaCapabilities::SupportsInheritance()
{
    return false;
}

bool SchemaCapabilities::SupportsMultipleSchemas()
{
    // NOTE: Currently, PostGIS provider supports single feature schema per datastore.
    //       Default name of schema is FdoPostGIS with class names formed
    //       according to following schema: datastore~tablename,
    //       ie. public~cities

    return false;
}

bool SchemaCapabilities::SupportsNetworkModel()
{
    // TODO: What's this about actually?

    return false;
}

bool SchemaCapabilities::SupportsNullValueConstraints()
{
    return true;
}

bool SchemaCapabilities::SupportsObjectProperties()
{
    return false;
}

bool SchemaCapabilities::SupportsSchemaModification()
{
    return true;
}

bool SchemaCapabilities::SupportsSchemaOverrides()
{
    return true;
}

bool SchemaCapabilities::SupportsUniqueValueConstraints()
{
    return true;
}

bool SchemaCapabilities::SupportsValueConstraintsList()
{
    // TODO: To be reviewed
    return false;
}

}} // namespace fdo::postgis
