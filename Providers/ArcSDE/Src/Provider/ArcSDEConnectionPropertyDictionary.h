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

#ifndef ARCSDECONNECTIONPROPERTYDICTIONARY_H
#define ARCSDECONNECTIONPROPERTYDICTIONARY_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class Property : public FdoIDisposable
{
    friend class ArcSDEConnection;

protected:
    FdoStringP mName;
    FdoStringP mLocalizedName;
    FdoStringP mDefaultValue;
    FdoStringP mValue;
    bool mIsPropertyRequired;
    bool mIsPropertyProtected;
	bool mIsPropertyFileName;
    bool mIsPropertyFilePath;
	bool mIsPropertyDatastoreName;
    bool mIsPropertyEnumerable;
    int mCountEnumerableProperties;
    const wchar_t** mEnumerableProperties;
    bool mIsPropertySet;

public:

    // zero args constructor needed by FdoPtr on linux:
    Property () :
        mName (L""),
        mLocalizedName (L""),
        mDefaultValue (L""),
        mValue (L""),
        mIsPropertyRequired (false),
        mIsPropertyProtected (false),
        mIsPropertyEnumerable (false),
        mCountEnumerableProperties (0),
        mEnumerableProperties (NULL),
        mIsPropertySet (false),
		mIsPropertyFileName(false),
		mIsPropertyFilePath(false),
		mIsPropertyDatastoreName(false)
    {
    }


    Property (const wchar_t* name, const wchar_t* localized_name, const wchar_t* value, bool required, bool protect, bool isEnumerable, int enumCount, const wchar_t** enumerables, bool bIsPropertyFileName, bool bIsPropertyFilePath, bool bIsPropertyDatastoreName) :
        mName (name),
        mLocalizedName (localized_name),
        mDefaultValue (value),
        mValue ((wchar_t*)value),
        mIsPropertyRequired (required),
        mIsPropertyProtected (protect),
        mIsPropertyEnumerable (isEnumerable),
        mCountEnumerableProperties (0),
        mEnumerableProperties (NULL),
        mIsPropertySet (false),
		mIsPropertyFileName(bIsPropertyFileName),
		mIsPropertyFilePath(bIsPropertyFilePath),
		mIsPropertyDatastoreName(bIsPropertyDatastoreName)
    {
        UpdateEnumerableProperties (enumCount, enumerables);
    }

protected:
    ~Property()
    {
        DeleteEnumerableProperties();
    }

    void Dispose()
    {
        delete this;
    };

public:
    const wchar_t* GetName ()
    {
        return (mName);
    }

    const wchar_t* GetLocalizedNamed ()
    {
        return (mLocalizedName);
    }

    const wchar_t* GetDefaultValue ()
    {
        return (mDefaultValue);
    }

    const wchar_t* GetValue ()
    {
        return (mValue);
    }

    void SetValue (const wchar_t* value)
    {
        mValue = value;
        mIsPropertySet = true;
    }

    bool GetIsPropertyRequired ()
    {
        return (mIsPropertyRequired);
    }

    void SetIsPropertyRequired (bool required)
    {
        mIsPropertyRequired = required;
    }

    bool GetIsPropertyProtected ()
    {
        return (mIsPropertyProtected);
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

    bool GetIsPropertyEnumerable ()
    {
        return (mIsPropertyEnumerable);
    }

    int GetCountEnumerableProperties ()
    {
        return (mCountEnumerableProperties);
    }

    const wchar_t** GetEnumerableProperties ()
    {
        return (mEnumerableProperties);
    }

    bool GetIsPropertySet ()
    {
        return (mIsPropertySet);
    }

protected:
    void DeleteEnumerableProperties(void)
    {
        if (mEnumerableProperties != NULL)
        {
            for (int i=0; i<mCountEnumerableProperties; i++)
                delete[] mEnumerableProperties[i];

            delete[] mEnumerableProperties;
        }

        mEnumerableProperties = NULL;
        mCountEnumerableProperties = 0;
    }

    // The caller allocates newValues but this object takes ownership of it and will deallocate it.
    void UpdateEnumerableProperties(int newCount, const wchar_t** newValues)
    {
        // Delete old strings:
        DeleteEnumerableProperties();

        // take ownership of given array of strings:
        mCountEnumerableProperties = newCount;
        mEnumerableProperties = newValues;
    }
};

class ArcSDEPropertyCollection : public FdoCollection<Property, FdoException>
{
    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
        // dispose this object
        virtual void Dispose()
        {
            delete this;
        }
};

class ArcSDEConnectionPropertyDictionary :
    public FdoIConnectionPropertyDictionary
{
    friend class ArcSDEConnection;
    friend class ArcSDEConnectionInfo;

protected:
    ArcSDEConnection* mConnection; // weak reference
    FdoPtr<ArcSDEPropertyCollection> mProperties;
    wchar_t** mNames;

public:
    // 0-argument constructor to please FdoPtr::operator->() :
    ArcSDEConnectionPropertyDictionary () { ArcSDEConnectionPropertyDictionary(NULL); }
    ArcSDEConnectionPropertyDictionary (ArcSDEConnection* connection);

    // Called whenever the user updates the connection string directly:
    void UpdateConnectionString(FdoString *newConnectionString);

protected:
    virtual ~ArcSDEConnectionPropertyDictionary (void);
    virtual void Dispose ();
    virtual void validate ();

public:
    /// <summary> Gets the names of all the properties that can appear in a connection string
    /// for this feature provider as an array of Strings. The order of the property
    /// names in the resulting array dictate the order in which they need to be 
    /// specified. This is especially important for the success of the 
    /// EnumeratePropertyValues method because properties that occur earlier in the array
    /// may be required for successful enumeration of properties that appear later.</summary>
    /// <param name="count">Output the number of parameters</param> 
    /// <returns>Returns the list of parameter names</returns> 
    FdoString** GetPropertyNames (FdoInt32& count);

    /// <summary>Gets the value of the specified property.</summary>
    /// <param name="name">Input the property name.</param> 
    /// <returns>Returns the property value.</returns> 
    FdoString* GetProperty (FdoString* name);

    /// <summary>Sets the value of the specified property. An exception is thrown if the connection is currently open.</summary>
    /// <param name="name">Input the property name</param> 
    /// <param name="value">Input the property value</param> 
    /// <returns>Returns nothing</returns> 
    void SetProperty (FdoString* name, FdoString* value);

    /// <summary>Gets the default value for the specified property.</summary>
    /// <param name="name">Input the property name</param> 
    /// <returns>Returns the property default value</returns> 
    FdoString* GetPropertyDefault (FdoString* name);

    /// <summary>Determines if the specified property is required.</summary>
    /// <param name="name">Input the property name</param> 
    /// <returns>Returns true if the specified property is required</returns> 
    bool IsPropertyRequired (FdoString* name);
 
    /// <summary> Indicates if the property is a password or other protected field
    /// that should be kept secure.</summary>
    /// <param name="name">Input the property name.</param> 
    /// <returns>Returns true if the property is a password or other protected field
    /// that should be kept secure.</returns> 
    bool IsPropertyProtected (FdoString* name);
 
	/// <summary>Determines if the specified property represents a file name.</summary>
    /// <param name="name">Input the property name</param> 
    /// <returns>Returns true if the specified property is a file name</returns> 
	bool IsPropertyFileName(FdoString* name);

	/// <summary>Determines if the specified property represents a path name.</summary>
    /// <param name="name">Input the property name</param> 
    /// <returns>Returns true if the specified property is a path name</returns> 
    bool IsPropertyFilePath(FdoString* name);

	/// <summary>Determines if the specified property represents a datastore name.</summary>
    /// <param name="name">Input the property name</param> 
    /// <returns>Returns true if the specified property is a datastore name</returns> 
	bool IsPropertyDatastoreName(FdoString* name);

    /// <summary>Determines if the possible values for the specified property can be enumerated via the EnumeratePropertyValues method.</summary>
    /// <param name="name">Input the property name</param> 
    /// <returns>Returns true if the possible values for the specified property can be enumerated.</returns> 
    bool IsPropertyEnumerable (FdoString* name);

    /// <summary> Returns an array of possible values for the specified property.</summary>
    /// <param name="name">Input the property name.</param> 
    /// <param name="count">Output the number of values.</param> 
    /// <returns>Returns the list of values for this property.</returns> 
    FdoString** EnumeratePropertyValues (FdoString* name, FdoInt32& count);

    /// <summary> Gets a localized name for the property (for NLS purposes).</summary>
    /// <param name="name">Input the property name.</param> 
    /// <returns>Returns the localized name for the property (for NLS purposes).</returns> 
    FdoString* GetLocalizedName (FdoString* name);

protected:
    Property* FindProperty (const wchar_t* name);
    bool CheckEnumerable (const wchar_t* value, Property* property);

};

#endif // ARCSDECONNECTIONPROPERTYDICTIONARY_H

