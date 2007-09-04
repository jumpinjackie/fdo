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
    return static_cast<FdoArgumentDefinition*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::ArgumentDefinition(System::String* name, System::String* description, NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType)  : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoArgumentDefinition::Create(StringToUni(name), StringToUni(description), static_cast<FdoDataType>(dataType)), true))
}

// For next release
//NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::ArgumentDefinition(System::String* name, System::String* description, NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType propertyType, NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType)  : Disposable(IntPtr::Zero, false)
//{
//	EXCEPTION_HANDLER(Attach(FdoArgumentDefinition::Create(StringToUni(name), StringToUni(description), static_cast<FdoPropertyType>(propertyType), static_cast<FdoDataType>(dataType)), true))
//}

System::String* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::get_Name()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetName())
	return unstr;
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::get_PropertyType()
{
	FdoPropertyType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetPropertyType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(unType);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::get_DataType()
{
	FdoDataType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unType);
}

System::String* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::get_Description()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetDescription())
	return unstr;
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::get_ArgumentValueList ()
{
    FdoPropertyValueConstraintList *result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetArgumentValueList());
    return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList *> (NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValueConstraint(result, true));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition::set_ArgumentValueList (NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList *argumentValueList)
{
	EXCEPTION_HANDLER(GetImpObj()->SetArgumentValueList(argumentValueList == NULL ? NULL : argumentValueList->GetImpObj()));
}
