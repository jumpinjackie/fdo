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
#include <Fdo\Commands\Locking\ILockedObjectReader.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Locking\mgILockedObjectReaderImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\mgPropertyValueCollection.h"

FdoILockedObjectReader* NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReaderImp::GetImpObj()
{
    return static_cast<FdoILockedObjectReader*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReaderImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReaderImp::GetFeatureClassName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureClassName())

	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReaderImp::GetIdentity()
{
	FdoPropertyValueCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIdentity())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValueCollection(IntPtr(result), true);
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReaderImp::GetLockOwner()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockOwner())

	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReaderImp::GetLockType()
{
	FdoLockType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockType())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>(result);
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReaderImp::GetLongTransaction()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLongTransaction())

	return CHECK_STRING(result);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReaderImp::ReadNext()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->ReadNext())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReaderImp::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}
