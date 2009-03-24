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
#include <Fdo\Commands\LongTransaction\ILongTransactionPrivilegeReader.h>
#include "FDO\Commands\LongTransaction\mgILTPrivilegeReaderImp.h"

FdoILongTransactionPrivilegeReader* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionPrivilegeReaderImp::GetImpObj()
{
    return static_cast<FdoILongTransactionPrivilegeReader*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionPrivilegeReaderImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionPrivilegeReaderImp::GetUsername()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetUserName())

	return CHECK_STRING(result);
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionPrivilegeReaderImp::GetPrivileges()
{
	System::Int32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPrivileges())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionPrivilegeReaderImp::ReadNext()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->ReadNext())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionPrivilegeReaderImp::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}
