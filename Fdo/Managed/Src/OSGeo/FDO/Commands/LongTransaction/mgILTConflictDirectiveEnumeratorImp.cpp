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
#include <Fdo\Commands\LongTransaction\ILongTransactionConflictDirectiveEnumerator.h>

#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\mgPropertyValueCollection.h"
#include "FDO\Commands\LongTransaction\mgILTConflictDirectiveEnumeratorImp.h"

FdoILongTransactionConflictDirectiveEnumerator* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::GetImpObj()
{
    return static_cast<FdoILongTransactionConflictDirectiveEnumerator*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::Dispose(System::Boolean disposing)
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

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::get_FeatureClassName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureClassName())

	return result;
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::get_Identity()
{
	FdoPropertyValueCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIdentity())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValueCollection(result, true);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionConflictResolution NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::get_Resolution()
{
	FdoLongTransactionConflictResolution result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetResolution())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionConflictResolution>(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::set_Resolution(NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionConflictResolution value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetResolution(static_cast<FdoLongTransactionConflictResolution>(value)))
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::get_Count()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCount())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::ReadNext()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->ReadNext())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp::Reset()
{
	EXCEPTION_HANDLER(GetImpObj()->Reset())
}
