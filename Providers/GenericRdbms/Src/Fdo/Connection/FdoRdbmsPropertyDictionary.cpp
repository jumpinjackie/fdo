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
#include <Common/Ptr.h>
#include "FdoRdbmsPropertyDictionary.h"
#include "FdoRdbmsConnectionInfo.h"
#include "../DataStore/FdoRdbmsGetDataStores.h"
#include "Fdo/Commands/CommandType.h"
#include <Inc/Nls/fdordbms_msg.h>
#include "GdbiException.h"
#include "FdoRdbmsConnection.h"


FdoRdbmsPropertyDictionary::FdoRdbmsPropertyDictionary():
mConnection(NULL),
mPropertyNames(NULL),
mPropertyNamesAllocated(false)
{
}

FdoRdbmsPropertyDictionary::FdoRdbmsPropertyDictionary(FdoRdbmsConnection *connection):
mConnection(connection),
mPropertyNames(NULL),
mPropertyNamesAllocated(false)

{
    mProperties = new FdoRdbmsCollection<FdoRdbmsProperty>;
    if (mProperties == NULL)
        throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_11, "Memory error"));

	// Properties names are set by subclasses
}

void FdoRdbmsPropertyDictionary::AddItem(const wchar_t *name, const wchar_t *localizedName, bool bPropertySet, wchar_t *defaultValue, bool bPropertyRequired, bool bPropertyProtected, 
										 bool bPropertyEnumerable, int countEnumerableProperties, const wchar_t **enumerableProperties,
						 				 bool bIsPropertyFileName, bool bIsPropertyFilePath, bool bIsPropertyDatastoreName, bool bIsPropertyQuoted )
{
    FdoRdbmsProperty *property = new FdoRdbmsProperty();
    if (property == NULL)
        throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_11, "Memory error"));

    property->SetName(name);
    property->SetLocalizedName(localizedName);
    property->SetIsPropertySet(bPropertySet);
    property->SetDefaultValue(defaultValue);
    property->SetValue(defaultValue);
    property->SetIsPropertyRequired(bPropertyRequired);
    property->SetIsPropertyProtected(bPropertyProtected);
    property->SetIsPropertyEnumerable(bPropertyEnumerable);
	property->SetIsPropertyFileName(bIsPropertyFileName);
	property->SetIsPropertyFilePath(bIsPropertyFilePath);
	property->SetIsPropertyDatastoreName(bIsPropertyDatastoreName);
    property->SetIsPropertyQuoted(bIsPropertyQuoted);
    property->SetCountEnumerableProperties(countEnumerableProperties);
    property->SetEnumerableProperties(enumerableProperties);
    mProperties->Add(property);
    property->Release();
}

FdoRdbmsPropertyDictionary::~FdoRdbmsPropertyDictionary()
{
    if (mPropertyNamesAllocated)
    {
        for (int i=0; i<mProperties->GetCount(); i++)
            delete [] mPropertyNames[i];
        delete [] mPropertyNames;
    }

    mProperties->Release();
    mProperties = NULL;
    if (mNames)
    {
        FdoString **dbnames = mNames->GetData();
        for (int i=0; i<mNames->GetCount(); i++)
        {
            FdoString *pdbname = (dbnames)[i];
            delete[] pdbname;
        }
    }
    return;
}

const wchar_t** FdoRdbmsPropertyDictionary::GetPropertyNames(int& count)
{
    count = mProperties->GetCount();
    if (mPropertyNamesAllocated == false)
    {
        mPropertyNames = (const wchar_t **) new wchar_t[sizeof(wchar_t *) * count];
        if (mPropertyNames == NULL)
            throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_11, "Memory error"));

        for (int i=0; i<count; i++)
        {
            FdoRdbmsProperty *prop = mProperties->GetItem(i);
            mPropertyNames[i] = new wchar_t[wcslen(prop->GetName())+1];
            if (mPropertyNames[i] == NULL)
                throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_11, "Memory error"));
            wcscpy((wchar_t*)(mPropertyNames[i]), prop->GetName());
            prop->Release();
        }
        mPropertyNamesAllocated = true;
    }

    return mPropertyNames;

}

const wchar_t* FdoRdbmsPropertyDictionary::GetProperty(const wchar_t* name)
{
    FdoPtr<FdoRdbmsProperty> property = GetPropertyObject((wchar_t*)name);
    return property->GetValue();
}

