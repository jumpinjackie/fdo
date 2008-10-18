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
#include <Fdo\Commands\Locking\IAcquireLock.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Locking\mgIAcquireLockImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\Locking\mgILockConflictReaderImp.h"

FdoIAcquireLock* NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IAcquireLockImp::GetImpObj()
{
    return static_cast<FdoIAcquireLock*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IAcquireLockImp::get_LockType()
{
	FdoLockType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockType())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IAcquireLockImp::set_LockType(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLockType(static_cast<FdoLockType>(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IAcquireLockImp::get_LockStrategy()
{
	FdoLockStrategy result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockStrategy())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy>(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IAcquireLockImp::set_LockStrategy(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLockStrategy(static_cast<FdoLockStrategy>(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader* NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IAcquireLockImp::Execute()
{
	FdoILockConflictReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->Execute())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILockConflictReader(result, true);
}
