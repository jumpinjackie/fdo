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

class FdoWmsIGetImagefromats;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS

///<summary>
/// This command get all supported image format from a specified WMS service
///</summary>
private __gc class IGetImageFormatsImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::IGetImageFormats
{
public:
    ///<summary>
    /// Executes the command. An exception is thrown if the feature class is NULL.
    ///</summary>
    /// <returns>Returns a list of image formats supportted by WMS service.</returns>
    NAMESPACE_OSGEO_COMMON::StringCollection* Execute();

	System::Void Dispose();

public private:
	IGetImageFormatsImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoWmsIGetImagefromats* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS
