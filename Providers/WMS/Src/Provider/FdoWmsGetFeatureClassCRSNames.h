/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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

#ifndef FdoWmsGetFeatureClassCRSNames_H
#define FdoWmsGetFeatureClassCRSNames_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "WMS/IGetFeatureClassCRSNames.h"
#include <FdoWmsCommand.h>

class FdoWmsGetFeatureClassCRSNames : public FdoWmsCommand<FdoWmsIGetFeatureClassCRSNames>
{
    friend class FdoWmsConnection;

protected:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoWmsGetFeatureClassCRSNames (const FdoWmsGetFeatureClassCRSNames  &right);

    // <summary>Constructs an instance of a command for the given connection. </summary>
    // <returns>Returns nothing</returns> 
    FdoWmsGetFeatureClassCRSNames (FdoIConnection* connection);

    // <summary>Virtual Destructor.</summary>
    // <returns>Returns nothing</returns> 
    virtual ~FdoWmsGetFeatureClassCRSNames (void);

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    virtual void Dispose ();

    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoWmsGetFeatureClassCRSNames & operator= (const FdoWmsGetFeatureClassCRSNames  &right);

public:
    //
    /// <summary> Gets the name of the FeatureClass to get all supported CRS names. </summary>
    /// <returns>Returns the FeatureClass name</returns> 
    virtual FdoString* GetFeatureClassName ();

    /// <summary> 
    /// Sets the name of the FeatureClass to get all supported CRS names 
    /// This function is mandatory; if not specified, 
    /// execution of the command will throw exception. 
    /// </summary>
    /// <param name="value">Input the FeatureClass name</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetFeatureClassName (FdoString* value);

    /// <summary> 
    /// Executes the FdoWmsGetFeatureClassCRSNames command and returns a 
    /// FdoStringCollection, which contains all CRS names supported by specific FeatureClass.
    /// <returns> 
	/// Returns the CRS names collection supported by specific FeatureClass.
    /// </returns> 
    virtual FdoStringCollection* Execute ();

private:
    FdoStringP mFeatureClassName;
};

#endif // FdoWmsGetFeatureClassCRSNames_H
