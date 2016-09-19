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
#include <Fdo\Commands\Feature\ISelect.h>
#include <Fdo\Commands\Locking\LockType.h>
#include <Fdo\Commands\Locking\LockStrategy.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Feature\mgISelectImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\Locking\mgLockType.h"
#include "FDO\Commands\Locking\mgLockStrategy.h"
#include "FDO\Commands\Locking\mgILockConflictReaderImp.h"
#include "FDO\Commands\Feature\mgIFeatureReaderImp.h"

FdoISelect* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::GetImpObj()
{
    return static_cast<FdoISelect*>(__super::UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::LockType::get()
{
	FdoLockType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockType())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::LockType::set(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLockType(static_cast<FdoLockType>(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::LockStrategy::get()
{
	FdoLockStrategy result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockStrategy())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy>(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::LockStrategy::set(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLockStrategy(static_cast<FdoLockStrategy>(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::Execute()
{
	FdoIFeatureReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->Execute())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIFeatureReader(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::ExecuteWithLock()
{
	FdoIFeatureReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->ExecuteWithLock())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIFeatureReader(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::LockConflicts::get()
{
	FdoILockConflictReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockConflicts())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILockConflictReader(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::JoinCriteria::get()
{
	FdoJoinCriteriaCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetJoinCriteria())

	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^>(ObjectFactory::CreateJoinCriteriaCollection(IntPtr(result), true));
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::Alias::get()
{
	FdoString* unstr;
    EXCEPTION_HANDLER(unstr = GetImpObj()->GetAlias())
	return CHECK_STRING(unstr);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp::Alias::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetAlias(StringToUni(value)))
}
