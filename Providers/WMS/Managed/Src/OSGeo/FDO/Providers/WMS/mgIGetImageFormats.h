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

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS

/// <summary>
/// This command gets all the image formats supported by a specified WMS service.
/// </summary>
public __gc __interface IGetImageFormats : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// <summary>
    /// Executes the command. 
    /// </summary>
    /// <returns>
    /// Returns a list of image formats supportted by WMS service.
    /// </returns>
    NAMESPACE_OSGEO_COMMON::StringCollection* Execute();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS
