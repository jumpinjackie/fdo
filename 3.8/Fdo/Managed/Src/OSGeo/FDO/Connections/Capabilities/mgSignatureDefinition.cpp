// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include "stdafx.h"
#include <Fdo\Connections\Capabilities\SignatureDefinition.h>
#include "FDO\Connections\Capabilities\mgSignatureDefinition.h"
#include "Fdo\Commands\CommandType.h"
#include "FDO\Commands\mgCommandType.h"
#include "FDO\mgObjectFactory.h"


NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::SignatureDefinition(IntPtr unmanaged,
                                                                                       Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoSignatureDefinition* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::GetImpObj()
{
    return static_cast<FdoSignatureDefinition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::SignatureDefinition(
                        NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnDataType,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection^ arguments) : Disposable(IntPtr::Zero, false)
{
    EXCEPTION_HANDLER(Attach(IntPtr(FdoSignatureDefinition::Create(static_cast<FdoDataType>(returnDataType), 
                                                            arguments->GetImpObj())), true));
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::SignatureDefinition(
                        NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType returnPropertyType,
                        NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnDataType,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection^ arguments) : Disposable(IntPtr::Zero, false)
{
    EXCEPTION_HANDLER(Attach(IntPtr(FdoSignatureDefinition::Create(static_cast<FdoPropertyType>(returnPropertyType), 
                                                            static_cast<FdoDataType>(returnDataType), 
                                                            arguments->GetImpObj())), true));
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::ReturnPropertyType::get()
{
	FdoPropertyType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetReturnPropertyType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(unType);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::ReturnType::get()
{
	FdoDataType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetReturnType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unType);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::Arguments::get()
{
	FdoReadOnlyArgumentDefinitionCollection *result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetArguments())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateReadOnlyArgumentDefinitionCollection(IntPtr(result), true);
}

// -----------------------------------------------------------------------------------------------------

FdoSignatureDefinitionCollection *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::GetImpObj()
{
    return static_cast<FdoSignatureDefinitionCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::SignatureDefinitionCollection() : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSignatureDefinitionCollection::Create()), true))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::SignatureDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{

}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::CopyTo(array<SignatureDefinition^>^ pArray, System::Int32 index)
{
	if (nullptr == pArray)
		throw gcnew System::ArgumentNullException();
	if (index < 0)
		throw gcnew System::ArgumentOutOfRangeException();
	if (pArray->Rank != 1 || index >= pArray->Length || this->Count + index > pArray->Length)
		throw gcnew System::ArgumentException();

	for (System::Int32 i = 0; i < this->Count; i++)
        pArray[index+i] = this->Item[i];
}

System::Object^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

    return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Add(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ value)
{
    System::Int32 index;

    EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

    return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IndexOf(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ value)
{
    System::Int32 index;

    EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

    return index;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Insert(System::Int32 index,
                                                                                                 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ value)
{
    EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Remove(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ value)
{
    EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Contains(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Item::get(System::Int32 index)
{
	FdoSignatureDefinition *result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSignatureDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

// -----------------------------------------------------------------------------------------------------

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::ReadOnlySignatureDefinitionCollection() : NAMESPACE_OSGEO_COMMON::CollectionReadOnlyBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSignatureDefinitionCollection::Create()), true))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::ReadOnlySignatureDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionReadOnlyBase(unmanaged, autoDelete)
{

}

FdoReadOnlySignatureDefinitionCollection *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::GetImpObj()
{
    return static_cast<FdoReadOnlySignatureDefinitionCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::CopyTo(array<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^>^ pArray, System::Int32 index)
{
	if (nullptr == pArray)
		throw gcnew System::ArgumentNullException();
	if (index < 0)
		throw gcnew System::ArgumentOutOfRangeException();
	if (pArray->Rank != 1 || index >= pArray->Length || this->Count + index > pArray->Length)
		throw gcnew System::ArgumentException();

	for (System::Int32 i = 0; i < this->Count; i++)
        pArray[index+i] = this->Item[i];
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::IndexOf(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::Contains(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = (!!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj())))

	return exist;
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::Item::get(System::String^ name)
{
    return nullptr;
	//FdoSignatureDefinition* upElement;

	//EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(StringToUni(name)))

	//return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSignatureDefinition(upElement, true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::Item::get(System::Int32 index)
{
	FdoSignatureDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSignatureDefinition(IntPtr(result), true);
}

System::Object^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}
