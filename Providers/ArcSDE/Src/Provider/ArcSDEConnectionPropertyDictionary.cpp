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

#include "ConnectionStringParser.h"

ArcSDEConnectionPropertyDictionary::ArcSDEConnectionPropertyDictionary (ArcSDEConnection* connection) :
    mConnection (connection),
    mNames (NULL)
{
    FdoPtr<Property> property;
    CHAR *mbPropName = NULL;

    // NOTE: we use a weak reference here to avoid a circular dependancy:
    // FDO_SAFE_ADDREF(connection);

    mProperties = CheckMemory(new ArcSDEPropertyCollection ());

    wide_to_multibyte(mbPropName, CONNECTIONPROPERTY_SERVER);
    property = CheckMemory(new Property (CONNECTIONPROPERTY_SERVER,
                           NlsMsgGet(ARCSDE_CONNECTION_PROPERTY_SERVER, mbPropName),
                           L"", true, false, false, 0, NULL, false, false, false));
    mProperties->Add (property);

    wide_to_multibyte(mbPropName, CONNECTIONPROPERTY_INSTANCE);
    property = CheckMemory(new Property (CONNECTIONPROPERTY_INSTANCE,
                             NlsMsgGet(ARCSDE_CONNECTION_PROPERTY_INSTANCE, mbPropName),
                             L"", true, false, false, 0, NULL, false, false, false));
    mProperties->Add (property);

    wide_to_multibyte(mbPropName, CONNECTIONPROPERTY_USERNAME);
    property = CheckMemory(new Property (CONNECTIONPROPERTY_USERNAME,
                             NlsMsgGet(ARCSDE_CONNECTION_PROPERTY_USERNAME, mbPropName),
                             L"", true, false, false, 0, NULL, false, false, false));
    mProperties->Add (property);

    wide_to_multibyte(mbPropName, CONNECTIONPROPERTY_PASSWORD);
    property = CheckMemory(new Property (CONNECTIONPROPERTY_PASSWORD,
                             NlsMsgGet(ARCSDE_CONNECTION_PROPERTY_PASSWORD, mbPropName),
                             L"", true, true, false, 0, NULL, false, false, false));
    mProperties->Add (property);

    wide_to_multibyte(mbPropName, CONNECTIONPROPERTY_DATASTORE);
    property = CheckMemory(new Property (CONNECTIONPROPERTY_DATASTORE,
                              NlsMsgGet(ARCSDE_CONNECTION_PROPERTY_DATASTORE, mbPropName),
                              L"", false, false, true, 0, NULL, false, false, true));
    mProperties->Add (property);

    const wchar_t* connection_string = mConnection->GetConnectionString ();
    UpdateConnectionString(connection_string);
}

ArcSDEConnectionPropertyDictionary::~ArcSDEConnectionPropertyDictionary (void)
{
    // clean up:
    if (NULL != mNames)
    {
        int count = mProperties->GetCount ();
        for (int i = 0; i < count; i++)
            delete [] mNames[i];
    }
    delete [] mNames;
    mNames = NULL;
}

void ArcSDEConnectionPropertyDictionary::Dispose()
{
    delete this;
}

void ArcSDEConnectionPropertyDictionary::validate ()
{
    if (mConnection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_INVALID, "Connection is invalid."));
}

/// <summary> Gets the names of all the properties that can appear in a connection string
/// for this feature provider as an array of Strings. The order of the property
/// names in the resulting array dictate the order in which they need to be 
/// specified. This is especially important for the success of the 
/// EnumeratePropertyValues method because properties that occur earlier in the array
/// may be required for successful enumeration of properties that appear later.</summary>
/// <param name="count">Output the number of parameters</param> 
/// <returns>Returns the list of parameter names</returns> 
FdoString** ArcSDEConnectionPropertyDictionary::GetPropertyNames (FdoInt32& count)
{
    validate ();
    count = mProperties->GetCount ();
    if (NULL == mNames)
    {
        mNames = CheckMemory(new wchar_t*[count]);

        for (int i = 0; i < count; i++)
        {
            FdoPtr<Property> property = mProperties->GetItem (i);
            const wchar_t* name = property->GetName ();
            if (NULL != name)
            {
                mNames[i] = new wchar_t[wcslen (name) + 1];
                wcscpy (mNames[i], name);
            }
            else
                mNames[i] = NULL;
        }
    }

    return ((const wchar_t**)mNames);
}

