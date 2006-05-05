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
#include "SdfPropertyDictionary.h"
#include <map>

#include "SdfConnection.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------

// constructs a SdfPropertyDictionary using the specified connection
SdfPropertyDictionary::SdfPropertyDictionary(SdfConnection* connection)
{
    // NOTE: don't addref the connection because we don't own
    //       it (rather, it owns us)
    m_connection = connection;

    m_mProps = new std::map<std::wstring, std::wstring>();
}


// default destructor
SdfPropertyDictionary::~SdfPropertyDictionary()
{
    delete m_mProps;
}


//-------------------------------------------------------
// GisIDisposable implementation
//-------------------------------------------------------


// dispose this object
void SdfPropertyDictionary::Dispose()
{
    delete this;
}


//-------------------------------------------------------
// FdoIConnectionPropertyDictionary implementation
//-------------------------------------------------------


//// Gets the names of all the properties that can appear in a connection string
//// for this feature provider as an array of Strings.  The order of the property
//// names in the resulting array dictate the order in which they need to be
//// specified.  This is especially important for the success of the
//// EnumeratePropertyValues method as properties that occur earlier in the array
//// may be required for successful enumeration of properties that appear later
//// in the array.  The count parameter returns the size of the returned array.
//FdoString** SdfPropertyDictionary::GetPropertyNames(FdoInt32& count)
//{
//    count = 2;
//    return (const wchar_t**)PROP_NAMES;
//}


// Gets the value of the specified property.
FdoString* SdfPropertyDictionary::GetProperty(FdoString* name)
{
    //TODO: should we do error checking?
    //as it is, it will return empty if the property is not there
    //so it is not fatal to not check for the property name being valid
    return (*m_mProps)[name].c_str();
}


// Sets the value of the specified property.  An exception will be thrown if
// the connection is currently open.
void SdfPropertyDictionary::SetProperty(FdoString* name, FdoString* value)
{
    // check the connection
    if (m_connection->GetConnectionState() != FdoConnectionState_Closed)
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_30_CONNECTION_OPEN)));

    // validate input
    if (value == NULL && (wcscmp(name, PROP_NAME_FILE) == 0))
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_31_PROPERTY_NULL)));

    if (wcscmp(name, PROP_NAME_FILE) == 0)
    {
        (*m_mProps)[name] = value;
    }
    else if (wcscmp(name, PROP_NAME_RDONLY) == 0)
    {
        if (_wcsnicmp(RDONLY_FALSE, value, wcslen(RDONLY_FALSE)) != 0
            && _wcsnicmp(RDONLY_TRUE, value, wcslen(RDONLY_TRUE)) != 0)
            throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_32_INVALID_PROPERTY_VALUE)));

        (*m_mProps)[name] = value;
    }
    else
    {
        //some other custom property the user added...
        if (value)
            (*m_mProps)[name] = value;
        else
            (*m_mProps)[name] = EMPTY_VALUE;
    }
}


// Gets the default value for the specified property.
FdoString* SdfPropertyDictionary::GetPropertyDefault(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FILE) == 0)
        return EMPTY_VALUE;
    else if (wcscmp(name, PROP_NAME_RDONLY) == 0)
        return RDONLY_FALSE;

    return EMPTY_VALUE;
}


// Returns true if the specified property is required, false if it is optional.
bool SdfPropertyDictionary::IsPropertyRequired(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FILE) == 0)
        return true;
    else if (wcscmp(name, PROP_NAME_RDONLY) == 0)
        return false;

    return false;
}


// Indicates if the property is a password or other protected field
// that should be kept secure.
bool SdfPropertyDictionary::IsPropertyProtected(FdoString* name)
{
    return false;
}

// Determines if the specified property represents a file name.
bool SdfPropertyDictionary::IsPropertyFileName(FdoString* name)
{
	return (wcscmp(name, PROP_NAME_FILE) == 0);
}

// Determines if the specified property represents a path name.
bool SdfPropertyDictionary::IsPropertyFilePath(FdoString* name)
{
	return false;
}

// Determines if the specified property represents a datastore name.
bool SdfPropertyDictionary::IsPropertyDatastoreName(FdoString* name)
{
	return false;
}

// Returns true if the possible values for the specified property can be
// enumerated via the EnumeratePropertyValues method.
bool SdfPropertyDictionary::IsPropertyEnumerable(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_RDONLY) == 0)
        return true;

    return false;
}


// Returns an array of possible values for the specified property.
FdoString** SdfPropertyDictionary::EnumeratePropertyValues(FdoString* name, FdoInt32& count)
{
	static const wchar_t* RDONLY_VALUES[] = {RDONLY_FALSE, RDONLY_TRUE};

    if (wcscmp(name, PROP_NAME_RDONLY) == 0)
    {
        count = 2;
        return (const wchar_t**)RDONLY_VALUES;
    }

    count = 0;
    return NULL;
}


// Returns a localized name for the property (for NLS purposes).
FdoString* SdfPropertyDictionary::GetLocalizedName(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FILE) == 0)
        return NlsMsgGetMain(SDFPROVIDER_48_PROP_NAME_FILE, "File");
    else if (wcscmp(name, PROP_NAME_RDONLY) == 0)
        return NlsMsgGetMain(SDFPROVIDER_49_PROP_NAME_READONLY, "ReadOnly");

    return NULL;
}


//-------------------------------------------------------
// SdfPropertyDictionary implementation
//-------------------------------------------------------


FdoString* SdfPropertyDictionary::GenerateConnectionString()
{
    m_connStr = L"";

    std::map<std::wstring, std::wstring>::iterator iter;
    
    for (iter = m_mProps->begin(); iter != m_mProps->end(); iter++)
    {
        m_connStr += iter->first;
        m_connStr += L"=";
        m_connStr += iter->second;
        m_connStr += L";";
    }

    return m_connStr.c_str();
}


void SdfPropertyDictionary::Clear()
{
    m_mProps->clear();

    //set default value for readonly
    (*m_mProps)[PROP_NAME_RDONLY] = RDONLY_FALSE;

    m_connStr = L"";
}

