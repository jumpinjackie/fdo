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

#pragma once

// This number is defined so our custom command would not clash with other
// custom provider commands.
#define WMS_FIRST_PROVIDER_COMMAND 1800

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS

/// <summary>The CommandType enumeration defines the list of WMS commands.</summary>
/// <param name="CommandType_GetImageFormats">Represents the FdoWmsIGetImagefromats command.</param>  
/// <param name="CommandType_GetFeatureClassStyles">Represents the FdoWmsIGetFeatureClassStyles command.</param> 
/// <param name="CommandType_GetFeatureClassCRSNames">Represents the FdoWmsIGetFeatureClassCRSNames command.</param> 
public __value enum CommandType {

    CommandType_GetImageFormats =  NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_FirstProviderCommand +  WMS_FIRST_PROVIDER_COMMAND,
    CommandType_GetFeatureClassStyles,
    CommandType_GetFeatureClassCRSNames

};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS
