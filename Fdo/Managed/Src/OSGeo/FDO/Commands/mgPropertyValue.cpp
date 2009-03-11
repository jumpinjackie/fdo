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
#include <Fdo\Commands\PropertyValue.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\mgPropertyValue.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Expression\mgIdentifier.h"
#include "FDO\Expression\mgValueExpression.h"

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::PropertyValue() : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoPropertyValue::Create()), true))
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::PropertyValue(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ name, NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ value) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoPropertyValue::Create(name->GetImpObj(), (value == nullptr ? nullptr : value->GetImpObj()))), true))
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::PropertyValue(System::String^ name, NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ value) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoPropertyValue::Create(StringToUni(name), (value == nullptr ? nullptr : value->GetImpObj()))), true))
}

FdoPropertyValue* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::GetImpObj()
{
    return static_cast<FdoPropertyValue*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::Name::get()
{
	FdoIdentifier* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIdentifier(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::Name::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::SetName(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::Value::get()
{
	FdoValueExpression* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetValue())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateValueExpression(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::Value::set(NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetValue((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::SetValue(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetValue(StringToUni(value)))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::StreamReader::set(NAMESPACE_OSGEO_COMMON::IStreamReader^ stream)
{
	EXCEPTION_HANDLER(GetImpObj()->SetStreamReader(static_cast<FdoIStreamReader*>(static_cast<NAMESPACE_OSGEO_COMMON::IStreamReaderImp^>(stream)->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_COMMON::IStreamReader^ NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue::StreamReader::get()
{
	FdoIStreamReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetStreamReader())

    return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIStreamReader(IntPtr(result), true);
}
