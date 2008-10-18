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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpFeatureCommand.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#ifndef FDORFPFEATURECOMMAND_H
#define FDORFPFEATURECOMMAND_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoRfpCommand.h"


template <class FDO_COMMAND> class FdoRfpFeatureCommand : public FdoRfpCommand<FDO_COMMAND>
{
    //friend class FdoRfpConnection;
    //friend class FdoRfpFeatureReader;

protected:
    FdoPtr<FdoFilter> mFilter;         // the command filter
    FdoPtr<FdoIdentifier> mClassName;  // the name of the feature class

protected:

    // Constructs an instance of a command for the given connection.    
    FdoRfpFeatureCommand (FdoIConnection* connection) :
        FdoRfpCommand<FDO_COMMAND>(connection)  // superclass constructor
    {
    }

    // Default destructor for a command.
    virtual ~FdoRfpFeatureCommand() 
    { 
    }

    virtual void Dispose()
    {
        delete this;
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
};



/// <summary>Gets the filter as a filter tree.</summary>
/// <returns>Returns the filter object</returns> 
template <class FDO_COMMAND> 
FdoFilter* FdoRfpFeatureCommand<FDO_COMMAND>::GetFilter ()
{
    //TODO: should I return an empty filter here?
    return (FDO_SAFE_ADDREF(mFilter.p));
}

/// <summary>Sets the filter as a Filter tree.</summary>
/// <param name="value">Input the filter object</param> 
/// <returns>Returns nothing</returns> 
template <class FDO_COMMAND> 
void FdoRfpFeatureCommand<FDO_COMMAND>::SetFilter (FdoFilter* value)
{
    mFilter = FDO_SAFE_ADDREF(value);
}

/// <summary>Sets the filter as a Filter tree.</summary>
/// <param name="value">Input the filter expression string</param> 
/// <returns>Returns nothing</returns>
template <class FDO_COMMAND> 
void FdoRfpFeatureCommand<FDO_COMMAND>::SetFilter (FdoString* value)
{
    mFilter = FdoFilter::Parse (value);
}



/// <summary>Gets the name of the class to be operated upon as an identifier.</summary>
/// <returns>Returns the class identifier.</returns> 
template <class FDO_COMMAND> 
FdoIdentifier* FdoRfpFeatureCommand<FDO_COMMAND>::GetFeatureClassName ()
{
    // addref the class name because we own it
    return (FDO_SAFE_ADDREF(mClassName.p));
}


/// <summary>Sets the name of the class to be operated upon as an identifier.</summary>
/// <param name="value">Input the class identifier</param> 
/// <returns>Returns nothing.</returns> 
template <class FDO_COMMAND> 
void FdoRfpFeatureCommand<FDO_COMMAND>::SetFeatureClassName (FdoIdentifier* value)
{
    mClassName = FDO_SAFE_ADDREF(value);  // does automatic release
}


/// <summary>Sets the name of the class to be operated upon as an identifier.</summary>
/// <param name="value">Input the class name</param> 
/// <returns>Returns nothing</returns>
template <class FDO_COMMAND> 
void FdoRfpFeatureCommand<FDO_COMMAND>::SetFeatureClassName (FdoString* value)
{
	mClassName = FdoIdentifier::Create (value);
}


#endif // FDORFPFEATURECOMMAND_H
