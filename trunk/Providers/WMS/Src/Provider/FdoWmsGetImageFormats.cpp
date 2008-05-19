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

#include "stdafx.h"

#include <malloc.h>
#include <string.h>

#include "FdoWmsGetImageFormats.h"

FdoWmsGetImageFormats::FdoWmsGetImageFormats (FdoIConnection *connection) :
    FdoWmsCommand<FdoWmsIGetImagefromats> (connection)
{
}

FdoWmsGetImageFormats::~FdoWmsGetImageFormats (void)
{
}

/// <summary>Dispose this object.</summary>
/// <returns>Returns nothing.</returns> 
void FdoWmsGetImageFormats::Dispose ()
{
    delete this;
}


/// <summary> 
/// Executes the FdoWmsGetImageFormats command and returns a 
/// FdoStringCollection, which contains all image formats supported by WMS server.
/// </summary>
/// <returns> 
/// Returns the image format collection supported by WMS server.
/// </returns> 
FdoStringCollection* FdoWmsGetImageFormats::Execute ()
{
    FdoStringsP imageFormats = mConnection->GetSupportedImageFormats();

    return FDO_SAFE_ADDREF(imageFormats.p);
}
