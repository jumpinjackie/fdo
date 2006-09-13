// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
class SdfConnectionCapabilities : public FdoIConnectionCapabilities
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // default constructor
        SdfConnectionCapabilities();

    protected:
        // default destructor
        virtual ~SdfConnectionCapabilities();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
        // dispose this object
        SDF_API virtual void Dispose();

    //-------------------------------------------------------
    // FdoIConnectionCapabilities implementation
    //-------------------------------------------------------

    public:
        // Gets a ThreadCapability value that declares the feature providers
        // level of thread safety.
        SDF_API virtual FdoThreadCapability GetThreadCapability();

        // Gets the list of spatial context extent types supported by the feature
        // provider.  The length parameter gives the number of types in the list.
        SDF_API virtual FdoSpatialContextExtentType* GetSpatialContextTypes(FdoInt32& length);

        // Returns true if the feature provider supports persistent locking.
        SDF_API virtual bool SupportsLocking();

        // Returns a list of supported lock types; a null list for SDF
        SDF_API virtual FdoLockType* GetLockTypes(FdoInt32& size);

        // Returns true if the feature provider supports connection timeout.
        SDF_API virtual bool SupportsTimeout();

        // Returns true if the feature provider supports transactions.
        SDF_API virtual bool SupportsTransactions();

        // Returns true if the feature provider supports long transactions.
        SDF_API virtual bool SupportsLongTransactions();

        // Returns true if the feature provider supports SQL commands.
        SDF_API virtual bool SupportsSQL();

        // Returns true if the feature provider supports configuration.
        SDF_API virtual bool SupportsConfiguration();

        // Returns true if the provider supports multiple spatial contexts.
        SDF_API virtual bool SupportsMultipleSpatialContexts();

        // Returns true if the provider supports specifying the coordinate system by name or ID without specifying the WKT
        // when creating a new spatial context.
        SDF_API virtual bool SupportsCSysWKTFromCSysName();

        /// \brief
        ///	Determines if write is supported by the provider or by the datastore depending on whether this request is at
        /// the provider or datastore level.
        ///
        /// \return
        ///	Returns true if write is supported by the provider or by the datastore depending on whether this request is at
        /// the provider or datastore level.
        SDF_API virtual bool SupportsWrite();

	    /// \brief
        /// Determines if the provider or datastore can support more than one user writing to a single datastore at
        /// one time.
        ///
        /// \return
        /// Returns true if the provider or datastore can support more than one user writing to a single datastore at
        /// one time.
        SDF_API virtual bool SupportsMultiUserWrite();
};
