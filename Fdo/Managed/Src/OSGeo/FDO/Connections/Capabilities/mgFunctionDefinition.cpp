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
#include <Fdo\Connections\Capabilities\FunctionDefinition.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Connections\Capabilities\mgFunctionDefinition.h"
#include "FDO\Connections\Capabilities\mgArgumentDefinitionCollection.h"
#include "FDO\Connections\Capabilities\mgReadOnlyArgDefColl.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::FunctionDefinition(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoFunctionDefinition* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::GetImpObj()
{
    return static_cast<FdoFunctionDefinition*>(__super::UnmanagedObject.ToPointer());

}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::Dispose(Boolean disposing)
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

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::FunctionDefinition(System::String* name, System::String* description, NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType, NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection* arguments) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoFunctionDefinition::Create(StringToUni(name), StringToUni(description), static_cast<FdoDataType>(returnType), arguments->GetImpObj()), true))
}

// For next release
//NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::FunctionDefinition(System::String* name, System::String* description, NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType returnPropertyType, NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType, NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection* arguments) : Disposable(IntPtr::Zero, false)
//{
//	EXCEPTION_HANDLER(Attach(FdoFunctionDefinition::Create(StringToUni(name), StringToUni(description), static_cast<FdoPropertyType>(returnPropertyType), static_cast<FdoDataType>(returnType), arguments->GetImpObj()), true))
//}

System::String* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::get_Name()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetName())
	return unstr;
}

System::String* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::get_Description()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetDescription())
	return unstr;
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::get_Arguments()
{
	FdoReadOnlyArgumentDefinitionCollection * unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetArguments())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateReadOnlyArgumentDefinitionCollection(unobj, true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::get_ReturnType()
{
	FdoDataType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetReturnType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unType);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::get_ReturnPropertyType()
{
	FdoPropertyType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetReturnPropertyType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(unType);
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::get_CanSetName()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->CanSetName())
	return rv;
}
