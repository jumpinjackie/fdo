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
	return static_cast<FdoProviderNameTokens*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::ProviderNameTokens(System::String^ name)
: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr(FdoProviderNameTokens::Create(StringToUni(name))), true)
{
}

array<System::String^>^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::GetNameTokens()
{
	FdoStringsP ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetNameTokens());
	return FdoStringsToStringArray(ret);
}

array<System::Double>^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::GetVersionTokens()
{
	FdoVectorP ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetVersionTokens());
	return FdoVectorToDoubleArrary(ret);
}

System::String^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::GetLocalName()
{
	FdoStringP ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetLocalName());
	return CHECK_STRING((FdoString*)ret);
}

array<System::String^>^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::FdoStringsToStringArray(const FdoStringsP &sa)
{
	FdoStringCollection *temp = sa;
	array<System::String^>^ ret = gcnew array<System::String^>(temp->GetCount());
	for(FdoInt32 i = 0; i < temp->GetCount(); ++i)
	{
        FdoString* str = (FdoString*)(temp->GetItem(i)->GetString());
		ret[i] = CHECK_STRING(str);
	}
	return ret;
}

array<System::Double>^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderNameTokens::FdoVectorToDoubleArrary(const FdoVectorP &da)
{
	FdoVector *temp = da;
	array<System::Double>^ ret = gcnew array<System::Double>(temp->GetCount());
	for(FdoInt32 i = 0; i < temp->GetCount(); ++i)
	{
		ret[i] = temp->GetItem(i)->GetValue();
	}
	return ret;
}

