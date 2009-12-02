/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

#ifndef FDORDBMSSQLSERVERSCHEMACAPABILITIES_H
#define FDORDBMSSQLSERVERSCHEMACAPABILITIES_H        1

#ifdef _WIN32
#pragma once
#endif

#include <../../src/Fdo/Capability/FdoRdbmsSchemaCapabilities.h>

class FdoRdbmsSqlServerSchemaCapabilities :    public FdoRdbmsSchemaCapabilities
{
public:
    FdoRdbmsSqlServerSchemaCapabilities(void) 
	{
	}

    /// <summary>Returns the maximum supported length of String, BLOB, or CLOB data
    /// properties. For decimal, it is the combination of the maximum scale
    /// and precision. For other data types that are not variable in size,
    /// the value returned is the byte length.</summary>
    /// <param name="dataType">The data type for which the information is to retrieved.</param>
    /// <returns>Returns the maximum data value length for the identified data type.</returns>
    FdoInt64 GetMaximumDataValueLength(FdoDataType dataType)
    {
        switch (dataType)
        {
            case FdoDataType_String:   return (FdoInt64)(2147483648 - 1);
            case FdoDataType_BLOB:     return (FdoInt64)-1;
            case FdoDataType_CLOB:     return (FdoInt64)-1;
            case FdoDataType_Decimal:  return (FdoInt64)(GetMaximumDecimalScale() + GetMaximumDecimalPrecision());
            case FdoDataType_Boolean:  return (FdoInt64)sizeof(FdoBoolean);
            case FdoDataType_Byte:     return (FdoInt64)sizeof(FdoByte);
            case FdoDataType_DateTime: return (FdoInt64)sizeof(FdoDateTime);
            case FdoDataType_Double:   return (FdoInt64)sizeof(FdoDouble);
            case FdoDataType_Int16:    return (FdoInt64)sizeof(FdoInt16);
            case FdoDataType_Int32:    return (FdoInt64)sizeof(FdoInt32);
            case FdoDataType_Int64:    return (FdoInt64)sizeof(FdoInt64);
            case FdoDataType_Single:   return (FdoInt64)sizeof(FdoFloat);
        }
        return (FdoInt64)-1;
    }

    /// <summary>Returns the maximum supported precision for a decimal data property.</summary>
	/// <returns>Returns the maximum supported precision for a decimal data property.</returns>
    virtual FdoInt32 GetMaximumDecimalPrecision()
    {
        return (FdoInt32)38;
    }

    /// <summary>Returns the maximum supported scale for a decimal data property.</summary>
	/// <returns>Returns the maximum supported scale for a decimal data property.</returns>
    virtual FdoInt32 GetMaximumDecimalScale()
    {
        return (FdoInt32)38;
    }

    /// <summary>Returns the maximum size of a value of the given type. It includes
    /// limits for the data store name, shema name, class name, property name
    /// and description.</summary>
    /// <param name="name">The schema element name type identifier for which the information is to
    /// be retrieved. Can be either one of the following options: FdoSchemaElementNameType_Datastore,
    /// FdoSchemaElementNameType_Schema, FdoSchemaElementNameType_Class, FdoSchemaElementNameType_Property or 
    /// FdoSchemaElementNameType_Description.</param>
	/// <returns>Returns the size limitation for the identified schema element.</returns>
    virtual FdoInt32 GetNameSizeLimit(FdoSchemaElementNameType name)
    {
        switch (name)
        {
            case FdoSchemaElementNameType_Datastore:   return  123;
            case FdoSchemaElementNameType_Schema:      return  200;
            case FdoSchemaElementNameType_Class:       return  200;
            case FdoSchemaElementNameType_Property:    return  200;
            case FdoSchemaElementNameType_Description: return  255;
        }
        return -1;
    }

	/// <summary>Determines if the provider supports exclusive value range constraints.</summary>
	/// <returns>Returns true if the provider supports exclusive value range constraints.</returns>
	virtual bool SupportsExclusiveValueRangeConstraints()
	{
		return true;
	}

	/// <summary>Determines if the provider supports inclusive value range constraints.</summary>
	/// <returns>Returns true if the provider supports inclusive value range constraints.</returns> 
	virtual bool SupportsInclusiveValueRangeConstraints()
	{
		return true;
	}

	/// <summary>Determines if the provider supports value constraints list.</summary>
	/// <returns>Returns true if the provider supports value constraints list.</returns>
	virtual bool SupportsValueConstraintsList()
	{
		return true;
	}

};

#endif // FDORDBMSQLSERVERSCHEMACAPABILITIES_H
