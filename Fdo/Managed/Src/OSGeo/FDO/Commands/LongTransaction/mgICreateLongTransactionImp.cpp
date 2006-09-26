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
#include <Fdo\Commands\LongTransaction\ICreateLongTransaction.h>

#include "FDO\Commands\LongTransaction\mgICreateLongTransactionImp.h"

FdoICreateLongTransaction* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionImp::GetImpObj()
{
    return static_cast<FdoICreateLongTransaction*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionImp::get_Name()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

		return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionImp::set_Name(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionImp::get_Description()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())

		return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionImp::set_Description(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDescription(StringToUni(value)))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}
