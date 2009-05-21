/*
* Copyright (C) 2007  Haris Kurtagic
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
*/

#include "stdafx.h"

c_KgInfConnectionCapabilities::c_KgInfConnectionCapabilities ()
{
}

c_KgInfConnectionCapabilities::~c_KgInfConnectionCapabilities ()
{
}

void c_KgInfConnectionCapabilities::Dispose()
{
    delete this;
}

/// <summary>Gets an FdoThreadCapability value that declares the feature provider's level of thread safety.</summary>
/// <returns>Returns the connection thread capability.</returns> 
FdoThreadCapability c_KgInfConnectionCapabilities::GetThreadCapability ()
{
    return (FdoThreadCapability_PerConnectionThreaded);
}

/// <summary>Gets the spatial context extent types supported by the feature provider.</summary>
/// <param name="length">Output the number of spatial context types.</param> 
/// <returns>Returns the list of spatial context extent types.</returns> 
FdoSpatialContextExtentType* c_KgInfConnectionCapabilities::GetSpatialContextTypes (FdoInt32& Length)
{
    static FdoSpatialContextExtentType types[] =
    {
        FdoSpatialContextExtentType_Static
    };

    Length = sizeof(types) / sizeof(FdoSpatialContextExtentType);

    return (types);
}

/// <summary>Determines if the feature provider supports persistent locking.</summary>
/// <returns>Returns true if the feature provider supports persistent locking.</returns> 
bool c_KgInfConnectionCapabilities::SupportsLocking ()
{
    return (false);
}

/// <summary>Gets an array of the FdoLockType values supported by the feature provider.</summary>
/// <param name="size">Output the number of lock types.</param>
/// <returns>Returns the list of lock types.</returns> 
FdoLockType* c_KgInfConnectionCapabilities::GetLockTypes(FdoInt32& Size)
{ 
  
    static FdoLockType locktypes[] = {
        FdoLockType_Shared,
        FdoLockType_Exclusive,
        FdoLockType_Transaction,
        FdoLockType_AllLongTransactionExclusive,
        FdoLockType_LongTransactionExclusive
    };

    Size = sizeof(locktypes)/sizeof(FdoLockType);
    return locktypes;
    
}

/// <summary>Determines if the feature provider supports connection timeout.</summary>
/// <returns>Returns true if the feature provider supports connection timeout.</returns> 
bool c_KgInfConnectionCapabilities::SupportsTimeout ()
{
    return false;
}

/// <summary>Determines if the feature provider supports transactions.</summary>
/// <returns>Returns true if the feature provider supports transactions.</returns> 
bool c_KgInfConnectionCapabilities::SupportsTransactions ()
{
    return false;
}

/// <summary>Determines true if the feature provider supports long transactions.</summary>
/// <returns>Returns true if the feature provider supports long transactions.</returns> 
bool c_KgInfConnectionCapabilities::SupportsLongTransactions ()
{
    return false;
}

/// <summary>Determines if the feature provider supports SQL commands.</summary>
/// <returns>Returns true if the feature provider supports SQL commands.</returns> 
bool c_KgInfConnectionCapabilities::SupportsSQL()
{
    return true;
}

/// <summary>Determines if the feature provider supports XML configuration.</summary>
/// <returns>Returns true if the feature provider supports the setting of a configuration.</returns> 
bool c_KgInfConnectionCapabilities::SupportsConfiguration()
{
    return true;
}

/// <summary>Determines if the provider supports multiple spatial contexts.</summary>
/// <returns>Returns true if the provider supports multiple spatial contexts.</returns> 
bool c_KgInfConnectionCapabilities::SupportsMultipleSpatialContexts()
{
    return true;
}

/// <summary>Determines if the provider supports specifying the coordinate system by name
/// or ID without specifying the WKT when creating a new spatial context.</summary>
/// <returns>Returns true if the provider supports specifying the coordinate system by name
/// or ID without specifying the WKT when creating a new spatial context.</returns> 
bool c_KgInfConnectionCapabilities::SupportsCSysWKTFromCSysName()
{
    return false;
}

#ifdef _FDO_3_2
/// \brief
///	Determines if write is supported by the provider or by the datastore depending on whether this request is at
/// the provider or datastore level.
///
/// \return
///	Returns true if write is supported by the provider or by the datastore depending on whether this request is at
/// the provider or datastore level.
bool c_KgInfConnectionCapabilities::SupportsWrite()
{
  return true;
}

/// \brief
/// Determines if the provider or datastore can support more than one user writing to a single datastore at
/// one time.
///
/// \return
/// Returns true if the provider or datastore can support more than one user writing to a single datastore at
/// one time.
bool c_KgInfConnectionCapabilities::SupportsMultiUserWrite()
{
  return true;
}

/// \brief
/// Determines if the provider can support the flush function. Flush is used to write any outstanding data
/// to the datastore. This is mainly used by the file based providers to ensure that any cached data is writen to the file.
///
/// \return
/// Returns true if the provider or datastore can support the flush function.
///
bool c_KgInfConnectionCapabilities::SupportsFlush()
{
  return false;
}
#endif