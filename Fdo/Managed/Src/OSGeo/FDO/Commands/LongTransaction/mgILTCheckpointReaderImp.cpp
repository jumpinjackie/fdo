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
#include <Fdo\Commands\LongTransaction\ILongTransactionCheckpointReader.h>
#include "FDO\Commands\LongTransaction\mgILTCheckpointReaderImp.h"

FdoILongTransactionCheckpointReader* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReaderImp::GetImpObj()
{
    return static_cast<FdoILongTransactionCheckpointReader*>(__super::UnmanagedObject.ToPointer());
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReaderImp::GetCheckpointName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCheckpointName())

	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReaderImp::GetCheckpointDescription()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCheckpointDescription())

	return CHECK_STRING(result);
}

System::DateTime NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReaderImp::GetCreationDate()
{
	FdoDateTime result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCreationDate())

	return FdoDateTimeToDateTime(result);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReaderImp::IsActive()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->IsActive())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReaderImp::ReadNext()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->ReadNext())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReaderImp::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}
