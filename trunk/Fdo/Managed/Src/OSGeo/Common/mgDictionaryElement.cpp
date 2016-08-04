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
#include "Common\mgDictionaryElement.h"
#include "Common\mgException.h"

NAMESPACE_OSGEO_COMMON::DictionaryElement::DictionaryElement(System::String^ name, System::String^ value) : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDictionaryElement::Create(StringToUni(name), StringToUni(value))), true))
}

NAMESPACE_OSGEO_COMMON::DictionaryElement::DictionaryElement(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoDictionaryElement* NAMESPACE_OSGEO_COMMON::DictionaryElement::GetImpObj()
{
	return static_cast<FdoDictionaryElement*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_COMMON::DictionaryElement::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_COMMON::DictionaryElement::Name::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return CHECK_STRING(result);
}

System::Boolean NAMESPACE_OSGEO_COMMON::DictionaryElement::CanSetName::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->CanSetName())

	return result;
}

System::String^ NAMESPACE_OSGEO_COMMON::DictionaryElement::Value::get()
{
	FdoString* result;
	
	EXCEPTION_HANDLER(result = GetImpObj()->GetValue())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElement::Value::set(String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetValue(StringToUni(value)))
}
