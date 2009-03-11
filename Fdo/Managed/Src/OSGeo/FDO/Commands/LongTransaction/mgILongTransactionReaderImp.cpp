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
#include <Fdo\Commands\LongTransaction\ILongTransactionReader.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\LongTransaction\mgILongTransactionReaderImp.h"
#include "FDO\mgObjectFactory.h"

FdoILongTransactionReader* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp::GetImpObj()
{
    return static_cast<FdoILongTransactionReader*>(__super::UnmanagedObject.ToPointer());
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp::GetName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp::GetDescription()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())

	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp::GetChildren()
{
	FdoILongTransactionReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetChildren())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader^>(ObjectFactory::CreateILongTransactionReader(IntPtr(result), true));
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp::GetParents()
{
	FdoILongTransactionReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetParents())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader^>(ObjectFactory::CreateILongTransactionReader(IntPtr(result), true));
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp::GetOwner()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetOwner())

	return CHECK_STRING(result);
}

System::DateTime NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp::GetCreationDate()
{
	FdoDateTime result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCreationDate())

	return FdoDateTimeToDateTime(result);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp::IsActive()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->IsActive())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp::IsFrozen()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->IsFrozen())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp::ReadNext()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->ReadNext())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}
