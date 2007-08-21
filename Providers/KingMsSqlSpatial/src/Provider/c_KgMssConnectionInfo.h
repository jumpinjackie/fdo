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

#ifndef _c_KgMssConnectionInfo_h
#define _c_KgMssConnectionInfo_h


/// \brief
/// The FdoIConnectionInfo interface exposes information about the feature provider
/// and provides access to the FdoIConnectionPropertyDictionary interface.
class c_KgMssConnectionInfo : public FdoIConnectionInfo
{
    

public:
    c_KgMssConnectionInfo (c_KgMssConnection * m_Connection); 
    c_KgMssConnectionInfo (); 
    
    
protected:
    
    c_KgMssConnection * m_Connection;
    FdoPtr<FdoCommonConnPropDictionary> m_PropertyDictionary; // connection parameters        

protected:
    
    virtual void Dispose ();
   

public:
    /// <summary>Gets the name of the feature provider.</summary>
    /// <returns>Returns the provider name</returns>
    FDOKGMSS_API FdoString* GetProviderName ();

    /// <summary>Gets the display name of the feature provider.</summary>
    /// <returns>Returns the provider's display name</returns>
    FDOKGMSS_API FdoString* GetProviderDisplayName();

    /// <summary>Gets the description of the feature provider.</summary>
    /// <returns>Returns the provider description</returns>
    FDOKGMSS_API FdoString* GetProviderDescription ();

    /// <summary>Gets the version of the feature provider.</summary>
    /// <returns>Returns provider version</returns>
    FDOKGMSS_API FdoString* GetProviderVersion ();

    /// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
    /// <returns>Returns FDO version supported.</returns>
    FDOKGMSS_API FdoString* GetFeatureDataObjectsVersion ();

    /// <summary>Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.</summary>
    /// <returns>Returns the property dictionary</returns>
    FDOKGMSS_API FdoIConnectionPropertyDictionary* GetConnectionProperties ();
    
    #ifdef _FDO_3_2
    /// \brief
    /// Returns the provider type. A provider can be a file-based, database-based or
    /// web-based provider. The valid values the function may return are defined in
    /// the enumeration FdoProviderDatastoreType. The enumeration includes the following
    /// values: FdoProviderDatastoreType_Unknown, FdoProviderDatastoreType_File,
    /// FdoProviderDatastoreType_DatabaseServer, FdoProviderDatastoreType_WebServer.
    /// 
    /// \return
    /// Returns the provider data store type.
    /// 
    FDOKGMSS_API virtual FdoProviderDatastoreType GetProviderDatastoreType();

    /// \brief
    /// File-based providers depend on a various files. This function returns a list
    /// of fully qualified dependend file names. The return parameter will be NULL if
    /// the provider is not a file-based provider.
    /// 
    /// \return
    /// Returns the list of fully-qualified dependend file names if the provider is a
    /// file-based provider, NULL otherwise.
    /// 
    FDOKGMSS_API virtual FdoStringCollection* GetDependentFileNames();
    #endif

};

#endif 

