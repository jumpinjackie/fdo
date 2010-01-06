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

#include "FdoWmsGetFeatureClassCRSNames.h"

FdoWmsGetFeatureClassCRSNames::FdoWmsGetFeatureClassCRSNames (FdoIConnection *connection) :
    FdoWmsCommand<FdoWmsIGetFeatureClassCRSNames> (connection)
{
}

FdoWmsGetFeatureClassCRSNames::~FdoWmsGetFeatureClassCRSNames (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing.</returns> 
void FdoWmsGetFeatureClassCRSNames::Dispose ()
{
    delete this;
}

/// <summary> Gets the name of the FeatureClass to get all supported CRS names. </summary>
/// <returns>Returns the FeatureClass name</returns> 
FdoString* FdoWmsGetFeatureClassCRSNames::GetFeatureClassName()
{
    return (mFeatureClassName);
}

/// <summary> 
/// Sets the name of the FeatureClass to get all supported CRS names 
/// This function is mandatory; if not specified, 
/// execution of the command will throw exception. 
/// </summary>
/// <param name="value">Input the FeatureClass name</param> 
/// <returns>Returns nothing</returns> 
void FdoWmsGetFeatureClassCRSNames::SetFeatureClassName(FdoString* value)
{
    mFeatureClassName = value;
}

/// <summary> 
/// Executes the FdoWmsGetFeatureClassCRSNames command and returns a 
/// FdoStringCollection, which contains all CRS names supported by specific FeatureClass.
/// </summary>
/// <returns> Returns the CRS names collection supported by specific FeatureClass.</returns> 
FdoStringCollection* FdoWmsGetFeatureClassCRSNames::Execute ()
{
    VALIDATE_ARGUMENT(mFeatureClassName);

    FdoStringsP crsNames = mConnection->GetSupportedCRSNames(mFeatureClassName);
    
    return FDO_SAFE_ADDREF(crsNames.p);
}
