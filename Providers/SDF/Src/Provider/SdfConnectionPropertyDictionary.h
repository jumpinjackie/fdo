#ifndef SDFCONNECTIONPROPERTYDICTIONARY_H
#define SDFCONNECTIONPROPERTYDICTIONARY_H
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
#include "SdfPropertyDictionary.h"

class SdfConnectionPropertyDictionary : public SdfPropertyDictionary
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------
	
	friend class SdfConnection;

public:
    // constructs a FmeConnectionPropertyDictionary using the specified connection
    SdfConnectionPropertyDictionary(SdfConnection* connection);

	// Gets the names of all the properties that can appear in a connection string
	// for this feature provider as an array of Strings.  The order of the property
	// names in the resulting array dictate the order in which they need to be
	// specified.  This is especially important for the success of the
	// EnumeratePropertyValues method as properties that occur earlier in the array
	// may be required for successful enumeration of properties that appear later
	// in the array.  The count parameter returns the size of the returned array.
	FdoString** GetPropertyNames(FdoInt32& count);

};

#endif

