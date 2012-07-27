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

#include "FdoWmsGetFeatureClassStyles.h"

FdoWmsGetFeatureClassStyles::FdoWmsGetFeatureClassStyles (FdoIConnection *connection) :
    FdoWmsCommand<FdoWmsIGetFeatureClassStyles> (connection)
{
}

FdoWmsGetFeatureClassStyles::~FdoWmsGetFeatureClassStyles (void)
{
}

/// <summary>Dispose this object.</summary>
/// <returns>Returns nothing.</returns> 
void FdoWmsGetFeatureClassStyles::Dispose ()
{
    delete this;
}

/// <summary> Gets the name of the FeatureClass to get all supported styles. </summary>
/// <returns>Returns the FeatureClass name</returns> 
FdoString* FdoWmsGetFeatureClassStyles::GetFeatureClassName()
{
    return (mFeatureClassName);
}

/// <summary> 
/// Sets the name of the FeatureClass to get all supported styles 
/// This function is mandatory; if not specified, 
/// execution of the command will throw exception. 
/// </summary>
/// <param name="value">Input the FeatureClass name</param> 
/// <returns>Returns nothing</returns> 
void FdoWmsGetFeatureClassStyles::SetFeatureClassName(FdoString* value)
{
    mFeatureClassName = value;
}

/// <summary> 
/// Executes the FdoWmsGetFeatureClassStyles command and returns a 
/// FdoStringCollection, which contains all styles supported by specific FeatureClass.
/// </summary>
/// <returns> Returns the style collection supported by FeatureClass. </returns> 
FdoStringCollection* FdoWmsGetFeatureClassStyles::Execute ()
{
    VALIDATE_ARGUMENT(mFeatureClassName);

    FdoStringsP styleNames = mConnection->GetSupportedStyles(mFeatureClassName);
    
    return FDO_SAFE_ADDREF(styleNames.p);
}
