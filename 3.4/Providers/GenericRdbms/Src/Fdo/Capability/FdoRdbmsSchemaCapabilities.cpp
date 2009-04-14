/*
 * 
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
#include "FdoRdbmsSchemaCapabilities.h"
#include "Sm/Lp/DataPropertyDefinition.h"

FdoRdbmsSchemaCapabilities::FdoRdbmsSchemaCapabilities(void)
{
}

FdoRdbmsSchemaCapabilities::~FdoRdbmsSchemaCapabilities(void)
{
}

void FdoRdbmsSchemaCapabilities::Dispose()
{
    delete this;
}


/// <summary>Returns an array of the FdoClassType objects the feature provider supports.</summary>
/// <param name="length">Output the number of class types.</param>
/// <returns>Returns the list of class types.</returns>
FdoClassType* FdoRdbmsSchemaCapabilities::GetClassTypes(FdoInt32& length)
{
    static FdoClassType classType[] = { FdoClassType_Class,
                                        FdoClassType_FeatureClass};
    length = sizeof(classType)/sizeof(FdoClassType);
    return classType;
}

/// <summary>Returns an array of the FdoDataType objects the feature provider supports.</summary>
/// <param name="length">Output the number of data types.</param>
/// <returns>Returns the list of data types.</returns>
FdoDataType* FdoRdbmsSchemaCapabilities::GetDataTypes(FdoInt32& length)
{
    static FdoDataType dataTypes[] = {  FdoDataType_Boolean,
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
    length = sizeof(dataTypes)/sizeof(FdoDataType);
    return dataTypes;
}

/// <summary>Returns the maximum supported length of String, BLOB, or CLOB data
/// properties. For decimal, it is the combination of the maximum scale
/// and precision. For other data types that are not variable in size,
/// the value returned is the byte length.</summary>
/// <param name="dataType">The data type for which the information is to retrieved.</param>
/// <returns>Returns the maximum data value length for the identified data type.</returns>
FdoInt64 FdoRdbmsSchemaCapabilities::GetMaximumDataValueLength(FdoDataType dataType)
{
    switch (dataType)
    {
        case FdoDataType_String:   return (FdoInt64)4000;
        case FdoDataType_BLOB:     return (FdoInt64)4000000000LL;
        case FdoDataType_CLOB:     return (FdoInt64)4000000000LL;
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
FdoInt32 FdoRdbmsSchemaCapabilities::GetMaximumDecimalPrecision()
{
    return (FdoInt32)38;
}

/// <summary>Returns the maximum supported scale for a decimal data property.</summary>
/// <returns>Returns the maximum supported scale for a decimal data property.</returns>
FdoInt32 FdoRdbmsSchemaCapabilities::GetMaximumDecimalScale()
{
    return (FdoInt32)127;
}

/// <summary>Returns the maximum size of a value of the given type. It includes
/// limits for the data store name, shema name, class name, property name
/// and description.</summary>
/// <param name="name">The schema element name type identifier for which the information is to
/// be retrieved. Can be either one of the following options: FdoSchemaElementNameType_Datastore,
/// FdoSchemaElementNameType_Schema, FdoSchemaElementNameType_Class, FdoSchemaElementNameType_Property or 
/// FdoSchemaElementNameType_Description.</param>
/// <returns>Returns the size limitation for the identified schema element.</returns>
FdoInt32 FdoRdbmsSchemaCapabilities::GetNameSizeLimit(FdoSchemaElementNameType name)
{
    switch (name)
    {
        case FdoSchemaElementNameType_Datastore:    return   30;
        case FdoSchemaElementNameType_Schema:       return  255;
        case FdoSchemaElementNameType_Class:        return  255;
        case FdoSchemaElementNameType_Property:     return  255;
        case FdoSchemaElementNameType_Description:  return  255;
    }

    return -1;
}

/// <summary>Returns a string that includes all the reserved characters that cannot be
/// used for the various schema element names for the provider.</summary>
/// <returns>Returns a string with the reserved characters for the identified schema element.</returns>
FdoString* FdoRdbmsSchemaCapabilities::GetReservedCharactersForName()
{
    return L".:";
}

/// <summary>Returns an array of the FdoDataType objects the feature provider supports for auto-generation.</summary>
/// <param name="length">Output the number of data types.</param>
/// <returns>Returns the list of data types.</returns>
FdoDataType* FdoRdbmsSchemaCapabilities::GetSupportedAutoGeneratedTypes(FdoInt32& length)
{
    return (FdoSmLpDataPropertyDefinition::GetSupportedAutoGeneratedTypes(length));
}

/// <summary>Returns a list of property types that can be used for identity properties.</summary>
/// <param name="length">Output the number of data types.</param>
/// <returns>Returns a string with the reserved characters for the identified schema element.</returns>
FdoDataType* FdoRdbmsSchemaCapabilities::GetSupportedIdentityPropertyTypes(FdoInt32& length)
{
	static FdoDataType supportedIdentityTypes[] = {	FdoDataType_Boolean,
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
	length = sizeof(supportedIdentityTypes)/sizeof(FdoDataType);
    return supportedIdentityTypes;
}

/// <summary>Determines if the provider supports association property.</summary>
/// <returns>Returns true if the provider support the association property.</returns>
bool FdoRdbmsSchemaCapabilities::SupportsAssociationProperties()
{
    return true;
}

/// <summary>Determines if the provider can auto-generate unique ID values for a class.</summary>
/// <returns>Returns true if the provider supports id autogeneration.</returns>
bool FdoRdbmsSchemaCapabilities::SupportsAutoIdGeneration()
{
    return true;
}

/// <summary>Determines if the provider supports multiple identity properties per class.</summary>
/// <returns>Returns true if the provider supports multiple identity properties per class.</returns>
bool FdoRdbmsSchemaCapabilities::SupportsCompositeId()
{
    return true;
}

/// <summary>Determines if the provider supports composite unique value constraint.</summary>
/// <returns>Returns true if the provider supports composite unique value constraint.</returns>
bool FdoRdbmsSchemaCapabilities::SupportsCompositeUniqueValueConstraints()
{
    return true;
}

/// <summary>Determines if the provider can auto-generate unique ID values that are unique for the
/// entire datastore, rather than just for a particular class.</summary>
/// <returns>Returns true if the provider supports id autogeneration unique across an datastore.</returns>
bool FdoRdbmsSchemaCapabilities::SupportsDataStoreScopeUniqueIdGeneration()
{
    return true;
}

/// <summary>Returns TRUE if default values can be specified for a data property
/// definition, FALSE otherwise.</summary>
/// <returns>Returns TRUE if default values can be specified for a data property definition, FALSE otherwise.</returns>
bool FdoRdbmsSchemaCapabilities::SupportsDefaultValue()
{
    return true;
}

/// <summary>Determines if the provider supports exclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports exclusive value range constraints.</returns>
bool FdoRdbmsSchemaCapabilities::SupportsExclusiveValueRangeConstraints()
{
    return true;
}

/// <summary>Determines if the provider supports inclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports inclusive value range constraints.</returns> 
bool FdoRdbmsSchemaCapabilities::SupportsInclusiveValueRangeConstraints()
{
    return true;
}

/// <summary>Determines if the feature provider supports inheritance.</summary>
/// <returns>Returns true if the feature provider supports inheritance.</returns> 
bool FdoRdbmsSchemaCapabilities::SupportsInheritance()
{
    return true;
}

/// <summary>Determines if the provider supports multiple schemas. Returns false if the provider supports
/// only a single schema.</summary>
/// <returns>Returns true if the provider supports multiple schemas.</returns> 
bool FdoRdbmsSchemaCapabilities::SupportsMultipleSchemas()
{
    return true;
}

/// <summary>Determines if the provider supports the network model.</summary>
/// <returns>Returns true if the provider supports the network model.</returns> 
bool FdoRdbmsSchemaCapabilities::SupportsNetworkModel()
{
    return false;
}

/// <summary>Determines if the provider supports nullable value constraint.</summary>
/// <returns>Returns true if the provider supports nullable value constraint.</returns>
bool FdoRdbmsSchemaCapabilities::SupportsNullValueConstraints()
{
    return true;
}

/// <summary>Determines if the provider supports object properties. Returns false if the provider
/// supports only data and geometric properties.</summary>
/// <returns>Returns true if the provider supports object properties.</returns> 
bool FdoRdbmsSchemaCapabilities::SupportsObjectProperties()
{
    return true;
}

/// <summary>Determines if the provider support update a schema through the ApplySchema command.
/// If False, then an application can only create a schema once for the provider. They can use the
/// ApplySchema command once for a given datastore. Once the schema is defined, it cannot be changed.
/// If True, then the provider supports the ability to modify or add to the schema after it is defined
/// initially.</summary>
/// <returns>Returns true if the provider supports the update of the schema.</returns> 
bool FdoRdbmsSchemaCapabilities::SupportsSchemaModification()
{
    return true;
}

/// <summary>Determines if the provider supports Schema Mapping Overrides (overrides to the default
/// rules for mapping Feature Schemas to provider-specific physical schemas).</summary>
/// <returns>Returns true if the provider supports Schema Overrides.</returns> 
bool FdoRdbmsSchemaCapabilities::SupportsSchemaOverrides()
{
    return true;
}

/// <summary>Determines if the provider supports unique value constraint.</summary>
/// <returns>Returns true if the provider supports unique value constraint.</returns>
bool FdoRdbmsSchemaCapabilities::SupportsUniqueValueConstraints()
{
    return true;
}

/// <summary>Determines if the provider supports value constraints list.</summary>
/// <returns>Returns true if the provider supports value constraints list.</returns>
bool FdoRdbmsSchemaCapabilities::SupportsValueConstraintsList()
{
    return true;
}
