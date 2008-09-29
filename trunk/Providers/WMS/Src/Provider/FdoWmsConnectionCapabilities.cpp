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
#include "FdoWmsConnectionCapabilities.h"

FdoWmsConnectionCapabilities::FdoWmsConnectionCapabilities ()
{
}

FdoWmsConnectionCapabilities::~FdoWmsConnectionCapabilities ()
{
}

void FdoWmsConnectionCapabilities::Dispose()
{
    delete this;
}

/// <summary>Gets an FdoThreadCapability value that declares the feature provider's level of thread safety.</summary>
/// <returns>Returns the connection thread capability.</returns> 
FdoThreadCapability FdoWmsConnectionCapabilities::GetThreadCapability ()
{
    return (FdoThreadCapability_PerConnectionThreaded);
}

/// <summary>Gets the spatial context extent types supported by the feature provider.</summary>
/// <param name="length">Output the number of spatial context types.</param> 
/// <returns>Returns the list of spatial context extent types.</returns> 
FdoSpatialContextExtentType* FdoWmsConnectionCapabilities::GetSpatialContextTypes (FdoInt32& length)
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
bool FdoWmsConnectionCapabilities::SupportsLocking ()
{
    return (false);
}

/// <summary>Determines the list of supported lock types.</summary>
/// <returns>Returns the list of supported lock types.</returns> 
FdoLockType* FdoWmsConnectionCapabilities::GetLockTypes(FdoInt32& size)
{
    size = 0;
    return ((FdoLockType*)NULL);
}

/// <summary>Determines if the feature provider supports connection timeout.</summary>
/// <returns>Returns true if the feature provider supports connection timeout.</returns> 
bool FdoWmsConnectionCapabilities::SupportsTimeout ()
{
    return (false);
}

/// <summary>Determines if the feature provider supports transactions.</summary>
/// <returns>Returns true if the feature provider supports transactions.</returns> 
bool FdoWmsConnectionCapabilities::SupportsTransactions ()
{
    return (false);
}

/// <summary>Determines true if the feature provider supports long transactions.</summary>
/// <returns>Returns true if the feature provider supports long transactions.</returns> 
bool FdoWmsConnectionCapabilities::SupportsLongTransactions ()
{
    return (false);
}

/// <summary>Determines if the feature provider supports SQL commands.</summary>
/// <returns>Returns true if the feature provider supports SQL commands.</returns> 
bool FdoWmsConnectionCapabilities::SupportsSQL()
{
    return (false);
}

/// <summary>Determines if the feature provider supports XML configuration.</summary>
/// <returns>Returns true if the feature provider supports the setting of a configuration.</returns> 
bool FdoWmsConnectionCapabilities::SupportsConfiguration()
{
    return (true);
}

/// <summary>Determines if the provider supports multiple spatial contexts.</summary>
/// <returns>Returns true if the provider supports multiple spatial contexts.</returns> 
bool FdoWmsConnectionCapabilities::SupportsMultipleSpatialContexts()
{
    return (true);
}

/// <summary>Determines if the provider supports specifying the coordinate system by name
/// or ID without specifying the WKT when creating a new spatial context.</summary>
/// <returns>Returns true if the provider supports specifying the coordinate system by name
/// or ID without specifying the WKT when creating a new spatial context.</returns> 
bool FdoWmsConnectionCapabilities::SupportsCSysWKTFromCSysName()
{
    return (false);
}

/// <summary>Determines if write is supported by the provider or by the datastore depending on
/// whether this request is at the provider or datastore level.</summary>
/// <returns>Returns true if the provider supports writing, false otherwise.</returns> 
bool FdoWmsConnectionCapabilities::SupportsWrite()
{
    return (false);
}

/// <summary>Determines if the provider or datastore can support more than one user writing to a
/// single datastore at one time.</summary>
/// <returns>Returns true if the provider supports multi-user writing, false otherwise.</returns> 
bool FdoWmsConnectionCapabilities::SupportsMultiUserWrite()
{
    return (false);
}