/// <summary>Gets the value of the specified property.</summary>
/// <param name="name">Input the property name.</param> 
/// <returns>Returns the property value.</returns> 
FdoString* ArcSDEConnectionPropertyDictionary::GetProperty (FdoString* name)
{
    validate ();
    FdoPtr<Property> property = FindProperty (name);
    return (property->GetValue ());
}

/// <summary>Sets the value of the specified property. An exception is thrown if the connection is currently open.</summary>
/// <param name="name">Input the property name</param> 
/// <param name="value">Input the property value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEConnectionPropertyDictionary::SetProperty (FdoString* name, FdoString* value)
{
    size_t length;
    int count;
    wchar_t* connection_string;

    validate ();

    // validate input
    FdoPtr<Property> property = FindProperty (name);
    if (property->GetIsPropertyRequired () && (NULL == value))
        throw FdoException::Create (NlsMsgGet1(ARCSDE_CONNECTION_REQUIRED_PROPERTY_NULL, "The required property '%1$ls' cannot be set to NULL.", name));

    if (!CheckEnumerable (value, property))
        throw FdoException::Create (NlsMsgGet2(ARCSDE_CONNECTION_ENUM_PROPERTY_WRONG_VALUE, "The given value '%1$ls' for enumerable property '%2$ls' is not in the enumeration value collection.", value, name));


    // update the property
    property->SetValue (value);

    // compute the new connection string length
    length = 0;
    count = mProperties->GetCount ();
    for (int i = 0; i < count; i++)
    {
        property = mProperties->GetItem(i);

        // skip over properties that aren't set:
        // NOTE: if a required property isn't set, ArcSDEConnection::Open() will throw appropriate exception
        if (!property->GetIsPropertySet ())
            continue;

        // verify we have a valid token
        const wchar_t* pname  = property->GetName ();
        const wchar_t* pvalue = property->GetValue ();
        if (pname == NULL || pvalue == NULL)
            continue;
        size_t nameLen  = wcslen (pname);
        size_t valueLen = wcslen (pvalue);
        if (nameLen == 0)   // but zero-length values are ok
            continue;

        // account for the token length
        if (length > 0)
            length += 1;        // ;
        length += nameLen;      // name
        length += 1;            // =
        length += valueLen;     // value
    }

    // generate the new connection string
    connection_string = new wchar_t[length + 1];
    connection_string[0] = L'\0';
    length = 0;
    for (int i = 0; i < count; i++)
    {
        property = mProperties->GetItem(i);

        // skip over properties that aren't set:
        // NOTE: if a required property isn't set, ArcSDEConnection::Open() will throw appropriate exception
        if (!property->GetIsPropertySet ())
            continue;

        // verify we have a valid token
        const wchar_t* tname  = property->GetName ();
        const wchar_t* tvalue = property->GetValue ();
        if (tname == NULL || tvalue == NULL)
            continue;
        size_t nameLen  = wcslen (tname);
        if (nameLen == 0)   // but zero-length values are ok
            continue;

        // add the token
        if (0 != length)
            wcscat (connection_string, CONNECTIONPROPERTY_DELIMITER);  // ;
        wcscat (connection_string, tname);                              // name
        wcscat (connection_string, CONNECTIONPROPERTY_SEPARATOR);      // =
        wcscat (connection_string, tvalue);                             // value
        length++;
    }

    // update it on the connection
    mConnection->SetConnectionString (connection_string);
    delete [] connection_string;

}

/// <summary>Gets the default value for the specified property.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns the property default value</returns> 
FdoString* ArcSDEConnectionPropertyDictionary::GetPropertyDefault (FdoString* name)
{
    validate ();
    FdoPtr<Property> property = FindProperty (name);
    return (property->GetDefaultValue ());
}

/// <summary>Determines if the specified property is required.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns true if the specified property is required</returns> 
bool ArcSDEConnectionPropertyDictionary::IsPropertyRequired (FdoString* name)
{
    validate ();
    FdoPtr<Property> property = FindProperty (name);
    return (property->GetIsPropertyRequired ());
}