void FdoRdbmsPropertyDictionary::SetProperty(const wchar_t* name, const wchar_t* value)
{
    bool bIsValid = true;
    int i;

    // Ignore null values;
    if( value == NULL )
        return;

    if( name == NULL || wcslen( name ) == 0 )
        return;

    if ( mConnection && mConnection->GetConnectionState() == FdoConnectionState_Open)
        throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_12, "Connection already opened"));
    FdoPtr<FdoRdbmsProperty> property = GetPropertyObject((wchar_t*)name);
    property->SetValue(value);
    property->SetIsPropertySet(true);

    FdoStringP str("");
    bool bPropertiesSet = true;
    for (i=0;i<mProperties->GetCount(); i++)
    {
        FdoRdbmsProperty *prop = mProperties->GetItem(i);

        if (prop->GetIsPropertySet() == false && prop->GetIsPropertyRequired())
            bIsValid = false;
        FdoStringP tmp;
        if (prop->GetIsPropertyQuoted())
            tmp = FdoStringP::Format(L"%ls=\"%ls\";", prop->GetName(), prop->GetValue());
        else
            tmp = FdoStringP::Format(L"%ls=%ls;", prop->GetName(), prop->GetValue());
        str = str + (const wchar_t * ) tmp;
        prop->Release();
    }

	// FdoRdbmsdataStorePropertyDictionary is connectionless
	if ( mConnection )
	{
		if (i == mProperties->GetCount()) {
			mConnection->SetConnectionString(str);
		}
		mConnection->SetConnectionStringValid(bIsValid);
	}

}

const wchar_t* FdoRdbmsPropertyDictionary::GetPropertyDefault(const wchar_t* name)
{
    FdoPtr<FdoRdbmsProperty> property = GetPropertyObject((wchar_t*)name);
    return property->GetDefaultValue();
}

bool FdoRdbmsPropertyDictionary::IsPropertyRequired(const wchar_t* name)
{
    FdoPtr<FdoRdbmsProperty>property = GetPropertyObject((wchar_t*)name);
    return property->GetIsPropertyRequired();
}

bool FdoRdbmsPropertyDictionary::IsPropertyEnumerable(const wchar_t* name)
{
    FdoPtr<FdoRdbmsProperty>property = GetPropertyObject((wchar_t*)name);
    return (property->GetIsPropertyEnumerable());
}

bool FdoRdbmsPropertyDictionary::IsPropertyProtected(const wchar_t* name)
{
    FdoPtr<FdoRdbmsProperty>property = GetPropertyObject((wchar_t*)name);
    return (property->GetIsPropertyProtected() != 0);
}

bool FdoRdbmsPropertyDictionary::IsPropertyFileName(const wchar_t* name)
{
    FdoPtr<FdoRdbmsProperty>property = GetPropertyObject((wchar_t*)name);
    return (property->GetIsPropertyFileName() != 0);
}

bool FdoRdbmsPropertyDictionary::IsPropertyFilePath(const wchar_t* name)
{
    FdoPtr<FdoRdbmsProperty>property = GetPropertyObject((wchar_t*)name);
    return (property->GetIsPropertyFilePath() != 0);
}

bool FdoRdbmsPropertyDictionary::IsPropertyDatastoreName(const wchar_t* name)
{
    FdoPtr<FdoRdbmsProperty>property = GetPropertyObject((wchar_t*)name);
    return (property->GetIsPropertyDatastoreName() != 0);
}

const FdoString *FdoRdbmsPropertyDictionary::GetLocalizedName(const wchar_t* name)
{
    FdoPtr<FdoRdbmsProperty>property = GetPropertyObject((wchar_t*)name);
    return property->GetLocalizedNamed();
}

const wchar_t **FdoRdbmsPropertyDictionary::EnumeratePropertyValues(const wchar_t* name, int& count)
{
    FdoPtr<FdoRdbmsProperty>property = GetPropertyObject((wchar_t*)name);

    if (wcscmp(name,  FDO_RDBMS_CONNECTION_DATASTORE) == 0)
    {
		if ( (mConnection == NULL) || (mConnection->GetConnectionState() == FdoConnectionState_Closed) )
			throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

        if (mConnection->GetDbiConnection() != NULL)
        {
            FdoRdbmsGetDataStores* dataStores = (FdoRdbmsGetDataStores *)mConnection->CreateCommand(FdoCommandType_ListDataStores);
            mNames = dataStores->GetDataStoresNames();
            FdoString **dbnames = mNames->GetData();
            property->SetEnumerableProperties(dbnames);
            property->SetCountEnumerableProperties(mNames->GetCount());
            dataStores->Release();
        }
        else
        {
            property->SetEnumerableProperties(NULL);
            property->SetCountEnumerableProperties(0);
        }
    }
    count = property->GetCountEnumerableProperties();
    return property->GetEnumerableProperties();
}

FdoRdbmsProperty* FdoRdbmsPropertyDictionary::GetPropertyObject(wchar_t *name)
{
    for (int i=0; i<mProperties->GetCount(); i++)
    {
        FdoRdbmsProperty *prop = mProperties->GetItem(i);
        if (wcscmp(prop->GetName(), name) == 0)
        {
            return prop;
        }
        prop->Release();
    }

    throw FdoConnectionException::Create(NlsMsgGet1(FDORDBMS_56, "Property '%1$ls' not found", name));
}

FdoRdbmsProperty* FdoRdbmsPropertyDictionary::FindPropertyObject(wchar_t *name)
{
    for (int i=0; i<mProperties->GetCount(); i++)
    {
        FdoRdbmsProperty *prop = mProperties->GetItem(i);
        if (wcscmp(prop->GetName(), name) == 0)
        {
            return prop;
        }
        prop->Release();
    }

    return NULL;
}

void FdoRdbmsPropertyDictionary::Dispose()
{
    delete this;
}


