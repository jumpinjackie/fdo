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
#include <Fdo\Commands\Feature\IUpdate.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Feature\mgIUpdateImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\mgPropertyValueCollection.h"
#include "FDO\Commands\Locking\mgILockConflictReaderImp.h"
 
FdoIUpdate* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IUpdateImp::GetImpObj()
{
    return static_cast<FdoIUpdate*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IUpdateImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IUpdateImp::PropertyValues::get()
{
	FdoPropertyValueCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyValues())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValueCollection(IntPtr(result), true);
}
 
System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IUpdateImp::Execute()
{
	System::Int32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->Execute())

	return result;
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IUpdateImp::LockConflicts::get()
{
	FdoILockConflictReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockConflicts())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILockConflictReader(IntPtr(result), true);
}
