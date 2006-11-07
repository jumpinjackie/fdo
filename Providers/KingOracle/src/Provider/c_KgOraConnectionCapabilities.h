/*
* Copyright (C) 2006  SL-King d.o.o
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

#ifndef _c_KGORACONNECTIONCAPABILITIES_H
#define _c_KGORACONNECTIONCAPABILITIES_H


/// \brief
/// The FdoIConnectionCapabilities interface declares the feature provider's capabilities.
class c_KgOraConnectionCapabilities : public FdoIConnectionCapabilities
{
public:
    c_KgOraConnectionCapabilities ();

protected:
    virtual ~c_KgOraConnectionCapabilities ();

protected:
    virtual void Dispose ();

public:

    /// \brief
    /// Gets an FdoThreadCapability value that declares the feature provider's level of thread safety.
    /// 
    /// \return
    /// Returns the connection thread capability.
    /// 
    FDOKGORA_API virtual FdoThreadCapability GetThreadCapability();

    /// \brief
    /// Gets the spatial context extent types supported by the feature provider.
    /// 
    /// \param length 
    /// Output the number of spatial context types.
    /// 
    /// \return
    /// Returns the list of spatial context extent types.
    /// 
    FDOKGORA_API virtual FdoSpatialContextExtentType* GetSpatialContextTypes(GisInt32& Length);

    /// \brief
    /// Determines if the feature provider supports persistent locking.
    /// 
    /// \return
    /// Returns true if the feature provider supports persistent locking.
    /// 
    FDOKGORA_API virtual bool SupportsLocking();

    /// \brief
    /// Gets an array of the FdoLockType values supported by the feature provider.
    /// 
    /// \param size 
    /// Output the number of lock types
    /// 
    /// \return
    /// Returns the list of lock types
    /// 
    FDOKGORA_API virtual FdoLockType* GetLockTypes(GisInt32& Size);

    /// \brief
    /// Determines if the feature provider supports connection timeout.
    /// 
    /// \return
    /// Returns true if the feature provider supports connection timeout.
    /// 
    FDOKGORA_API virtual bool SupportsTimeout();

    /// \brief
    /// Determines if the feature provider supports transactions.
    /// 
    /// \return
    /// Returns true if the feature provider supports transactions.
    /// 
    FDOKGORA_API virtual bool SupportsTransactions();

    /// \brief
    /// Determines true if the feature provider supports long transactions.
    /// 
    /// \return
    /// Returns true if the feature provider supports long transactions.
    /// 
    FDOKGORA_API virtual bool SupportsLongTransactions();

    /// \brief
    /// Determines if the feature provider supports SQL commands.
    /// 
    /// \return
    /// Returns true if the feature provider supports SQL commands.
    /// 
    FDOKGORA_API virtual bool SupportsSQL();

    /// \brief
    /// Determines if the feature provider supports XML configuration.
    /// 
    /// \return
    /// Returns true if the feature provider supports the setting of a configuration.
    /// 
    FDOKGORA_API virtual bool SupportsConfiguration();

    /// \brief
    /// Determines if the provider supports multiple spatial contexts.
    /// 
    /// \return
    /// Returns true if the provider supports multiple spatial contexts.
    /// 
    FDOKGORA_API virtual bool SupportsMultipleSpatialContexts();

    /// \brief
    /// Determines if the provider supports specifying the coordinate system by name or ID without specifying the WKT
    /// when creating a new spatial context.
    /// 
    /// \return
    /// Returns true if the provider supports specifying the coordinate system by name or ID without specifying the WKT
    /// when creating a new spatial context.
    /// 
    FDOKGORA_API virtual bool SupportsCSysWKTFromCSysName();
};

#endif 

