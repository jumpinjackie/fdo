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

#ifndef _c_KGORACONNECTIONCAPABILITIES_H
#define _c_KGORACONNECTIONCAPABILITIES_H


/// \brief
/// The FdoIConnectionCapabilities interface declares the feature provider's capabilities.
class c_KgMssConnectionCapabilities : public FdoIConnectionCapabilities
{
public:
    c_KgMssConnectionCapabilities ();

protected:
    virtual ~c_KgMssConnectionCapabilities ();

protected:
    virtual void Dispose ();

public:

    /// \brief
    /// Gets an FdoThreadCapability value that declares the feature provider's level of thread safety.
    /// 
    /// \return
    /// Returns the connection thread capability.
    /// 
    FDOKGMSS_API virtual FdoThreadCapability GetThreadCapability();

    /// \brief
    /// Gets the spatial context extent types supported by the feature provider.
    /// 
    /// \param length 
    /// Output the number of spatial context types.
    /// 
    /// \return
    /// Returns the list of spatial context extent types.
    /// 
    FDOKGMSS_API virtual FdoSpatialContextExtentType* GetSpatialContextTypes(FdoInt32& Length);

    /// \brief
    /// Determines if the feature provider supports persistent locking.
    /// 
    /// \return
    /// Returns true if the feature provider supports persistent locking.
    /// 
    FDOKGMSS_API virtual bool SupportsLocking();

    /// \brief
    /// Gets an array of the FdoLockType values supported by the feature provider.
    /// 
    /// \param size 
    /// Output the number of lock types
    /// 
    /// \return
    /// Returns the list of lock types
    /// 
    FDOKGMSS_API virtual FdoLockType* GetLockTypes(FdoInt32& Size);

    /// \brief
    /// Determines if the feature provider supports connection timeout.
    /// 
    /// \return
    /// Returns true if the feature provider supports connection timeout.
    /// 
    FDOKGMSS_API virtual bool SupportsTimeout();

    /// \brief
    /// Determines if the feature provider supports transactions.
    /// 
    /// \return
    /// Returns true if the feature provider supports transactions.
    /// 
    FDOKGMSS_API virtual bool SupportsTransactions();

    /// \brief
    /// Determines true if the feature provider supports long transactions.
    /// 
    /// \return
    /// Returns true if the feature provider supports long transactions.
    /// 
    FDOKGMSS_API virtual bool SupportsLongTransactions();

    /// \brief
    /// Determines if the feature provider supports SQL commands.
    /// 
    /// \return
    /// Returns true if the feature provider supports SQL commands.
    /// 
    FDOKGMSS_API virtual bool SupportsSQL();

    /// \brief
    /// Determines if the feature provider supports XML configuration.
    /// 
    /// \return
    /// Returns true if the feature provider supports the setting of a configuration.
    /// 
    FDOKGMSS_API virtual bool SupportsConfiguration();

    /// \brief
    /// Determines if the provider supports multiple spatial contexts.
    /// 
    /// \return
    /// Returns true if the provider supports multiple spatial contexts.
    /// 
    FDOKGMSS_API virtual bool SupportsMultipleSpatialContexts();

    /// \brief
    /// Determines if the provider supports specifying the coordinate system by name or ID without specifying the WKT
    /// when creating a new spatial context.
    /// 
    /// \return
    /// Returns true if the provider supports specifying the coordinate system by name or ID without specifying the WKT
    /// when creating a new spatial context.
    /// 
    FDOKGMSS_API virtual bool SupportsCSysWKTFromCSysName();
    
    #ifdef _FDO_3_2
    /// \brief
    ///	Determines if write is supported by the provider or by the datastore depending on whether this request is at
    /// the provider or datastore level.
    ///
    /// \return
    ///	Returns true if write is supported by the provider or by the datastore depending on whether this request is at
    /// the provider or datastore level.
    FDOKGMSS_API virtual bool SupportsWrite();

	/// \brief
    /// Determines if the provider or datastore can support more than one user writing to a single datastore at
    /// one time.
    ///
    /// \return
    /// Returns true if the provider or datastore can support more than one user writing to a single datastore at
    /// one time.
    FDOKGMSS_API virtual bool SupportsMultiUserWrite();

    /// \brief
    /// Determines if the provider can support the flush function. Flush is used to write any outstanding data
    /// to the datastore. This is mainly used by the file based providers to ensure that any cached data is writen to the file.
    ///
    /// \return
    /// Returns true if the provider or datastore can support the flush function.
    ///
    FDOKGMSS_API virtual bool SupportsFlush();
    #endif
};

#endif 

