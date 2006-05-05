#ifndef _FDORDBMSPROPERTYDICTIONARY_H_
#define _FDORDBMSPROPERTYDICTIONARY_H_
#ifdef _WIN32
#pragma once
#endif
/*
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
 */

#include "Fdo.h"
#include "../Other/Collection.h"
#include "Fdo/Connections/IPropertyDictionary.h"

#include <Common/Ptr.h>
#include "Fdo/Commands/CommandType.h"

typedef  FdoArray<FdoString *> FdoRdbmsDataStoreNames;

#define FDO_RDBMS_CONNECTION_USERNAME    L"Username"
#define FDO_RDBMS_CONNECTION_PASSWORD    L"Password"
#define FDO_RDBMS_CONNECTION_SERVICE     L"Service"
#define FDO_RDBMS_CONNECTION_DATASTORE   L"DataStore"

#define FDO_RDBMS_DATASTORE_DESCRIPTION	 L"Description"
#define FDO_RDBMS_DATASTORE_LTMODE		 L"LtMode"
#define FDO_RDBMS_DATASTORE_LOCKMODE	 L"LockMode"
#define FDO_RDBMS_DATASTORE_TABLESPACE	 L"TableSpace"

#define FDO_RDBMS_DATASTORE_FOR_READ	0
#define FDO_RDBMS_DATASTORE_FOR_CREATE	1
#define FDO_RDBMS_DATASTORE_FOR_DELETE	2

class FdoRdbmsConnection;

class FdoRdbmsProperty : public FdoIDisposable
{
public:
    FdoRdbmsProperty() :
        mName(NULL),
        mValue(NULL),
        mDefaultValue(NULL),
        mLocalizedName(NULL),
        mIsPropertyRequired(false),
        mIsPropertySet(false),
        mIsPropertyProtected(false),
        mIsPropertyEnumerable(false),
		mIsPropertyFileName(false),
		mIsPropertyFilePath(false),
		mIsPropertyDatastoreName(false),
        mIsPropertyQuoted(false),
        mEnumerableProperties(NULL),
        mCountEnumerableProperties(0)
    {
    }
    ~FdoRdbmsProperty()
    {
        if (mName)
            delete [] mName;
        if (mValue)
            delete [] mValue;
        if (mDefaultValue)
            delete [] mDefaultValue;
        if (mLocalizedName)
            delete [] mLocalizedName;
    }
    void Dispose()
    {
        delete this;
    };

    wchar_t *GetName()
    {
        return mName;
    }

    wchar_t *GetDefaultValue()
    {
        return mDefaultValue;
    }
    wchar_t*  GetValue()
    {
        return mValue;
    }

    void SetValue(const wchar_t *value)
    {
        if (mValue)
            delete [] mValue;
        mValue = new wchar_t[wcslen(value)+1];
        wcscpy(mValue, value);
    }

    bool GetIsPropertyRequired()
    {
        return mIsPropertyRequired;
    }

    bool GetIsPropertySet()
    {
        return mIsPropertySet;
    }

    bool GetIsPropertyProtected()
    {
        return mIsPropertyProtected;
    }
    bool GetIsPropertyEnumerable()
    {
        return mIsPropertyEnumerable;
    }

	bool GetIsPropertyFileName()
	{
		return mIsPropertyFileName;
	}

    bool GetIsPropertyFilePath()
	{
		return mIsPropertyFilePath;
	}

	bool GetIsPropertyDatastoreName()
	{
		return mIsPropertyDatastoreName;
	}

	bool GetIsPropertyQuoted()
	{
		return mIsPropertyQuoted;
	}

    const wchar_t *GetLocalizedNamed()
    {
        return mLocalizedName;
    }

    const wchar_t **GetEnumerableProperties()
    {
        return mEnumerableProperties;
    }

    int  GetCountEnumerableProperties()
    {
        return mCountEnumerableProperties;
    }

    void SetName(const wchar_t *name)
    {
        if (mName)
            delete [] mName;
        mName = new wchar_t[wcslen(name)+1];
        wcscpy(mName, name);
    }

    void SetLocalizedName(const wchar_t *name)
    {
        if (mLocalizedName)
            delete [] mLocalizedName;
        mLocalizedName = new wchar_t[wcslen(name)+1];
        wcscpy(mLocalizedName, name);
    }

    void SetDefaultValue(wchar_t *defaultValue)
    {
        if (mDefaultValue)
            delete [] mDefaultValue;
        mDefaultValue = new wchar_t[wcslen(defaultValue)+1];
        wcscpy(mDefaultValue, defaultValue);
    }

    void SetIsPropertyRequired(bool IsPropertyRequired)
    {
        mIsPropertyRequired = IsPropertyRequired;
    }

    void SetIsPropertySet(bool IsPropertySet)
    {
        mIsPropertySet = IsPropertySet;
    }

