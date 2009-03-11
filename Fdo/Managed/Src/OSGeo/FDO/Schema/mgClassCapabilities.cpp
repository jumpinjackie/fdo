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
#include <Fdo\Schema\ClassCapabilities.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Commands\Locking\LockType.h>
#include <Fdo\Commands\CommandType.h>

#include "mgClassCapabilities.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgClassDefinition.h"

NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities::ClassCapabilities(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities::ClassCapabilities(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ parent) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoClassCapabilities::Create(*(parent->GetImpObj()))), true))
}

FdoClassCapabilities* NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities::GetImpObj()
{
	return static_cast<FdoClassCapabilities*>(__super::UnmanagedObject.ToPointer());
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities::SupportsLocking::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->SupportsLocking())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities::SupportsLocking::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSupportsLocking(value))
}

array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities::LockTypes::get()
{
	FdoLockType* result;
	FdoInt32 len;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLockTypes(len))

	return WrapFdoLockTypeArray(result, len);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities::LockTypes::set(array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ types)
{
	FdoLockTypeArray* typeArray = UnwrapLockTypeArray(types);
    try
    {
	    EXCEPTION_HANDLER(GetImpObj()->SetLockTypes(typeArray->GetData(), typeArray->GetCount()))
    }
    finally
    {
        if (typeArray != nullptr)
	        typeArray->Release();
    }
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities::SupportsLongTransactions::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->SupportsLongTransactions())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities::SupportsLongTransactions::set(System::Boolean value )
{
	EXCEPTION_HANDLER(GetImpObj()->SetSupportsLongTransactions(value))
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities::Parent::get()
{
	FdoClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetParent())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(IntPtr(result), true);
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities::SupportsWrite::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->SupportsWrite())

	return result;
}