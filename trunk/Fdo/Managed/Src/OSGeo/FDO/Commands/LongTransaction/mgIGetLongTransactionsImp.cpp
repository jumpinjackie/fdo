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
#include <Fdo\Commands\LongTransaction\IGetLongTransactions.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\LongTransaction\mgIGetLongTransactionsImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\LongTransaction\mgILongTransactionReaderImp.h"

FdoIGetLongTransactions* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionsImp::GetImpObj()
{
    return static_cast<FdoIGetLongTransactions*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionsImp::get_Name()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionsImp::set_Name(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionsImp::Execute()
{
	FdoILongTransactionReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->Execute())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILongTransactionReader(result, true);
}
