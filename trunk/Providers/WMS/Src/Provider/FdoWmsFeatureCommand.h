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

#ifndef FDOWMSFEATURECOMMAND_H
#define FDOWMSFEATURECOMMAND_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoWmsCommand.h"

template <class FDO_COMMAND> class FdoWmsFeatureCommand : public FdoWmsCommand<FDO_COMMAND>
{
    friend class FdoWmsConnection;
    friend class FdoWmsFeatureReader;

protected:

    // <summary>Constructs an instance of a command for the given connection. </summary>
    // <returns>Returns nothing</returns> 
    FdoWmsFeatureCommand (FdoIConnection* connection) :
        FdoWmsCommand<FDO_COMMAND>(connection)  // superclass constructor
    {
    }

    // <summary>Virtual Destructor.</summary>
    // <returns>Returns nothing</returns> 
    virtual ~FdoWmsFeatureCommand() 
    { 
    }

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    virtual void Dispose()
    {
        delete this;
    }

    /// <summary>Gets the FdoPropertyValueCollection that specifies the names and values
    /// of the properties for the instance to be inserted OR updated.</summary>
    /// <returns>Returns the list of properties and their values.</returns> 
    virtual FdoPropertyValueCollection* GetPropertyValues ()
    {
        // allocate a new property value collection if not yet done
        if (mValues == NULL)
            mValues = FdoPropertyValueCollection::Create ();

        return (FDO_SAFE_ADDREF(mValues.p));
    }

public:

    /// <summary>Gets the filter as a filter tree.</summary>
    /// <returns>Returns the filter object</returns> 
    virtual FdoFilter* GetFilter ();

    /// <summary>Sets the filter as a Filter tree.</summary>
    /// <param name="value">Input the filter object</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetFilter (FdoFilter* value);

    /// <summary>Sets the filter as a Filter tree.</summary>
    /// <param name="value">Input the filter expression string</param> 
    /// <returns>Returns nothing</returns>
    virtual void SetFilter (FdoString* value);

    /// <summary>Gets the name of the class to be operated upon as an identifier.</summary>
    /// <returns>Returns the class identifier.</returns> 
    virtual FdoIdentifier* GetFeatureClassName ();

    /// <summary>Sets the name of the class to be operated upon as an identifier.</summary>
    /// <param name="value">Input the class identifier</param> 
    /// <returns>Returns nothing.</returns> 
    virtual void SetFeatureClassName (FdoIdentifier* value);

    /// <summary>Sets the name of the class to be operated upon as an identifier.</summary>
    /// <param name="value">Input the class name</param> 
    /// <returns>Returns nothing</returns>
    virtual void SetFeatureClassName (FdoString* value);

protected:
    FdoPtr<FdoFilter> mFilter;         // the command filter
    FdoPtr<FdoIdentifier> mClassName;  // the name of the feature class
    FdoPtr<FdoPropertyValueCollection> mValues;  // the collection of values to insert or update
};



/// <summary>Gets the filter as a filter tree.</summary>
/// <returns>Returns the filter object</returns> 
template <class FDO_COMMAND> 
FdoFilter* FdoWmsFeatureCommand<FDO_COMMAND>::GetFilter ()
{
    return (FDO_SAFE_ADDREF(mFilter.p));
}

/// <summary>Sets the filter as a Filter tree.</summary>
/// <param name="value">Input the filter object</param> 
/// <returns>Returns nothing</returns> 
template <class FDO_COMMAND> 
void FdoWmsFeatureCommand<FDO_COMMAND>::SetFilter (FdoFilter* value)
{
    mFilter = FDO_SAFE_ADDREF(value);
}

/// <summary>Sets the filter as a Filter tree.</summary>
/// <param name="value">Input the filter expression string</param> 
/// <returns>Returns nothing</returns>
template <class FDO_COMMAND> 
void FdoWmsFeatureCommand<FDO_COMMAND>::SetFilter (FdoString* value)
{
    FdoPtr<FdoFilter> filter = FdoFilter::Parse (value);
    mFilter = FDO_SAFE_ADDREF(filter.p);
}


/// <summary>Gets the name of the class to be operated upon as an identifier.</summary>
/// <returns>Returns the class identifier.</returns> 
template <class FDO_COMMAND> 
FdoIdentifier* FdoWmsFeatureCommand<FDO_COMMAND>::GetFeatureClassName ()
{
    // addref the class name because we own it
    return (FDO_SAFE_ADDREF(mClassName.p));
}


/// <summary>Sets the name of the class to be operated upon as an identifier.</summary>
/// <param name="value">Input the class identifier</param> 
/// <returns>Returns nothing.</returns> 
template <class FDO_COMMAND> 
void FdoWmsFeatureCommand<FDO_COMMAND>::SetFeatureClassName (FdoIdentifier* value)
{
    mClassName = FDO_SAFE_ADDREF(value);
}


/// <summary>Sets the name of the class to be operated upon as an identifier.</summary>
/// <param name="value">Input the class name</param> 
/// <returns>Returns nothing</returns>
template <class FDO_COMMAND> 
void FdoWmsFeatureCommand<FDO_COMMAND>::SetFeatureClassName (FdoString* value)
{
    FdoPtr<FdoIdentifier> class_name;

    if (value != NULL)
        class_name = FdoIdentifier::Create (value);
    else
        class_name = NULL;

    SetFeatureClassName (class_name);
}


#endif // FDOWMSFEATURECOMMAND_H
