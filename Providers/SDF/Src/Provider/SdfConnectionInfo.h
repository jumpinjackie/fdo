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
//  
#ifndef SDFCONNECTIONINFO_H
#define SDFCONNECTIONINFO_H

class SdfConnectionInfo : public FdoIConnectionInfo
{
    friend class SdfConnection;

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

    private:
    /**
     * The connection that spawned us.
     */
        SdfConnection* m_connection;
    /**
     * The dictionary of connection parameters.
     */
    FdoPtr<FdoCommonConnPropDictionary> mPropertyDictionary;

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // constructs a SdfConnectionInfo using the specified connection
        SdfConnectionInfo(SdfConnection* connection);

    protected:
        // default destructor
        virtual ~SdfConnectionInfo();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
        // dispose this object
        SDF_API virtual void Dispose();

    //-------------------------------------------------------
    // FdoIConnectionInfo implementation
    //-------------------------------------------------------

    public:
        // Returns the name of the feature provider.
        SDF_API virtual FdoString* GetProviderName();

        // Returns the provider's display name
        SDF_API virtual FdoString* GetProviderDisplayName();

        // Returns the description of the feature provider.
        SDF_API virtual FdoString* GetProviderDescription();

        // Returns the version of the feature provider.
        SDF_API virtual FdoString* GetProviderVersion();

        // Returns the version of the Feature Data Objects specification
        // this provider conforms to.
        SDF_API virtual FdoString* GetFeatureDataObjectsVersion();

        // Returns the IConnectionPropertyDictionary interface that can be used
        // to dynamically query and set the properties required to establish a
        // connection.
        SDF_API virtual FdoIConnectionPropertyDictionary* GetConnectionProperties();

        // Returns the provider type. A provider can be a file-based, database-based or
        // web-based provider. The valid values the function may return are defined in
        // the enumeration FdoProviderDatastoreType. The enumeration includes the following
        // values: FdoProviderDatastoreType_Unknown, FdoProviderDatastoreType_File,
        // FdoProviderDatastoreType_DatabaseServer, FdoProviderDatastoreType_WebServer.
        SDF_API virtual FdoProviderDatastoreType GetProviderDatastoreType();

        // File-based providers depend on a various files. This function returns a list
        // of fully qualified dependend file names. The return parameter will be NULL if
        // the provider is not a file-based provider.
        SDF_API virtual FdoStringCollection* GetDependentFileNames();

    //-------------------------------------------------------
    // SdfConnectionInfo implementation
    //-------------------------------------------------------
};


#endif

