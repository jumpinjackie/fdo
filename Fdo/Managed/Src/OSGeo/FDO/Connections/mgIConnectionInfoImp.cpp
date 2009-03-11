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

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::ProviderName::get()
{
	FdoString* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetProviderName())
	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::ProviderDisplayName::get()
{
	FdoString* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetProviderDisplayName())
	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::ProviderDescription::get()
{
	FdoString* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetProviderDescription())
	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::ProviderVersion::get()
{
	FdoString* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetProviderVersion())
	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::FeatureDataObjectsVersion::get()
{
	FdoString* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureDataObjectsVersion())
	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionary^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::ConnectionProperties::get()
{
	FdoIConnectionPropertyDictionary* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetConnectionProperties())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnectionPropertyDictionary(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::ProviderDatastoreType NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::ProviderDatastoreType::get()
{
    FdoProviderDatastoreType pdsType;
    EXCEPTION_HANDLER(pdsType = GetImpObj()->GetProviderDatastoreType());
    return static_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS::ProviderDatastoreType>(pdsType);
}

NAMESPACE_OSGEO_COMMON::StringCollection^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp::DependentFileNames::get()
{
	FdoStringCollection* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetDependentFileNames())
	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateStringCollection(IntPtr(result), true);
}

