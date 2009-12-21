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


#include "stdafx.h"

#include <FdoCommonStringUtil.h>


c_KgOraConnectionInfo::c_KgOraConnectionInfo () : m_Connection(NULL)    
{
  
}

c_KgOraConnectionInfo::c_KgOraConnectionInfo (c_KgOraConnection* Conn) : m_Connection(Conn)    
{
  
}

void c_KgOraConnectionInfo::Dispose()
{
    delete this;
}



/// <summary>Gets the name of the feature provider.</summary>
/// <returns>Returns the provider name</returns>
FdoString* c_KgOraConnectionInfo::GetProviderName ()
{
    
    return (D_KGORA_PROVIDER_NAME);
}

/// <summary>Gets the display name of the feature provider.</summary>
/// <returns>Returns the provider's display name</returns>
FdoString* c_KgOraConnectionInfo::GetProviderDisplayName()
{
    
    return NlsMsgGet(M_KGORA_PROVIDER_DISPLAY_NAME, D_KGORA_PROVIDER_DEFAULT_DISPLAY_NAME);
}

/// <summary>Gets the description of the feature provider.</summary>
/// <returns>Returns the provider description</returns>
FdoString* c_KgOraConnectionInfo::GetProviderDescription ()
{
    
    return NlsMsgGet(M_KGORA_PROVIDER_DESCRIPTION, D_KGORA_PROVIDER_DEFAULT_DESCRIPTION);
}

/// <summary>Gets the version of the feature provider.</summary>
/// <returns>Returns provider version</returns>
FdoString* c_KgOraConnectionInfo::GetProviderVersion ()
{
    
    return (D_KGORA_PROVIDER_VERSION);
}

/// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
/// <returns>Returns FDO version supported.</returns>
FdoString* c_KgOraConnectionInfo::GetFeatureDataObjectsVersion ()
{
    
    return (D_KGORA_FDO_VERSION);
}

#ifndef _FDO_3_2
/// \brief
/// Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.
/// 
/// \return
/// Returns the property dictionary
/// 
FdoIConnectionPropertyDictionary* c_KgOraConnectionInfo::GetConnectionProperties ()
{
    
  if (m_PropertyDictionary == NULL)
  {
    m_PropertyDictionary = new FdoCommonConnPropDictionary (m_Connection);

    char* propname;
        
		wide_to_multibyte (propname, D_CONN_PROPERTY_USERNAME);
    FdoPtr<ConnectionProperty>  newproperty = new ConnectionProperty (D_CONN_PROPERTY_USERNAME
                                                                    ,NlsMsgGet(M_KGORA_CONNECTION_PROPERTY_USERNAME, propname)
                                                                    ,L"", false, false, false, false, false, false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
        
    wide_to_multibyte (propname, D_CONN_PROPERTY_PASSWORD);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_PASSWORD,NlsMsgGet(M_KGORA_CONNECTION_PROPERTY_PASSWORD, propname),
                L"", false, false, false, false, false, false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
                
		wide_to_multibyte (propname, D_CONN_PROPERTY_SERVICE_NAME);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_SERVICE_NAME,
                                            NlsMsgGet(M_KGORA_CONNECTION_PROPERTY_SERVICE_NAME, propname),
                                            L"", false, false, false, false, false, false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
    
    wide_to_multibyte (propname, D_CONN_PROPERTY_ORACLE_SCHEMA);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_ORACLE_SCHEMA,
                                            NlsMsgGet(M_KGORA_CONNECTION_PROPERTY_ORACLE_SCHEMA, propname),
                                            L"", false, false, false, false, false, false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
    
    wide_to_multibyte (propname, D_CONN_PROPERTY_KING_FDO_CLASS);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_KING_FDO_CLASS,
                                            NlsMsgGet(M_KGORA_CONNECTION_PROPERTY_FDO_VIEWS_TABLE, propname),
                                            L"", false, false, false, false, false, false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
  }
  
  return (FDO_SAFE_ADDREF(m_PropertyDictionary.p));
    
}//end of c_KgOraConnectionInfo::GetConnectionProperties 

#endif

#ifdef _FDO_3_2

/// \brief
/// Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.
/// 
/// \return
/// Returns the property dictionary
/// 
FdoIConnectionPropertyDictionary* c_KgOraConnectionInfo::GetConnectionProperties ()
{
    
  if (m_PropertyDictionary == NULL)
  {
    m_PropertyDictionary = new FdoCommonConnPropDictionary (m_Connection);

    char* propname;
        
		wide_to_multibyte (propname, D_CONN_PROPERTY_USERNAME);
    FdoPtr<ConnectionProperty>  newproperty = new ConnectionProperty (D_CONN_PROPERTY_USERNAME
                                                                    ,NlsMsgGet(M_KGORA_CONNECTION_PROPERTY_USERNAME, propname)
                                                                    ,L"", false, false, false, false, false, false,false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
        
    wide_to_multibyte (propname, D_CONN_PROPERTY_PASSWORD);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_PASSWORD,NlsMsgGet(M_KGORA_CONNECTION_PROPERTY_PASSWORD, propname),
                L"", false, false, false, false, false, false,false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
                
		wide_to_multibyte (propname, D_CONN_PROPERTY_SERVICE_NAME);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_SERVICE_NAME,
                                            NlsMsgGet(M_KGORA_CONNECTION_PROPERTY_SERVICE_NAME, propname),
                                            L"", false, false, false, false, false, false,false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
    
    wide_to_multibyte (propname, D_CONN_PROPERTY_ORACLE_SCHEMA);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_ORACLE_SCHEMA,
                                            NlsMsgGet(M_KGORA_CONNECTION_PROPERTY_ORACLE_SCHEMA, propname),
                                            L"", false, false, false, false, false, false,false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
    
    wide_to_multibyte (propname, D_CONN_PROPERTY_KING_FDO_CLASS);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_KING_FDO_CLASS,
                                            NlsMsgGet(M_KGORA_CONNECTION_PROPERTY_FDO_VIEWS_TABLE, propname),
                                            L"", false, false, false, false, false, false,false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
    
    wide_to_multibyte (propname, D_CONN_PROPERTY_SDE_SCHEMA);
    newproperty = new ConnectionProperty (D_CONN_PROPERTY_SDE_SCHEMA,
      NlsMsgGet(M_KGORA_CONNECTION_PROPERTY_SDE_SCHEMA, propname),
      L"", false, false, false, false, false, false,false, 0, NULL);
    m_PropertyDictionary->AddProperty(newproperty);
  }
  
  return (FDO_SAFE_ADDREF(m_PropertyDictionary.p));
    
}//end of c_KgOraConnectionInfo::GetConnectionProperties 

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
FdoProviderDatastoreType c_KgOraConnectionInfo::GetProviderDatastoreType()
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
FdoStringCollection* c_KgOraConnectionInfo::GetDependentFileNames()
{
  return NULL;
}

#endif
