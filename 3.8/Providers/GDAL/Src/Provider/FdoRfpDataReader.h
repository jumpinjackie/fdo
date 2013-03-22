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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpDataReader.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#ifndef FDORFPDATAREADER_H
#define FDORFPDATAREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoRfpCommonReader.h"
#include "FdoDefaultDataReader.h"
//
// Class forward declarations
//

//
// Implementation of FdoIDataReader
//
class FdoRfpDataReader :
	public FdoRfpCommonReader<FdoDefaultDataReader>
{
    typedef FdoRfpCommonReader<FdoDefaultDataReader> superclass;
//
// Data members
//
private:

//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoRfpDataReader(const FdoPtr<FdoRfpQueryResult>& queryResult);
public:
	// Static factory function
	static FdoRfpDataReader* Create(const FdoPtr<FdoRfpQueryResult>& queryResult);

// 
// Exposed functions
//
public:
    /// <summary>Gets the number of propertys in the result set.</summary>
    /// <returns>Returns the number of propertys.</returns> 
    virtual FdoInt32 GetPropertyCount();

    /// <summary>Gets the name of the property at the given ordinal position.</summary>
    /// <param name="index">Input the position of the property.</param> 
    /// <returns>Returns the property name</returns> 
    virtual FdoString* GetPropertyName(FdoInt32 index);

    /// <summary>Gets the index of the property with the specified name.</summary>
    /// <param name="propertyName">Input the name of the property.</param> 
    /// <returns>Returns the property index</returns> 
    virtual FdoInt32 GetPropertyIndex(FdoString* propertyName);

    /// <summary>Gets the data type of the property with the specified name.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the type of the property.</returns> 
    virtual FdoDataType GetDataType(FdoString* propertyName);
    
    /// <summary>Gets the FDO property type of a given property. This is used
    //  to indicate if a given property is a geometric property or a data property. If the property is
    //  a FdoPropertyType_Data, then GetDataType can be used to to find the data type of the property.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FDO property type.</returns> 
    virtual FdoPropertyType GetPropertyType(FdoString* propertyName);

//
// Internally used helper functions
//
private:

};

typedef FdoPtr<FdoRfpDataReader> FdoRfpDataReaderP;


#endif
