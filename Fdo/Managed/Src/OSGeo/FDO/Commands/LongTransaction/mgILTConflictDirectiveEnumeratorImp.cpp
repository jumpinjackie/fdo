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
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Commands\PropertyValueCollection.h>
#include <Fdo\Commands\LongTransaction\ILTConflictDirectiveEnumerator.h>

#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\mgPropertyValueCollection.h"
#include "FDO\Commands\LongTransaction\mgILTConflictDirectiveEnumeratorImp.h"

FdoILongTransactionConflictDirectiveEnumerator* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::GetImpObj()
{
    return static_cast<FdoILongTransactionConflictDirectiveEnumerator*>(__super::UnmanagedObject.ToPointer());
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::FeatureClassName::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureClassName())

	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::Identities::get()
{
	FdoPropertyValueCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIdentity())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValueCollection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionConflictResolution NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::Resolution::get()
{
	FdoLongTransactionConflictResolution result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetResolution())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionConflictResolution>(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::Resolution::set(NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionConflictResolution value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetResolution(static_cast<FdoLongTransactionConflictResolution>(value)))
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::Count::get()
{
	System::Int32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCount())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::ReadNext()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->ReadNext())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::Reset()
{
	EXCEPTION_HANDLER(GetImpObj()->Reset())
}
