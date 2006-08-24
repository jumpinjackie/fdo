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
#include <FdoCommonStringUtil.h>
//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// constructs a SdfConnectionInfo using the specified connection
SdfConnectionInfo::SdfConnectionInfo(SdfConnection* connection)
{
    m_connection = connection;
    // NOTE: don't addref the connection because we don't own
    //       it (rather, it owns us)
}


// default destructor
SdfConnectionInfo::~SdfConnectionInfo()
{
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
    if (mPropertyDictionary == NULL)
    {
        wchar_t** pReadOnlyVals = new wchar_t*[2];
        pReadOnlyVals[0] = new wchar_t[6];
        pReadOnlyVals[1] = new wchar_t[5];
        wcscpy(pReadOnlyVals[0], RDONLY_FALSE);
        wcscpy(pReadOnlyVals[1], RDONLY_TRUE);
        mPropertyDictionary = new FdoCommonConnPropDictionary ((FdoIConnection*)m_connection);

        // Define all the connection properties:
        char* mbPropName = NULL;
        wide_to_multibyte(mbPropName, PROP_NAME_FILE);
        FdoPtr<ConnectionProperty> newProp = new ConnectionProperty (PROP_NAME_FILE,
                NlsMsgGetMain(SDFPROVIDER_48_PROP_NAME_FILE, mbPropName),
                EMPTY_VALUE, true, false, false, true, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(newProp);

        wide_to_multibyte(mbPropName, PROP_NAME_RDONLY);
        newProp = new ConnectionProperty (PROP_NAME_RDONLY,
                NlsMsgGetMain(SDFPROVIDER_49_PROP_NAME_READONLY, mbPropName),
                RDONLY_TRUE, false, false, true, false, false, false, false, 2, (const wchar_t**)pReadOnlyVals);
        newProp->SetValue(RDONLY_TRUE);
        mPropertyDictionary->AddProperty(newProp);
    }
    return (FDO_SAFE_ADDREF(mPropertyDictionary.p));
}

