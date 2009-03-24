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
// The following header files sequence is very important
// The sequence can not be changed
#include <Fdo\Commands\Locking\ILockConflictReader.h>
#include <Fdo\Commands\Locking\IReleaseLock.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Locking\mgIReleaseLockImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\Locking\mgILockConflictReaderImp.h"

FdoIReleaseLock* NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IReleaseLockImp::GetImpObj()
{
    return static_cast<FdoIReleaseLock*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IReleaseLockImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IReleaseLockImp::LockOwner::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockOwner())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IReleaseLockImp::LockOwner::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLockOwner(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader^ NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IReleaseLockImp::Execute()
{
	FdoILockConflictReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->Execute())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILockConflictReader(IntPtr(result), true);
}
