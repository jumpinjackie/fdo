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
#include <Fdo\Commands\Locking\IGetLockedObjects.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Locking\mgIGetLockedObjectsImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\Locking\mgILockedObjectReaderImp.h"

FdoIGetLockedObjects* NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IGetLockedObjectsImp::GetImpObj()
{
    return static_cast<FdoIGetLockedObjects*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IGetLockedObjectsImp::get_LockOwner()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockOwner())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IGetLockedObjectsImp::set_LockOwner(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLockOwner(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReader* NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IGetLockedObjectsImp::Execute()
{
	FdoILockedObjectReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->Execute())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILockedObjectReader(result, true);
}
