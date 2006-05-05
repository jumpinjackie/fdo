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

FdoWfsSchemaCapabilities::FdoWfsSchemaCapabilities()
{
}

FdoWfsSchemaCapabilities::~FdoWfsSchemaCapabilities()
{
}

void FdoWfsSchemaCapabilities::Dispose()
{
    delete this;
}

/// <summary>Returns an array of the FdoClassType objects the feature provider supports.</summary>
/// <param name="length">Output the number of class types.</param> 
/// <returns>Returns the list of class types.</returns> 
FdoClassType* FdoWfsSchemaCapabilities::GetClassTypes(FdoInt32& length)
{
    static FdoClassType classes[] =
    {
        FdoClassType_Class,
        FdoClassType_FeatureClass
    };

    length = sizeof (classes) / sizeof (FdoClassType);

    return (classes);
}

/// <summary>Returns an array of the FdoDataType objects the feature provider supports.</summary>
/// <param name="length">Output the number of data types.</param> 
/// <returns>Returns the list of data types</returns> 
FdoDataType* FdoWfsSchemaCapabilities::GetDataTypes(FdoInt32& length)
{
#pragma message ("ToDo: schema capabilities data types")
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
        FdoDataType_String,
//      FdoDataType_BLOB,		// TBD
//      FdoDataType_CLOB		// TBD
    };

    length = sizeof (types) / sizeof (FdoDataType);
    return (types);
}


/// <summary>Determines if the feature provider supports inheritance.</summary>
/// <returns>Returns true if the feature provider supports inheritance</returns> 
bool FdoWfsSchemaCapabilities::SupportsInheritance()
{
    return (true);
}


/// <summary>Determines if the provider supports multiple schemas. Returns false if the provider supports only a single schema.</summary>
/// <returns>Returns true if the provider supports multiple schemas.</returns> 
bool FdoWfsSchemaCapabilities::SupportsMultipleSchemas()
{
    return (true);
}


/// <summary>Determines if the provider supports object properties. Returns false if the provider supports only data and geometric properties.</summary>
/// <returns>Returns true if the provider supports object properties.</returns> 
bool FdoWfsSchemaCapabilities::SupportsObjectProperties()
{
    return (true);
}


/// <summary>Determines if the provider supports relations between classes.</summary>
/// <returns>Returns true if the provider supports relationships.</returns> 
bool FdoWfsSchemaCapabilities::SupportsRelations()
{
    return false;
}


/// <summary>Determines if the provider supports Schema Mapping Overrides 
/// (overrides to the default rules for mapping Feature Schemas to provider-specific
/// physical schemas)</summary>
/// <returns>Returns true if the provider supports Schema Overrides</returns> 
bool FdoWfsSchemaCapabilities::SupportsSchemaOverrides()
{
    return false;
}


/// <summary>Determines if the provider supports association property.</summary>
/// <returns>Returns true if the provider support the association property.</returns> 
bool FdoWfsSchemaCapabilities::SupportsAssociationProperties(void)
{
    return true;
}

/// <summary>Determines if the provider supports the network model.</summary>
/// <returns>Returns true if the provider supports the network model.</returns> 
bool FdoWfsSchemaCapabilities::SupportsNetworkModel(void)
{
    return false;
}

/// <summary>Determines if the provider can auto-generate unique ID values for a class.</summary>
/// <returns>Returns true if the provider supports id autogeneration.</returns> 
bool FdoWfsSchemaCapabilities::SupportsAutoIdGeneration()
{
    return false;
}

/// <summary>Determines if the provider can auto-generate unique ID values that are unique for the entire datastore, rather than just for a particular class.</summary>
/// <returns>Returns true if the provider supports id autogeneration unique across an datastore.</returns> 
bool FdoWfsSchemaCapabilities::SupportsDataStoreScopeUniqueIdGeneration()
{
    return false;
}

/// <summary>Returns an array of the FdoDataType objects the feature provider supports for auto-generation.</summary>
/// <param name="length">Output the number of data types.</param> 
/// <returns>Returns the list of data types</returns> 
FdoDataType* FdoWfsSchemaCapabilities::GetSupportedAutoGeneratedTypes(FdoInt32& length)
{
	length = 0;
    return NULL;
}

bool FdoWfsSchemaCapabilities::SupportsSchemaModification()
{
    return false;
}

/// <summary>Determines if the provider supports inclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports inclusive value range constraints.</returns> 
bool FdoWfsSchemaCapabilities::SupportsInclusiveValueRangeConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports exclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports exclusive value range constraints.</returns>
bool FdoWfsSchemaCapabilities::SupportsExclusiveValueRangeConstraints()
{
    return false;
}
/// <summary>Determines if the provider supports value constraints list.</summary>
/// <returns>Returns true if the provider supports value constraints list.</returns>
bool FdoWfsSchemaCapabilities::SupportsValueConstraintsList()
{
    return false;
}

/// <summary>Determines if the provider supports nullable value constraint.</summary>
/// <returns>Returns true if the provider supports nullable value constraint.</returns>
bool FdoWfsSchemaCapabilities::SupportsNullValueConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports unique value constraint.</summary>
/// <returns>Returns true if the provider supports unique value constraint.</returns>
bool FdoWfsSchemaCapabilities::SupportsUniqueValueConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports composite unique value constraint.</summary>
/// <returns>Returns true if the provider supports composite unique value constraint.</returns>
bool FdoWfsSchemaCapabilities::SupportsCompositeUniqueValueConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports multiple identity properties per class.</summary>
/// <returns>Returns true if the provider supports multiple identity properties per class.</returns>
bool FdoWfsSchemaCapabilities::SupportsCompositeId()
{
    return true;
}
