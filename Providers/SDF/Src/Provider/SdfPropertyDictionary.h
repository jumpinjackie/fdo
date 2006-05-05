#ifndef _SDFPROPERTYDICTIONARY_H
#define _SDFPROPERTYDICTIONARY_H
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


#define	PROP_NAME_FILE		L"File"	  
#define	PROP_NAME_RDONLY	L"ReadOnly"  
#define	EMPTY_VALUE			L""
#define	RDONLY_FALSE		L"FALSE"
#define	RDONLY_TRUE			L"TRUE"

class SdfConnection;

class SdfPropertyDictionary : public FdoIPropertyDictionary
{
	friend class SdfConnection;

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

public:
    // constructs a FmeConnectionPropertyDictionary using the specified connection
    SdfPropertyDictionary(SdfConnection* connection);

protected:
    // default destructor
    virtual ~SdfPropertyDictionary();

    //-------------------------------------------------------
    // GisIDisposable implementation
    //-------------------------------------------------------

protected:
    // dispose this object
    SDF_API virtual void Dispose();

    //-------------------------------------------------------
    // FdoIConnectionPropertyDictionary implementation
    //-------------------------------------------------------

public:
    // Gets the names of all the properties that can appear in a connection string
    // for this feature provider as an array of Strings.  The order of the property
    // names in the resulting array dictate the order in which they need to be
    // specified.  This is especially important for the success of the
    // EnumeratePropertyValues method as properties that occur earlier in the array
    // may be required for successful enumeration of properties that appear later
    // in the array.  The count parameter returns the size of the returned array.
    SDF_API virtual FdoString** GetPropertyNames(FdoInt32& count) = 0;

    // Gets the value of the specified property.
    SDF_API virtual FdoString* GetProperty(FdoString* name);

    // Sets the value of the specified property.  An exception will be thrown if
    // the connection is currently open.
    SDF_API virtual void SetProperty(FdoString* name, FdoString* value);

    // Gets the default value for the specified property.
    SDF_API virtual FdoString* GetPropertyDefault(FdoString* name);

    // Returns true if the specified property is required, false if it is optional.
    SDF_API virtual bool IsPropertyRequired(FdoString* name);

    // Indicates if the property is a password or other protected field
    // that should be kept secure.
    SDF_API virtual bool IsPropertyProtected(FdoString* name);

    // Returns true if the possible values for the specified property can be
    // enumerated via the EnumeratePropertyValues method.
    SDF_API virtual bool IsPropertyEnumerable(FdoString* name);

    // Returns an array of possible values for the specified property.
    SDF_API virtual FdoString** EnumeratePropertyValues(FdoString* name, FdoInt32& count);

    // Returns a localized name for the property (for NLS purposes).
    SDF_API virtual FdoString* GetLocalizedName(FdoString* name);

	// Determines if the specified property represents a file name.
 	SDF_API virtual bool IsPropertyFileName(FdoString* name);

	// <summary>Determines if the specified property represents a path name.
    SDF_API virtual bool IsPropertyFilePath(FdoString* name);

	// Determines if the specified property represents a datastore name.
	SDF_API virtual bool IsPropertyDatastoreName(FdoString* name);

    //-------------------------------------------------------
    // FmeConnectionPropertyDictionary implementation
    //-------------------------------------------------------

public:
    virtual FdoString* GenerateConnectionString();

    //removed entries from the property table.
    //Needed for when the user changes the connection string
    //of the connection using the manual SetConnectionString()
    virtual void Clear();

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

protected:
    SdfConnection* m_connection;
    std::map<std::wstring, std::wstring>* m_mProps;
    std::wstring m_connStr;

};

#endif


