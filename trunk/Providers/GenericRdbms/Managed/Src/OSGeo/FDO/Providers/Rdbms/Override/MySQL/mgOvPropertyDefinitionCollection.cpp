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
#include <RdbmsOv.h>
#include <MySQLOv.h>
#include <RdbmsOvPropertyDefinitionCollection.h>
#include <MySQLOvPropertyDefinition.h>
#include <MySQLOvPropertyDefinitionCollection.h>

#include "FDO\Providers\Rdbms\Override\MySQL\mgIOvPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvPropertyDefinitionCollection.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgObjectFactory.h"

FdoMySQLOvPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::GetImpObj()
{
    return static_cast<FdoMySQLOvPropertyDefinitionCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::OvPropertyDefinitionCollection() : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{	
	EXCEPTION_HANDLER(Attach(IntPtr(FdoMySQLOvPropertyDefinitionCollection::Create()), true))	
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::OvPropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinitionCollection^ baseCollection) : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoMySQLOvPropertyDefinitionCollection::Create(static_cast<FdoRdbmsOvPropertyDefinitionCollection*>(baseCollection->UnmanagedObject.ToPointer()))), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::OvPropertyDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{

}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::CopyTo(array<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Add(IOvPropertyDefinition^ value)
{
	System::Int32 index;

    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(index = GetImpObj()->Add(dynamic_cast<FdoMySQLOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(index = GetImpObj()->Add(nullptr))
    }

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::IndexOf(IOvPropertyDefinition^ value)
{
	System::Int32 index;

    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(dynamic_cast<FdoMySQLOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(nullptr))
    }

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Insert(System::Int32 index, IOvPropertyDefinition^ value)
{
    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(GetImpObj()->Insert(index, dynamic_cast<FdoMySQLOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(GetImpObj()->Insert(index, nullptr))
    }
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Remove(IOvPropertyDefinition^ value)
{
    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(GetImpObj()->Remove(dynamic_cast<FdoMySQLOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(GetImpObj()->Remove(nullptr))
    }
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Contains(IOvPropertyDefinition^ value)
{
	System::Boolean exist;

    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(dynamic_cast<FdoMySQLOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(nullptr))
    }

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Item::get(System::Int32 index)
{
	FdoMySQLOvPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::ObjectFactory::CreateIOvPropertyDefinition(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Item::get(System::String^ index)
{
	FdoMySQLOvPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(StringToUni(index)))

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::ObjectFactory::CreateIOvPropertyDefinition(IntPtr(result), true);
}

System::Void  NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition^ value)
{
    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, dynamic_cast<FdoMySQLOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, nullptr))
    }
}
