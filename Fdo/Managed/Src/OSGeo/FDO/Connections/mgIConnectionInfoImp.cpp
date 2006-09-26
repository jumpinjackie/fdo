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
#include <Fdo.h>

#include "FDO\Connections\mgIConnectionInfoImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Connections\mgIConnectionPropertyDictionaryImp.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::IConnectionInfoImp(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoIConnectionInfo* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::GetImpObj()
{
    return static_cast<FdoIConnectionInfo*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

System::String* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::get_ProviderName()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetProviderName())
	return unstr;
}

System::String* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::get_ProviderDisplayName()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetProviderDisplayName())
	return unstr;
}

System::String* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::get_ProviderDescription()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetProviderDescription())
	return unstr;
}

System::String* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::get_ProviderVersion()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetProviderVersion())
	return unstr;
}

System::String* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::get_FeatureDataObjectsVersion()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetFeatureDataObjectsVersion())
	return unstr;
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionary* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::get_ConnectionProperties()
{
	FdoIConnectionPropertyDictionary* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetConnectionProperties())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnectionPropertyDictionary(unobj, true);
}
