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
/// This command gets all the image formats supported by a specified WMS service.
///</summary>
public ref class GetImageFormatsCommand : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::IGetImageFormats
{
public:
    /// <summary>
    /// Constructs a WMS GetImageFormats command using the result
    /// returned from IConnection::CreateCommand(). This constructor is provided 
    /// in order that clients are provided a mechanism to downcast from the FDO 
    /// ICommand interface to an FDO Provider specific interface that derives from ICommand. 
    /// This constructor is the only mechanism provided by the FDO Managed API that will 
    /// allow the client to correctly construct a provider specific ICommand instance. 
    /// </summary>
    GetImageFormatsCommand(NAMESPACE_OSGEO_FDO_COMMANDS::ICommand^ command, System::Boolean autoDelete);

    /// <summary>
    /// Executes the command. An exception is thrown if the feature class is nullptr.
    /// </summary>
    /// <returns>
    /// Returns a list of image formats supportted by WMS service.
    /// </returns>
    virtual NAMESPACE_OSGEO_COMMON::StringCollection^ Execute();

internal:
	GetImageFormatsCommand(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoWmsIGetImagefromats* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS
