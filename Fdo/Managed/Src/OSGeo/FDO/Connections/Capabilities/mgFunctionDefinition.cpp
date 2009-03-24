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
#include "FDO\Connections\Capabilities\mgSignatureDefinition.h"
#include "FDO\Connections\Capabilities\mgReadOnlyArgDefColl.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::FunctionDefinition(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoFunctionDefinition* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::GetImpObj()
{
    return static_cast<FdoFunctionDefinition*>(UnmanagedObject.ToPointer());

}

IntPtr NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::FunctionDefinition(
                                    System::String^ name,
                                    System::String^ description,
                                    NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType,
                                    NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection^ arguments) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFunctionDefinition::Create(StringToUni(name),
                                                           StringToUni(description),
                                                           static_cast<FdoDataType>(returnType),
                                                           arguments->GetImpObj())), true))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::FunctionDefinition(
                                    System::String^ name,
                                    System::String^ description,
                                    NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType,
                                    NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection^ arguments,
                                    NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionCategoryType functionCategoryType) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFunctionDefinition::Create(StringToUni(name),
                                                           StringToUni(description),
                                                           static_cast<FdoDataType>(returnType),
                                                           arguments->GetImpObj(),
                                                           static_cast<FdoFunctionCategoryType>(functionCategoryType))), true))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::FunctionDefinition (
                                    System::String^ name,
                                    System::String^ description,
                                    NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType returnPropertyType,
                                    NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType,
                                    NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection^ arguments) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFunctionDefinition::Create(StringToUni(name),
                                                           StringToUni(description),
                                                           static_cast<FdoPropertyType>(returnPropertyType),
                                                           static_cast<FdoDataType>(returnType),
                                                           arguments->GetImpObj())), true))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::FunctionDefinition (
                                    System::String^ name,
                                    System::String^ description,
                                    NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType returnPropertyType,
                                    NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType,
                                    NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection^ arguments,
                                    NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionCategoryType functionCategoryType) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFunctionDefinition::Create(StringToUni(name),
                                                           StringToUni(description),
                                                           static_cast<FdoPropertyType>(returnPropertyType),
                                                           static_cast<FdoDataType>(returnType),
                                                           arguments->GetImpObj(),
                                                           static_cast<FdoFunctionCategoryType>(functionCategoryType))), true))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::FunctionDefinition(
                                    System::String^ name,
                                    System::String^ description,
                                    System::Boolean isAggregate,
                                    NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection^ signatures) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFunctionDefinition::Create(StringToUni(name),
                                                           StringToUni(description),
                                                           isAggregate,
                                                           signatures->GetImpObj())), true))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::FunctionDefinition(
                                    System::String^ name,
                                    System::String^ description,
                                    System::Boolean isAggregate,
                                    NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection^ signatures,
                                    NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionCategoryType functionCategoryType) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFunctionDefinition::Create(StringToUni(name),
                                                           StringToUni(description),
                                                           isAggregate,
                                                           signatures->GetImpObj(),
                                                           static_cast<FdoFunctionCategoryType>(functionCategoryType))), true))
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::Name::get()
{
	FdoString* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetName())
	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::Description::get()
{
	FdoString* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())
	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::Arguments::get()
{
	FdoReadOnlyArgumentDefinitionCollection* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetArguments())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateReadOnlyArgumentDefinitionCollection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::ReturnType::get()
{
	FdoDataType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetReturnType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unType);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::ReturnPropertyType::get()
{
	FdoPropertyType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetReturnPropertyType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(unType);
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::CanSetName::get()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->CanSetName())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::IsAggregate::get()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->IsAggregate());
	return rv;
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::Signatures::get()
{
    FdoReadOnlySignatureDefinitionCollection* result;
    EXCEPTION_HANDLER(result = GetImpObj()->GetSignatures());
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateReadOnlySignatureDefinitionCollection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionCategoryType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition::FunctionCategoryType::get()
{
	FdoFunctionCategoryType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetFunctionCategoryType())
	return static_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionCategoryType>(unType);
}
