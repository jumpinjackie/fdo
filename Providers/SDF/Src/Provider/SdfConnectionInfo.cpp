// 
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
#include "stdafx.h"
#include "SdfConnectionInfo.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// constructs a SdfConnectionInfo using the specified connection
SdfConnectionInfo::SdfConnectionInfo(SdfConnection* connection)
{
    m_connection = connection;
    m_PropertyDictionary = NULL;

    // NOTE: don't addref the connection because we don't own
    //       it (rather, it owns us)
}


// default destructor
SdfConnectionInfo::~SdfConnectionInfo()
{
    FDO_SAFE_RELEASE(m_PropertyDictionary);
}


//-------------------------------------------------------
// FdoIDisposable implementation
//-------------------------------------------------------


// dispose this object
void SdfConnectionInfo::Dispose()
{
    delete this;
}


//-------------------------------------------------------
// FdoIConnectionInfo implementation
//-------------------------------------------------------


// Returns the name of the feature provider.
FdoString* SdfConnectionInfo::GetProviderName()
{
    return SDF_PROVIDER_NAME;
}

// Returns the provider's display name
FdoString* SdfConnectionInfo::GetProviderDisplayName()
{
    return NlsMsgGetMain(SDF_PROVIDER_DISPLAY_NAME, SDF_PROVIDER_DEFAULT_DISPLAY_NAME);
}

// Returns the description of the feature provider.
FdoString* SdfConnectionInfo::GetProviderDescription()
{
    return NlsMsgGetMain(SDF_PROVIDER_DESCRIPTION, SDF_PROVIDER_DEFAULT_DESCRIPTION);
}


// Returns the version of the feature provider.
FdoString* SdfConnectionInfo::GetProviderVersion()
{
    return SDF_PROVIDER_VERSION;
}


// Returns the version of the Feature Data Objects specification
// this provider conforms to.
FdoString* SdfConnectionInfo::GetFeatureDataObjectsVersion()
{
    return SDF_FDO_VERSION;
}


// Returns the IConnectionPropertyDictionary interface that can be used
// to dynamically query and set the properties required to establish a
// connection.
FdoIConnectionPropertyDictionary* SdfConnectionInfo::GetConnectionProperties()
{
    if (m_PropertyDictionary == NULL)
        m_PropertyDictionary = new SdfConnectionPropertyDictionary(m_connection);

    // addref the dictionary because we own it
    FDO_SAFE_ADDREF(m_PropertyDictionary);
    return (FdoIConnectionPropertyDictionary *)m_PropertyDictionary;
}

