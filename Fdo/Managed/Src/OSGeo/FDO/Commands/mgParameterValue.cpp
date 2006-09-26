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
#include <Fdo\Commands\ParameterValue.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\mgParameterValue.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Expression\mgLiteralValue.h"

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue::ParameterValue() : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoParameterValue::Create(), true))
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue::ParameterValue(System::String* name) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoParameterValue::Create(StringToUni(name)), true))
}

FdoParameterValue* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue::GetImpObj()
{
    return static_cast<FdoParameterValue*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue::ParameterValue(System::String* name, NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue* value) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoParameterValue::Create(StringToUni(name), (value == NULL ? NULL : value->GetImpObj())), true))
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue::get_Name()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue::set_Name(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue::get_Value()
{
	FdoLiteralValue* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetValue())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateLiteralValue(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue::set_Value(NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetValue((value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue::set_Value(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetValue(StringToUni(value)))
}
