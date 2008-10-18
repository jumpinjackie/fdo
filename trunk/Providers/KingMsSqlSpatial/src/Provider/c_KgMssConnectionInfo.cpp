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

#include <FdoCommonStringUtil.h>


c_KgMssConnectionInfo::c_KgMssConnectionInfo () : m_Connection(NULL)    
{
  
}

c_KgMssConnectionInfo::c_KgMssConnectionInfo (c_KgMssConnection* Conn) : m_Connection(Conn)    
{
  
}

void c_KgMssConnectionInfo::Dispose()
{
    delete this;
}



/// <summary>Gets the name of the feature provider.</summary>
/// <returns>Returns the provider name</returns>
FdoString* c_KgMssConnectionInfo::GetProviderName ()
{
    
    return (D_KGMSS_PROVIDER_NAME);
}

/// <summary>Gets the display name of the feature provider.</summary>
/// <returns>Returns the provider's display name</returns>
FdoString* c_KgMssConnectionInfo::GetProviderDisplayName()
{
    
    return D_KGMSS_PROVIDER_DEFAULT_DISPLAY_NAME;
}

/// <summary>Gets the description of the feature provider.</summary>
/// <returns>Returns the provider description</returns>
FdoString* c_KgMssConnectionInfo::GetProviderDescription ()
{
    
    return D_KGMSS_PROVIDER_DEFAULT_DESCRIPTION;
}

/// <summary>Gets the version of the feature provider.</summary>
/// <returns>Returns provider version</returns>
FdoString* c_KgMssConnectionInfo::GetProviderVersion ()
{
    
    return (D_KGMSS_PROVIDER_VERSION);
}

/// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
/// <returns>Returns FDO version supported.</returns>
FdoString* c_KgMssConnectionInfo::GetFeatureDataObjectsVersion ()
{
    
    return (D_KGMSS_FDO_VERSION);
}

#ifndef _FDO_3_2
/// \brief
/// Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.
/// 
/// \return
/// Returns the property dictionary
/// 
FdoIConnectionPropertyDictionary* c_KgMssConnectionInfo::GetConnectionProperties ()
{
    
  if (m_PropertyDictionary == NULL)
  {
    m_PropertyDictionary = new FdoCommonConnPropDictionary (m_Connection);

    char* propname;
        
		wide_to_multibyte (propname, D_CONN_PROPERTY_USERNAME);
    FdoPtr<ConnectionProperty>  newproperty = new ConnectionProperty (D_CONN_PROPERTY_USERNAME
                                                                    ,L"Username"
                                                                    ,L"", false, false, false, false, false, false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
        
    wide_to_multibyte (propname, D_CONN_PROPERTY_PASSWORD);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_PASSWORD,L"Password",
                L"", false, false, false, false, false, false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
                
		wide_to_multibyte (propname, D_CONN_PROPERTY_SERVICE_NAME);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_SERVICE_NAME,
                                            L"DSN",
                                            L"", false, false, false, false, false, false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
    
    wide_to_multibyte (propname, D_CONN_PROPERTY_TABLE_SCHEMA);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_TABLE_SCHEMA,
                                            L"Schema",
                                            L"", false, false, false, false, false, false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
    
    
    wide_to_multibyte (propname, D_CONN_PROPERTY_KING_FDO_CLASS);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_KING_FDO_CLASS,
                                            L"KingFdoClass",
                                            L"", false, false, false, false, false, false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
    
  }
  
  return (FDO_SAFE_ADDREF(m_PropertyDictionary.p));
    
}//end of c_KgMssConnectionInfo::GetConnectionProperties 

#endif

#ifdef _FDO_3_2

/// \brief
/// Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.
/// 
/// \return
/// Returns the property dictionary
/// 
FdoIConnectionPropertyDictionary* c_KgMssConnectionInfo::GetConnectionProperties ()
{
    
  if (m_PropertyDictionary == NULL)
  {
    m_PropertyDictionary = new FdoCommonConnPropDictionary (m_Connection);

    char* propname;
        
		wide_to_multibyte (propname, D_CONN_PROPERTY_USERNAME);
    FdoPtr<ConnectionProperty>  newproperty = new ConnectionProperty (D_CONN_PROPERTY_USERNAME
                                                                    ,L"Username"
                                                                    ,L"", false, false, false, false, false, false,false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
        
    wide_to_multibyte (propname, D_CONN_PROPERTY_PASSWORD);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_PASSWORD,L"Password",
                L"", false, false, false, false, false, false,false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
                
		wide_to_multibyte (propname, D_CONN_PROPERTY_SERVICE_NAME);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_SERVICE_NAME,
                                            L"DSN",
                                            L"", false, false, false, false, false, false,false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
    
    wide_to_multibyte (propname, D_CONN_PROPERTY_TABLE_SCHEMA);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_TABLE_SCHEMA,
                                            L"Schema",
                                            L"", false, false, false, false, false, false,false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
    
    
    wide_to_multibyte (propname, D_CONN_PROPERTY_KING_FDO_CLASS);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_KING_FDO_CLASS,
                                            L"KingFdoClass",
                                            L"", false, false, false, false, false, false,false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
    
  }
  
  return (FDO_SAFE_ADDREF(m_PropertyDictionary.p));
    
}//end of c_KgMssConnectionInfo::GetConnectionProperties 

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
FdoProviderDatastoreType c_KgMssConnectionInfo::GetProviderDatastoreType()
{
  return FdoProviderDatastoreType_DatabaseServer;
}

/// \brief
/// File-based providers depend on a various files. This function returns a list
/// of fully qualified dependend file names. The return parameter will be NULL if
/// the provider is not a file-based provider.
/// 
/// \return
/// Returns the list of fully-qualified dependend file names if the provider is a
/// file-based provider, NULL otherwise.
/// 
FdoStringCollection* c_KgMssConnectionInfo::GetDependentFileNames()
{
  return NULL;
}

#endif