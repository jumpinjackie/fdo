/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
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
* 
 */

#include "stdafx.h"

#include <malloc.h>

#include "FdoCommonConnPropDictionary.h"
#include "FdoCommonConnStringParser.h"

//TODO: rename this cpp file to not contain the keyword "Connection"
// since it is now usable for several kinds of FdoIPropertyDictionary
// subclasses.


// Syncronizes this dictionary to the given connection string;
// Call this method whenever the user updates the connection string directly
// (e.g. in FdoIConnection::SetConnectionString()):
void FdoCommonConnPropDictionary::UpdateFromConnectionString(FdoString *newConnectionString)
{
    FdoPtr<ConnectionProperty> property;

    // Initialize all properties to empty string:
    for (FdoInt32 i=0; i<mProperties->GetCount(); i++)
    {
        property = mProperties->GetItem(i);
        property->SetValue(L"");
    }

    // Populate the properties that are set in newConnectionString:
    if (NULL != newConnectionString)
    {
        FdoCommonConnStringParser parser (this, newConnectionString);
        for (FdoInt32 i=0; i<mProperties->GetCount(); i++)
        {
            property = mProperties->GetItem(i);
            if (parser.IsPropertyValueSet(property->GetName()))
            {
                FdoStringP val = parser.GetPropertyValueW(property->GetName());
                property->SetValue(val);
            }
        }
    }
}

// Define a new connection property;
// Dall this method once per connection property
// immediately after creating a new instance of FdoCommonPropDictionary:
void FdoCommonConnPropDictionary::AddProperty(ConnectionProperty *newConnProp)
{
    // let base class do its work:
    FdoCommonPropDictionary<FdoIConnectionPropertyDictionary>::AddProperty(newConnProp);

    // Populate the dictionary from what's currently in the connection string:
    const wchar_t* connection_string = mConnection->GetConnectionString ();
    UpdateFromConnectionString(connection_string);
}



void FdoCommonConnPropDictionary::SetProperty (FdoString* name, FdoString* value)
{
    // let base class do its work:
    FdoCommonPropDictionary<FdoIConnectionPropertyDictionary>::SetProperty(name, value);

    // compute the new connection string length
    FdoPtr<ConnectionProperty> pProperty;
    size_t length = 0;
    int count = mProperties->GetCount ();
    for (int i = 0; i < count; i++)
    {
        pProperty = mProperties->GetItem(i);

        // skip over properties that aren't set:
        // NOTE: if a required property isn't set, FdoIConnection::Open() should throw appropriate exception
        if (!pProperty->GetIsPropertySet ())
            continue;

        // verify we have a valid token
        const wchar_t* pname  = pProperty->GetName ();
        const wchar_t* pvalue = pProperty->GetValue ();
        if (pname == NULL || pvalue == NULL)
            continue;
        size_t nameLen  = wcslen (pname);
        size_t valueLen = wcslen (pvalue);
        if (nameLen == 0)   // but zero-length values are ok
            continue;

        // account for the token length
        length += nameLen;      // name
        length += 1;            // =
        length += valueLen;     // value
        length += 1;        // ;
    }

    // generate the new connection string
    wchar_t* connection_string;
    connection_string = new wchar_t[length + 2 + 1];
    connection_string[0] = L'\0';
    length = 0;
    for (int i = 0; i < count; i++)
    {
        pProperty = mProperties->GetItem(i);

        // skip over properties that aren't set:
        // NOTE: if a required property isn't set, FdoIConnection::Open() should throw appropriate exception
        if (!pProperty->GetIsPropertySet ())
            continue;

        // verify we have a valid token
        const wchar_t* tname  = pProperty->GetName ();
        const wchar_t* tvalue = pProperty->GetValue ();
        if (tname == NULL || tvalue == NULL)
            continue;
        size_t nameLen  = wcslen (tname);
        if (nameLen == 0)   // but zero-length values are ok
            continue;

        wcscat (connection_string, tname);                              // name
        wcscat (connection_string, CONNECTIONPROPERTY_SEPARATOR);       // =
		if (tvalue != NULL && (pProperty->GetIsPropertyQuoted() || wcsrchr(tvalue, L';') != NULL))
		{
			wcscat (connection_string, L"\"");                              // "
			wcscat (connection_string, tvalue);                             // value
			wcscat (connection_string, L"\"");                              // "
		}
		else
			wcscat (connection_string, tvalue);                             // value
        wcscat (connection_string, CONNECTIONPROPERTY_DELIMITER);   // ;
        length++;
    }

    // update it on the connection
    mConnection->SetConnectionString (connection_string);
    delete [] connection_string;
}
