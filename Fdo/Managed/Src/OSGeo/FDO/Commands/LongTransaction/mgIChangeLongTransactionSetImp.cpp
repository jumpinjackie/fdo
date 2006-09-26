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
#include <Fdo\Commands\LongTransaction\IChangeLongTransactionSet.h>

#include "FDO\Commands\LongTransaction\mgIChangeLongTransactionSetImp.h"

FdoIChangeLongTransactionSet* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionSetImp::GetImpObj()
{
    return static_cast<FdoIChangeLongTransactionSet*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionSetImp::get_LongTransactionName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLongTransactionName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionSetImp::set_LongTransactionName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLongTransactionName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionSetOperations NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionSetImp::get_Operation()
{
	FdoLongTransactionSetOperations result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetOperation())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionSetOperations>(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionSetImp::set_Operation(NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionSetOperations value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperation(static_cast<FdoLongTransactionSetOperations>(value)))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionSetImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}
