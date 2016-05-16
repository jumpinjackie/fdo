/*
* 
* Copyright (C) 2004-2006  Autodesk, Inc.
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
#include "mgProvider.h"
#include <Fdo/ClientServices/Provider.h>

FdoProvider* NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider::GetImpObj()
{
	return static_cast<FdoProvider*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider::Name::get()
{
	FdoString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetName());
	return CHECK_STRING(ret);
}

System::String^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider::DisplayName::get()
{
	FdoString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDisplayName());
	return CHECK_STRING(ret);
}

System::String^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider::Description::get()
{
	FdoString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDescription());
	return CHECK_STRING(ret);
}

System::String^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider::Version::get()
{
	FdoString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetVersion());
	return CHECK_STRING(ret);
}

System::String^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider::FeatureDataObjectsVersion::get()
{
	FdoString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetFeatureDataObjectsVersion());
	return CHECK_STRING(ret);
}

System::String^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider::LibraryPath::get()
{
	FdoString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetLibraryPath());
	return CHECK_STRING(ret);
}

System::Boolean NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider::IsManaged::get()
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->GetIsManaged())
	return ret;
}

