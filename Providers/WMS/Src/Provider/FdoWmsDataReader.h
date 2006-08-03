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

#ifndef FDOWMSDATAREADER_H
#define FDOWMSDATAREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoWmsReader.h"

class FdoWmsDataReader : public virtual FdoWmsReader<FdoIDataReader>
{
    typedef FdoWmsReader<FdoIDataReader> superclass;
    friend class FdoWmsSelectAggregatesCommand;

protected:
    // <summary>Virtual Destructor.</summary>
    // <returns>Returns nothing</returns> 
    virtual ~FdoWmsDataReader (void);

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    virtual void Dispose();

public:
    // <summary>Default Constructor.</summary>
    // <returns>Returns nothing</returns> 
    FdoWmsDataReader ();

    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoWmsDataReader (const FdoWmsDataReader &right) { }

    /// <summary>Gets the number of propertys in the result set.</summary>
    /// <returns>Returns the number of propertys.</returns> 
    virtual FdoInt32 GetPropertyCount();

    /// <summary>Gets the name of the property at the given ordinal position.</summary>
    /// <param name="index">Input the position of the property.</param> 
    /// <returns>Returns the property name</returns> 
    virtual FdoString* GetPropertyName(FdoInt32 index);

    /// <summary>Gets the data type of the property with the specified name.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the type of the property.</returns> 
    virtual FdoDataType GetDataType (FdoString* propertyName);

    /// <summary>Gets the FDO property type of a given property. This is used
    //  to indicate if a given property is a geometric property or a data property. If the property is
    //  a FdoPropertyType_DataProperty, then GetDataType can be used to to find the data type of the property.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FDO property type.</returns> 
    virtual FdoPropertyType GetPropertyType (FdoString* propertyName);

    /// <summary>Advances the reader to the next item and returns true if there is
    /// another object to read or false if reading is complete. The default
    /// position of the reader is prior to the first item. Thus you must
    /// call ReadNext to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    virtual bool ReadNext ();
};

#endif // FDOWMSDATAREADER_H