/// <summary> Indicates if the property is a password or other protected field
/// that should be kept secure.</summary>
/// <param name="name">Input the property name.</param> 
/// <returns>Returns true if the property is a password or other protected field
/// that should be kept secure.</returns> 
bool ArcSDEConnectionPropertyDictionary::IsPropertyProtected (FdoString* name)
{
    validate ();
    FdoPtr<Property> property = FindProperty (name);
    return (property->GetIsPropertyProtected ());
}

/// <summary>Determines if the specified property represents a file name.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns true if the specified property is a file name</returns> 
bool ArcSDEConnectionPropertyDictionary::IsPropertyFileName(FdoString* name)
{
    validate ();
    FdoPtr<Property> property = FindProperty (name);
    return (property->GetIsPropertyFileName());
}

/// <summary>Determines if the specified property represents a path name.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns true if the specified property is a path name</returns> 
bool ArcSDEConnectionPropertyDictionary::IsPropertyFilePath(FdoString* name)
{
    validate ();
    FdoPtr<Property> property = FindProperty (name);
    return (property->GetIsPropertyFilePath());
}

/// <summary>Determines if the specified property represents a datastore name.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns true if the specified property is a datastore name</returns> 
bool ArcSDEConnectionPropertyDictionary::IsPropertyDatastoreName(FdoString* name)
{
    validate ();
    FdoPtr<Property> property = FindProperty (name);
    return (property->GetIsPropertyDatastoreName());
}

/// <summary>Determines if the possible values for the specified property can be enumerated via the EnumeratePropertyValues method.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns true if the possible values for the specified property can be enumerated.</returns> 
bool ArcSDEConnectionPropertyDictionary::IsPropertyEnumerable (FdoString* name)
{
    validate ();
    FdoPtr<Property> property = FindProperty (name);
    return (property->GetIsPropertyEnumerable ());
}

/// <summary> Returns an array of possible values for the specified property.</summary>
/// <param name="name">Input the property name.</param> 
/// <param name="count">Output the number of values.</param> 
/// <returns>Returns the list of values for this property.</returns> 
FdoString** ArcSDEConnectionPropertyDictionary::EnumeratePropertyValues (FdoString* name, FdoInt32& count)
{
    validate ();
    FdoPtr<Property> property = FindProperty (name);
    count = property->GetCountEnumerableProperties ();
    return (property->GetEnumerableProperties ());
}

/// <summary> Gets a localized name for the property (for NLS purposes).</summary>
/// <param name="name">Input the property name.</param> 
/// <returns>Returns the localized name for the property (for NLS purposes).</returns> 
FdoString* ArcSDEConnectionPropertyDictionary::GetLocalizedName (FdoString* name)
{
    validate ();
    FdoPtr<Property> property = FindProperty (name);
    return (property->GetLocalizedNamed ());
}

// Returns the property with the specified name.
Property* ArcSDEConnectionPropertyDictionary::FindProperty (const wchar_t* name)
{
    validate ();
    int count = mProperties->GetCount ();
    for (int i = 0; i < count; i++)
    {
        FdoPtr<Property> property = mProperties->GetItem (i);
        if (0 == wcscmp (property->GetName (), name))
            return (property.Detach ());
    }

    throw FdoException::Create (NlsMsgGet1(ARCSDE_CONNECTION_PROPERTY_NOT_FOUND, "The connection property '%1$ls' was not found.", name));
}

// Returns whether the specified value is in the set of enumerable values.
bool ArcSDEConnectionPropertyDictionary::CheckEnumerable (const wchar_t* value, Property* property)
{
    const wchar_t** choices;
    int count;
    bool ret;

    ret = false;

    count = property->GetCountEnumerableProperties ();
    choices = property->GetEnumerableProperties ();
    if (NULL == choices)
        ret = true;
    else
        for (int i = 0; i < count && !ret; i++)
            if (0 == wcscmp (value, choices[i]))
                ret = true;

    return (ret);
}


// Called whenever the user updates the connection string directly:
void ArcSDEConnectionPropertyDictionary::UpdateConnectionString(FdoString *newConnectionString)
{
    FdoPtr<Property> property;

    // Initialize all properties to empty string:
    for (FdoInt32 i=0; i<mProperties->GetCount(); i++)
    {
        property = mProperties->GetItem(i);
        property->SetValue(L"");
    }

    // Populate the properties that are set in newConnectionString:
    if (NULL != newConnectionString)
    {
        ConnectionStringParser parser (this, newConnectionString);
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

