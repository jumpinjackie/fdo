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
#include <Fdo\Commands\LongTransaction\IChangeLongTransactionPrivileges.h>
#include "FDO\Commands\LongTransaction\mgIChangeLTPrivilegesImp.h"

FdoIChangeLongTransactionPrivileges* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp::GetImpObj()
{
    return static_cast<FdoIChangeLongTransactionPrivileges*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp::get_LongTransactionName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLongTransactionName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp::set_LongTransactionName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLongTransactionName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp::get_UserName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetUserName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp::set_UserName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetUserName(StringToUni(value)))
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp::get_Privileges()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPrivileges())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp::set_Privileges(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetPrivileges(value))
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionPrivilegeOperations NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp::get_Operation()
{
	FdoLongTransactionPrivilegeOperations result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetOperation())

		return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionPrivilegeOperations>(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp::set_Operation(NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionPrivilegeOperations value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperation(static_cast<FdoLongTransactionPrivilegeOperations>(value)))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp::Dispose()
{
	NAMESPACE_OSGEO_RUNTIME::Disposable::Dispose();
}
