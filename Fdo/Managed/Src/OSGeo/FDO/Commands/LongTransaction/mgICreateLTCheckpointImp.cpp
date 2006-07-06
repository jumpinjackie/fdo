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
#include <Fdo\Commands\LongTransaction\ICreateLongTransactionCheckpoint.h>
#include "FDO\Commands\LongTransaction\mgICreateLTCheckpointImp.h"

FdoICreateLongTransactionCheckpoint* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionCheckpointImp::GetImpObj()
{
    return static_cast<FdoICreateLongTransactionCheckpoint*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionCheckpointImp::get_LongTransactionName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLongTransactionName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionCheckpointImp::set_LongTransactionName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLongTransactionName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionCheckpointImp::get_CheckpointName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCheckpointName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionCheckpointImp::set_CheckpointName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCheckpointName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionCheckpointImp::get_CheckpointDescription()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCheckpointDescription())

		return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionCheckpointImp::set_CheckpointDescription(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCheckpointDescription(StringToUni(value)))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionCheckpointImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionCheckpointImp::Dispose()
{
	NAMESPACE_OSGEO_RUNTIME::Disposable::Dispose();
}
