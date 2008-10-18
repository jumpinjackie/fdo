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

#ifndef FDOWMSCONNECTIONCAPABILITIES_H
#define FDOWMSCONNECTIONCAPABILITIES_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWmsConnectionCapabilities : public FdoIConnectionCapabilities
{
public:
    // <summary>Default Constructor.</summary>
    // <returns>Returns nothing</returns> 
    FdoWmsConnectionCapabilities ();

protected:
    // <summary>Virtual Destructor.</summary>
    // <returns>Returns nothing</returns> 
    virtual ~FdoWmsConnectionCapabilities ();

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    virtual void Dispose ();

public:

    /// <summary>Gets an FdoThreadCapability value that declares the feature provider's level of thread safety.</summary>
    /// <returns>Returns the connection thread capability.</returns> 
    FdoThreadCapability GetThreadCapability ();

    /// <summary>Gets the spatial context extent types supported by the feature provider.</summary>
    /// <param name="length">Output the number of spatial context types.</param> 
    /// <returns>Returns the list of spatial context extent types.</returns> 
    FdoSpatialContextExtentType* GetSpatialContextTypes (FdoInt32& length);

    /// <summary>Determines if the feature provider supports persistent locking.</summary>
    /// <returns>Returns true if the feature provider supports persistent locking.</returns> 
    bool SupportsLocking ();

    /// <summary>Determines the list of supported lock types.</summary>
    /// <returns>Returns the list of supported lock types.</returns> 
    virtual FdoLockType* GetLockTypes(FdoInt32& size);

    /// <summary>Determines if the feature provider supports connection timeout.</summary>
    /// <returns>Returns true if the feature provider supports connection timeout.</returns> 
    bool SupportsTimeout ();

    /// <summary>Determines if the feature provider supports transactions.</summary>
    /// <returns>Returns true if the feature provider supports transactions.</returns> 
    bool SupportsTransactions ();

    /// <summary>Determines true if the feature provider supports long transactions.</summary>
    /// <remarks><span class="note">Note&nbsp;</span> Long Transaction commands are NOT supported 
    /// in this release.</remarks>
    /// <returns>Returns true if the feature provider supports long transactions.</returns> 
    bool SupportsLongTransactions ();

    /// <summary>Determines if the feature provider supports SQL commands.</summary>
    /// <returns>Returns true if the feature provider supports SQL commands.</returns> 
    bool SupportsSQL ();

    /// <summary>Determines if the feature provider supports XML configuration.</summary>
    /// <returns>Returns true if the feature provider supports the setting of a configuration.</returns> 
    bool SupportsConfiguration();

    /// <summary>Determines if the provider supports multiple spatial contexts.</summary>
    /// <returns>Returns true if the provider supports multiple spatial contexts.</returns> 
    bool SupportsMultipleSpatialContexts();

    /// <summary>Determines if the provider supports specifying the coordinate system by name
    /// or ID without specifying the WKT when creating a new spatial context.</summary>
    /// <returns>Returns true if the provider supports specifying the coordinate system by name
    /// or ID without specifying the WKT when creating a new spatial context.</returns> 
    bool SupportsCSysWKTFromCSysName();

    /// <summary>Determines if write is supported by the provider or by the datastore depending on
    /// whether this request is at the provider or datastore level.</summary>
    /// <returns>Returns true if the provider supports writing, false otherwise.</returns> 
    virtual bool SupportsWrite();

    /// <summary>Determines if the provider or datastore can support more than one user writing to a
    /// single datastore at one time.</summary>
    /// <returns>Returns true if the provider supports multi-user writing, false otherwise.</returns> 
    virtual bool SupportsMultiUserWrite();

    // \brief
    /// Determines if the provider can support the flush function. Flush is used to write any outstanding data
    /// to the datastore. This is mainly used by the file based providers to ensure that any cached data is writen to the file.
    ///
    /// \return
    /// Returns true if the provider or datastore can support the flush function.
    ///
    virtual bool SupportsFlush() {return false;}
};

#endif // FDOWMSCONNECTIONCAPABILITIES_H
