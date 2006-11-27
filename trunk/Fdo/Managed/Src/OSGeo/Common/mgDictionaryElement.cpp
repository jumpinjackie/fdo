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

NAMESPACE_OSGEO_COMMON::DictionaryElement::DictionaryElement(System::String* name, System::String* value) : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDictionaryElement::Create(StringToUni(name), StringToUni(value)), true))
}

NAMESPACE_OSGEO_COMMON::DictionaryElement::DictionaryElement(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoDictionaryElement* NAMESPACE_OSGEO_COMMON::DictionaryElement::GetImpObj()
{
	return static_cast<FdoDictionaryElement*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElement::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

System::String* NAMESPACE_OSGEO_COMMON::DictionaryElement::get_Name()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return result;
}

System::Boolean NAMESPACE_OSGEO_COMMON::DictionaryElement::get_CanSetName()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->CanSetName())

	return result;
}

System::String* NAMESPACE_OSGEO_COMMON::DictionaryElement::get_Value()
{
	FdoString* result;
	
	EXCEPTION_HANDLER(result = GetImpObj()->GetValue())

	return result;
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElement::set_Value(String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetValue(StringToUni(value)))
}
