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
#include "WMS\IGetFeatureClassStyles.h"
#include "mgIGetFeatureClassStyles.h"
#include "mgIGetFeatureClassStylesImp.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::IGetFeatureClassStylesImp::IGetFeatureClassStylesImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
{

}

FdoWmsIGetFeatureClassStyles* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::IGetFeatureClassStylesImp::GetImpObj()
{
    return static_cast<FdoWmsIGetFeatureClassStyles*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::IGetFeatureClassStylesImp::set_FeatureClassName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetFeatureClassName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::IGetFeatureClassStylesImp::get_FeatureClassName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureClassName())

    return result;
}

NAMESPACE_OSGEO_COMMON::StringCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::IGetFeatureClassStylesImp::Execute()
{
    FdoStringCollection* result;
	EXCEPTION_HANDLER(result = GetImpObj()->Execute());
    return (new NAMESPACE_OSGEO_COMMON::StringCollection(result, true));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::IGetFeatureClassStylesImp::Dispose()
{
	NAMESPACE_OSGEO_RUNTIME::Disposable::Dispose();
}
