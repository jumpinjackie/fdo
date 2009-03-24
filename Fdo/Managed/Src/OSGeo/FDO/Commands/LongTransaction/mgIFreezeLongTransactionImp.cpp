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
#include <Fdo\Commands\LongTransaction\IFreezeLongTransaction.h>

#include "FDO\Commands\LongTransaction\mgIFreezeLongTransactionImp.h"

FdoIFreezeLongTransaction* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IFreezeLongTransactionImp::GetImpObj()
{
    return static_cast<FdoIFreezeLongTransaction*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IFreezeLongTransactionImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IFreezeLongTransactionImp::Name::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IFreezeLongTransactionImp::Name::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionFreezeOperations NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IFreezeLongTransactionImp::Operation::get()
{
	FdoLongTransactionFreezeOperations result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetOperation())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionFreezeOperations>(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IFreezeLongTransactionImp::Operation::set(NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionFreezeOperations value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperation(static_cast<FdoLongTransactionFreezeOperations>(value)))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IFreezeLongTransactionImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}