    void SetIsPropertyProtected(bool IsPropertyProtected)
    {
        mIsPropertyProtected = IsPropertyProtected;
    }

	void SetIsPropertyFileName( bool isPropertyFileName)
	{
		mIsPropertyFileName = isPropertyFileName;
	}

    void SetIsPropertyFilePath( bool isPropertyFilePath)
	{
		mIsPropertyFilePath = isPropertyFilePath;
	}

	void SetIsPropertyDatastoreName( bool isPropertyDatastoreName)
	{
		mIsPropertyDatastoreName = isPropertyDatastoreName;
	}

	void SetIsPropertyQuoted( bool isPropertyQuoted)
	{
		mIsPropertyQuoted = isPropertyQuoted;
	}

    void SetIsPropertyEnumerable(bool IsPropertyEnumerable)
    {
        mIsPropertyEnumerable = IsPropertyEnumerable;
    }
    void SetEnumerableProperties(const wchar_t **enumerableProperties)
    {
        mEnumerableProperties = enumerableProperties;
    }

    void SetCountEnumerableProperties(int countEnumerableProperties)
    {
        mCountEnumerableProperties = countEnumerableProperties;
    }

private:
    wchar_t         *mName;
    wchar_t         *mLocalizedName;
    wchar_t         *mDefaultValue;
    wchar_t         *mValue;
    bool            mIsPropertyRequired;
    bool            mIsPropertySet;
    bool            mIsPropertyProtected;
    bool            mIsPropertyEnumerable;
	bool			mIsPropertyFileName;
    bool			mIsPropertyFilePath;
	bool			mIsPropertyDatastoreName;
    bool            mIsPropertyQuoted;
    const wchar_t   **mEnumerableProperties;
    int             mCountEnumerableProperties;
};

class FdoRdbmsPropertyDictionary : public FdoIPropertyDictionary
{
    friend class FdoRdbmsConnection;

protected:
    virtual ~FdoRdbmsPropertyDictionary();
    virtual void Dispose();

    virtual FdoRdbmsProperty* GetPropertyObject(wchar_t *name);
    virtual FdoRdbmsProperty* FindPropertyObject(wchar_t *name);
    void AddItem(const wchar_t *name, const wchar_t *localizedName, bool bPropertySet, wchar_t *defaultValue, bool bPropertyRequired, bool bPropertyProtected, 
				 bool bPropertyEnumerable, int countEnumerableProperties, const wchar_t **enumerableProperties,
				 bool bIsPropertyFileName, bool bIsPropertyFilePath, bool bIsPropertyDatastoreName, bool bIsPropertyQuoted=false);

public:
    FdoRdbmsPropertyDictionary();
    FdoRdbmsPropertyDictionary(FdoRdbmsConnection *connection);

    // Gets the names of all the properties that can appear in a connection string
    // for this feature provider as an array of Strings. The order of the property
    // names in the resulting array dictate the order in which they need to be
    // specified. This is especially important for the success of the
    // EnumeratePropertyValues method as properties that occur earlier in the array
    // may be required for successful enumeration of properties that appear later
    // in the array.
    virtual const wchar_t **GetPropertyNames(int& length);

    // Gets the value of the specified property.
    virtual const wchar_t* GetProperty(const wchar_t* name);

    // Sets the value of the specified property. An exception will be thrown if the connection is currently open.
    virtual void SetProperty(const wchar_t* name, const wchar_t* value);

    // Gets the default value for the specified property.
    virtual const wchar_t* GetPropertyDefault(const wchar_t* name);

    // Returns true if the specified property is required, false if it is optional.
    virtual bool IsPropertyRequired(const wchar_t* name);

    // Indicates if the property is a password or other protected field
    // that should be kept secure.
    virtual bool IsPropertyProtected(FdoString* name);

    // Returns true if the possible values for the specified property can be enumerated via the EnumeratePropertyValues method.
    virtual bool IsPropertyEnumerable(FdoString* name);

    // Returns an array of possible values for the specified property.
    virtual const wchar_t** EnumeratePropertyValues(FdoString* name, int& length);

    // Returns a localized name for the property (for NLS purposes).
    virtual FdoString* GetLocalizedName(FdoString* name);

	/// Determines if the specified property represents a file name.
	virtual bool IsPropertyFileName(FdoString* name);

	/// Determines if the specified property represents a path name.
    virtual bool IsPropertyFilePath(FdoString* name);

	/// Determines if the specified property represents a datastore name.
	virtual bool IsPropertyDatastoreName(FdoString* name);

protected:
    FdoRdbmsConnection *mConnection;
    const wchar_t **mPropertyNames;
    bool mPropertyNamesAllocated;
    FdoRdbmsCollection<FdoRdbmsProperty> *mProperties;
    FdoPtr<FdoRdbmsDataStoreNames> mNames;
};

#endif

