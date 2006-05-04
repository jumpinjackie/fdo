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
#include <Fdo\Commands\Locking\ILockOwnersReader.h>

#include "FDO\Commands\Locking\mgILockOwnersReaderImp.h"

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockOwnersReaderImp::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
			Detach();
	}
}

FdoILockOwnersReader* NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockOwnersReaderImp::GetImpObj()
{
    return static_cast<FdoILockOwnersReader*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockOwnersReaderImp::GetLockOwner()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockOwner())

		return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockOwnersReaderImp::ReadNext()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->ReadNext())

		return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockOwnersReaderImp::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}
