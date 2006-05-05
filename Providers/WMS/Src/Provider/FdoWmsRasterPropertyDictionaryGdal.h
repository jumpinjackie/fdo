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
 *
 */
#ifndef FDOWMSRASTERPROPERTYDICTIONARYGDAL_H
#define FDOWMSRASTERPROPERTYDICTIONARYGDAL_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

//
// class forward declarations
//
class FdoWmsBandRasterGdal;

//
// implementation of FdoIRasterPropertyDictionary
//
class FdoWmsRasterPropertyDictionayGdal : public FdoIRasterPropertyDictionary
{
	friend class FdoWmsBandRasterGdal;
//
// data members
//
private:
	FdoPtr<FdoWmsBandRasterGdal>	m_raster;

//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoWmsRasterPropertyDictionayGdal(FdoWmsBandRasterGdal* raster);
	virtual ~FdoWmsRasterPropertyDictionayGdal();
	void Dispose() { delete this; }

//
// exposed functions
//
public:

    /// <summary>Gets the names of all the properties that apply to this
    /// image as a collection of Strings. The order of the property names
    /// in the collection dictate the order in which they need to
    /// be specified. This is especially important for the success of
    /// the EnumeratePropertyValues method as properties that occur
    /// earlier in the collection may be required for successful enumeration
    /// of properties that appear later in the collection.</summary>
    /// <returns>A collection of the names of properties in this dictionary.</returns>
    virtual FdoStringCollection* GetPropertyNames ();

    /// <summary>Gets the data type for the specified property.</summary>
    /// <param name="name">The name of the property to get the data type of.</param>
    /// <returns>The data type of the property.</returns>
    virtual FdoDataType GetPropertyDataType (FdoString* name);

    /// <summary>Gets the value of the specified property (or its default).</summary>
    /// <param name="name">The name of the property to get the value of.</param>
    /// <returns>The value of the property.</returns>
    virtual FdoDataValue* GetProperty (FdoString* name);

    /// <summary>Sets the value of the specified property.</summary>
    /// <param name="name">The name of the property to set the value of.</param>
    /// <param name="value">The new value for the property.</param>
    virtual void SetProperty (FdoString* name, FdoDataValue* value);

    /// <summary>Gets the default value for the specified property.</summary>
    /// <param name="name">The name of the property to get the default value of.</param>
    /// <returns>The default value of the property.</returns>
    virtual FdoDataValue* GetPropertyDefault (FdoString* name);

    /// <summary>Predicate to determine if the property is required to be set.</summary>
    /// <param name="name">The name of the property to get the required status of.</param>
    /// <returns>Returns true if the specified property is required, false if it is optional.</returns>
    virtual bool IsPropertyRequired (FdoString* name);

    /// <summary>Predicate to determine if the property is enumerable (has more than one value).</summary>
    /// <param name="name">The name of the property to get the enumerable status of.</param>
    /// <returns>Returns true if the possible values for the specified property
    /// can be enumerated via the GetPropertyValues method.</returns>
    virtual bool IsPropertyEnumerable (FdoString* name);

    /// <summary>Access the values of the specified enumerable property.
    /// The property must respond TRUE to IsPropertyEnumerable.</summary>
    /// <param name="name">The name of the property to get the collection of values for.</param>
    /// <returns>A collection of datavaluesfor the enumerable property.</returns>
    virtual FdoDataValueCollection* GetPropertyValues (FdoString* name);

    /// <summary>Set the values of the specified enumerable property.
    /// The property must respond TRUE to IsPropertyEnumerable.
    /// Clients will need to use SetPropertyValues when creating an image
    /// with specific values for the enumerable property.
    /// An example would be setting a table of wavelength ranges corresponding
    /// to the false colour used to encode the image.</summary>
    /// <param name="name">The name of the property to set the collection of values for.</param>
    /// <param name="collection">The collection of values for the enumerable property.</param>
    virtual void SetPropertyValues (FdoString* name, FdoDataValueCollection* collection);

//
// internally used helper functions
//
private:

};

#endif
