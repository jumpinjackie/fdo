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
#include <Fdo\Connections\Capabilities\ArgumentDefinition.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Connections\Capabilities\mgArgumentDefinition.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::ArgumentDefinition(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoArgumentDefinition* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::GetImpObj()
{
    return static_cast<FdoArgumentDefinition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::ArgumentDefinition(System::String^ name, System::String^ description, NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType)  : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoArgumentDefinition::Create(StringToUni(name), StringToUni(description), static_cast<FdoDataType>(dataType))), true))
}


NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::ArgumentDefinition(System::String^ name, System::String^ description, NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType propertyType, NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType)  : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoArgumentDefinition::Create(StringToUni(name), StringToUni(description), static_cast<FdoPropertyType>(propertyType), static_cast<FdoDataType>(dataType))), true))
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::Name::get()
{
	FdoString* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetName())
	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::PropertyType::get()
{
	FdoPropertyType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetPropertyType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(unType);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::DataType::get()
{
	FdoDataType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unType);
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::Description::get()
{
	FdoString* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())
	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::ArgumentValueList::get()
{
    FdoPropertyValueConstraintList* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetArgumentValueList());
    return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList^> (NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValueConstraint(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::ArgumentValueList::set(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList ^argumentValueList)
{
	EXCEPTION_HANDLER(GetImpObj()->SetArgumentValueList(argumentValueList == nullptr ? nullptr : argumentValueList->GetImpObj()));
}
