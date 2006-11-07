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
  }
  
  return (FDO_SAFE_ADDREF(m_PropertyDictionary.p));
    
}//end of c_KgOraConnectionInfo::GetConnectionProperties 

