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
#include "mgProviderNameTokens.h"
#include <Fdo/ClientServices/ProviderNameTokens.h>

FdoProviderNameTokens* NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::GetImpObj()
{
	return static_cast<FdoProviderNameTokens*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::ProviderNameTokens(System::String* name)
: NAMESPACE_OSGEO_RUNTIME::Disposable(FdoProviderNameTokens::Create(StringToUni(name)), true)
{
}

System::String *NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::GetNameTokens()[]
{
	FdoStringsP ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetNameTokens());
	return FdoStringsToStringArray(ret);
}

System::Double NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::GetVersionTokens()[]
{
	FdoVectorP ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetVersionTokens());
	return FdoVectorToDoubleArrary(ret);
}

System::String *NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::GetLocalName()
{
	FdoStringP ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetLocalName());
	return (FdoString *)ret;
}

System::String *NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::FdoStringsToStringArray(const FdoStringsP &sa)[]
{
	FdoStringCollection *temp = sa;
	System::String *ret[] = __gc new System::String*[temp->GetCount()];
	for(FdoInt32 i = 0; i < temp->GetCount(); ++i)
	{
		ret[i] = (FdoString *)(temp->GetItem(i)->GetString());
	}
	return ret;
}

System::Double NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::FdoVectorToDoubleArrary(const FdoVectorP &da)[]
{
	FdoVector *temp = da;
	System::Double ret __gc[] = __gc new System::Double[temp->GetCount()];
	for(FdoInt32 i = 0; i < temp->GetCount(); ++i)
	{
		ret[i] = temp->GetItem(i)->GetValue();
	}
	return ret;
}

