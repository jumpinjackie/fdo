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

FdoWfsConnectionCapabilities::FdoWfsConnectionCapabilities ()
{
}

FdoWfsConnectionCapabilities::~FdoWfsConnectionCapabilities ()
{
}

void FdoWfsConnectionCapabilities::Dispose()
{
    delete this;
}

/// <summary>Gets an FdoThreadCapability value that declares the feature provider's level of thread safety.</summary>
/// <returns>Returns the connection thread capability.</returns> 
FdoThreadCapability FdoWfsConnectionCapabilities::GetThreadCapability ()
{
    return (FdoThreadCapability_PerConnectionThreaded);
}

/// <summary>Gets the spatial context extent types supported by the feature provider.</summary>
/// <param name="length">Output the number of spatial context types.</param> 
/// <returns>Returns the list of spatial context extent types.</returns> 
FdoSpatialContextExtentType* FdoWfsConnectionCapabilities::GetSpatialContextTypes (FdoInt32& length)
{
    static FdoSpatialContextExtentType types[] =
    {
        FdoSpatialContextExtentType_Static
    };

    length = sizeof(types) / sizeof(FdoSpatialContextExtentType);

    return (types);
}

/// <summary>Determines if the feature provider supports persistent locking.</summary>
/// <returns>Returns true if the feature provider supports persistent locking.</returns> 
bool FdoWfsConnectionCapabilities::SupportsLocking ()
{
	// NOTE: Locking will be supported in Phase II. 	
    return (false);
}

FdoLockType* FdoWfsConnectionCapabilities::GetLockTypes(FdoInt32& size)
{
    size = 0;
    return ((FdoLockType*)NULL);
}

/// <summary>Determines if the feature provider supports connection timeout.</summary>
/// <returns>Returns true if the feature provider supports connection timeout.</returns> 
bool FdoWfsConnectionCapabilities::SupportsTimeout ()
{
    return (false);
}

/// <summary>Determines if the feature provider supports transactions.</summary>
/// <returns>Returns true if the feature provider supports transactions.</returns> 
bool FdoWfsConnectionCapabilities::SupportsTransactions ()
{
    return (false);
}

/// <summary>Determines true if the feature provider supports long transactions.</summary>
/// <returns>Returns true if the feature provider supports long transactions.</returns> 
bool FdoWfsConnectionCapabilities::SupportsLongTransactions ()
{
	// NOTE: Transaction will be supported in Phase II.	
    return (false);
}

/// <summary>Determines if the feature provider supports SQL commands.</summary>
/// <returns>Returns true if the feature provider supports SQL commands.</returns> 
bool FdoWfsConnectionCapabilities::SupportsSQL()
{
    return (false);
}

/// <summary>Determines if the feature provider supports XML configuration.</summary>
/// <returns>Returns true if the feature provider supports the setting of a configuration.</returns> 
bool FdoWfsConnectionCapabilities::SupportsConfiguration()
{
    return (false);
}

/// <summary>Determines if the provider supports multiple spatial contexts.</summary>
/// <returns>Returns true if the provider supports multiple spatial contexts.</returns> 
bool FdoWfsConnectionCapabilities::SupportsMultipleSpatialContexts()
{
    return true;
}

/// <summary>Determines if the provider supports specifying the coordinate system by name
/// or ID without specifying the WKT when creating a new spatial context.</summary>
/// <returns>Returns true if the provider supports specifying the coordinate system by name
/// or ID without specifying the WKT when creating a new spatial context.</returns> 
bool FdoWfsConnectionCapabilities::SupportsCSysWKTFromCSysName()
{
    return false;
}
