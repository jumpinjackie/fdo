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
#include <Fdo\Commands\LongTransaction\IGetLongTransactionsInSet.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\LongTransaction\mgIGetLongTransactionsInSetImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\LongTransaction\mgILongTransactionSetReaderImp.h"

FdoIGetLongTransactionsInSet* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionsInSetImp::GetImpObj()
{
    return static_cast<FdoIGetLongTransactionsInSet*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionSetReader^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionsInSetImp::Execute()
{
	FdoILongTransactionSetReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->Execute())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILongTransactionSetReader(IntPtr(result), true);
}
