/*
* 
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
#include "WMS\IGetImageFormats.h"
#include "mgIGetImageFormats.h"
#include "mgIGetImageFormatsImp.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::GetImageFormatsCommand::GetImageFormatsCommand(NAMESPACE_OSGEO_FDO_COMMANDS::ICommand^ command, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(command, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::GetImageFormatsCommand::GetImageFormatsCommand(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
{
}

FdoWmsIGetImagefromats* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::GetImageFormatsCommand::GetImpObj()
{
    return static_cast<FdoWmsIGetImagefromats*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::GetImageFormatsCommand::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_COMMON::StringCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::GetImageFormatsCommand::Execute()
{
    FdoStringCollection* result;
	EXCEPTION_HANDLER(result = GetImpObj()->Execute());
    return (gcnew NAMESPACE_OSGEO_COMMON::StringCollection(IntPtr(result), true));
}
