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
#include "Rdbms\Override\RdbmsOv.h"
#include "ODBC\ODBCOvPropertyDefinitionCollection.h"

#include "FDO\Providers\Rdbms\Override\ODBC\mgOvPropertyDefinitionCollection.h"
#include "FDO\Providers\Rdbms\Override\ODBC\mgObjectFactory.h"
#include "FDO\Providers\Rdbms\Override\ODBC\mgOvPropertyDefinition.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::OvPropertyDefinitionCollection() : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoOdbcOvPropertyDefinitionCollection::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::OvPropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinitionCollection^ baseCollection) : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoOdbcOvPropertyDefinitionCollection::Create((nullptr == baseCollection ? nullptr : static_cast<FdoRdbmsOvPropertyDefinitionCollection*>(baseCollection->UnmanagedObject.ToPointer())))), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::OvPropertyDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{

}

FdoOdbcOvPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::GetImpObj()
{
	return static_cast<FdoOdbcOvPropertyDefinitionCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::CopyTo(array<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Add(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value)
{
	System::Int32 index;

    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(index = GetImpObj()->Add(dynamic_cast<FdoOdbcOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(index = GetImpObj()->Add(nullptr))
    }

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::IndexOf(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value)
{
	System::Int32 index;

    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(dynamic_cast<FdoOdbcOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(nullptr))
    }

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value)
{
    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(GetImpObj()->Insert(index, dynamic_cast<FdoOdbcOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(GetImpObj()->Insert(index, nullptr))
    }
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Remove(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value)
{
    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(GetImpObj()->Remove(dynamic_cast<FdoOdbcOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(GetImpObj()->Remove(nullptr))
    }
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Contains(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value)
{
	System::Boolean exist;

    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(dynamic_cast<FdoOdbcOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(nullptr))
    }

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Item::get(System::Int32 index)
{
	FdoOdbcOvPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::ObjectFactory::CreateIOvPropertyDefinition(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Item::get(System::String^ index)
{
	FdoOdbcOvPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(StringToUni(index)))

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::ObjectFactory::CreateIOvPropertyDefinition(IntPtr(result), true);
}

System::Void  NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value)
{
    if (nullptr != value)
    {
        System::IDisposable^ baseDisp = static_cast<System::IDisposable^>(value);
        NAMESPACE_OSGEO_RUNTIME::Disposable^ fdoDisp = static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(baseDisp);
        FdoIDisposable* ptr = static_cast<FdoIDisposable*>(fdoDisp->UnmanagedObject.ToPointer());
    	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, dynamic_cast<FdoOdbcOvPropertyDefinition*>(ptr)))
    }
    else
    {
    	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, nullptr))
    }
}
