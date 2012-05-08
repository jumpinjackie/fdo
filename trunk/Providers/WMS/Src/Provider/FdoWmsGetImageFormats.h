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

#ifndef FdoWmsGetImageFormats_H
#define FdoWmsGetImageFormats_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "WMS/IGetImageFormats.h"
#include <FdoWmsCommand.h>

class FdoWmsGetImageFormats : public FdoWmsCommand<FdoWmsIGetImagefromats>
{
    friend class FdoWmsConnection;

protected:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoWmsGetImageFormats (const FdoWmsGetImageFormats &right);

    /// <summary>Constructs an instance of a command for the given connection. </summary>
    /// <returns>Returns nothing</returns> 
    FdoWmsGetImageFormats (FdoIConnection* connection);

    /// <summary>Virtual Destructor.</summary>
    /// <returns>Returns nothing</returns> 
    virtual ~ FdoWmsGetImageFormats (void);

    /// <summary>Dispose this object.</summary>
    /// <returns>Returns nothing</returns> 
    virtual void Dispose ();
    
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoWmsGetImageFormats & operator= (const FdoWmsGetImageFormats &right);

public:
    //
    /// <summary> 
    /// Executes the GetImageFormats command and returns a 
    /// FdoStringCollection, which contains all image formats supported by WMS service.
    /// </summary>
    /// <returns> 
    /// Returns the image format collection supported by WMS service.
    /// </returns> 
    virtual FdoStringCollection* Execute ();

};

#endif // FdoWmsGetImageFormats_H

