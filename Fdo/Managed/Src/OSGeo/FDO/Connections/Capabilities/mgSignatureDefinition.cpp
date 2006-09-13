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
    return static_cast<FdoSignatureDefinition*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::Dispose(Boolean disposing)
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

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::SignatureDefinition(
                        NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnDataType,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection *arguments) : Disposable(IntPtr::Zero, false)
{
    EXCEPTION_HANDLER(Attach(FdoSignatureDefinition::Create(static_cast<FdoDataType>(returnDataType), 
                                                            arguments->GetImpObj()), true));
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::SignatureDefinition(
                        NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType returnPropertyType,
                        NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnDataType,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection *arguments) : Disposable(IntPtr::Zero, false)
{
    EXCEPTION_HANDLER(Attach(FdoSignatureDefinition::Create(static_cast<FdoPropertyType>(returnPropertyType), 
                                                            static_cast<FdoDataType>(returnDataType), 
                                                            arguments->GetImpObj()), true));
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::get_ReturnPropertyType()
{
	FdoPropertyType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetReturnPropertyType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(unType);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::get_ReturnType()
{
	FdoDataType unType;
	EXCEPTION_HANDLER(unType = GetImpObj()->GetReturnType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unType);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition::get_Arguments()
{
	FdoReadOnlyArgumentDefinitionCollection *unObj;
	EXCEPTION_HANDLER(unObj = GetImpObj()->GetArguments())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateReadOnlyArgumentDefinitionCollection(unObj, true);
}

// -----------------------------------------------------------------------------------------------------

FdoSignatureDefinitionCollection *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::GetImpObj()
{
    return static_cast<FdoSignatureDefinitionCollection*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::SignatureDefinitionCollection() : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoSignatureDefinitionCollection::Create(), true))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::SignatureDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::ICollection::CopyTo(System::Array *array,
                                                                                                              System::Int32 index) 
{
	if (NULL == array)
	{
		throw new System::ArgumentNullException();
	}

	if (index < 0)
	{
		throw new System::ArgumentOutOfRangeException();
	}
	if (array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
	{
		throw new System::ArgumentException();
	}

	for (System::Int32 i=0;i<this->Count;i++)
	{
		array->set_Item(index + i, get_Item(i));
	}
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::CopyTo(SignatureDefinition *array[],
                                                                                                 System::Int32 index)
{
	if (NULL == array)
	{
		throw new System::ArgumentNullException();
	}

	if (index < 0)
	{
		throw new System::ArgumentOutOfRangeException();
	}
	if (array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
	{
		throw new System::ArgumentException();
	}

	for (System::Int32 i=0;i<this->Count;i++)
	{
		array[index+i] = __try_cast<SignatureDefinition*>(get_Item(i));
	}
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Dispose(System::Boolean disposing)
{
	if (disposing)
	{

	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}

System::Object* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::ICollection::get_IsSynchronized()
{
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IList::get_IsFixedSize() 
{ 
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IList::get_IsReadOnly() 
{ 
	return false;
}

System::Object *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IList::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IList::set_Item(System::Int32 index,
                                                                                                          System::Object *value)
{
	set_RealTypeItem(index,  __try_cast<SignatureDefinition*>(value) );
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IList::Add(System::Object *value)
{
	return Add(__try_cast<SignatureDefinition*>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IList::Contains(System::Object *value)
{
	return Contains(__try_cast<SignatureDefinition*>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IList::IndexOf(System::Object *value)
{
	return IndexOf(__try_cast<SignatureDefinition*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IList::Insert(System::Int32 index,
                                                                                                        System::Object *value)
{
	Insert(index,__try_cast<SignatureDefinition*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IList::Remove(System::Object *value)
{
	return Remove(__try_cast<SignatureDefinition*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Collections::IEnumerator *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

    return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Add(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value)
{
    FdoInt32 index;

    EXCEPTION_HANDLER(index = GetImpObj()->Add((value == NULL ? NULL : value->GetImpObj())))

    return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::IndexOf(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition* value)
{
    FdoInt32 index;

    EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == NULL ? NULL : value->GetImpObj())))

    return index;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Insert(System::Int32 index,
                                                                                                 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value)
{
    EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Remove(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value)
{
    EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Contains(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

/*
Implementation for SignatureDefinitionCollection::Enumerator
*/ 
System::Object *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoSignatureDefinition *upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSignatureDefinition(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::get_RealTypeItem(System::Int32 index)
{
	FdoSignatureDefinition *upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSignatureDefinition(upElement, true);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::set_RealTypeItem(System::Int32 index,
                                                                                                           NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection::set_Item(System::Int32 index,
                                                                                                   NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value)
{
	set_RealTypeItem(index, value);
}

// -----------------------------------------------------------------------------------------------------

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::ReadOnlySignatureDefinitionCollection() : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoSignatureDefinitionCollection::Create(), true))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::ReadOnlySignatureDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoReadOnlySignatureDefinitionCollection *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::GetImpObj()
{
    return static_cast<FdoReadOnlySignatureDefinitionCollection*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::Dispose(System::Boolean disposing)
{
	if (disposing)
	{

	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::ICollection::CopyTo(System::Array *array,
                                                                                                                      System::Int32 index) 
{
	if (NULL == array)
	{
		throw new System::ArgumentNullException();
	}

	if (index < 0)
	{
		throw new System::ArgumentOutOfRangeException();
	}
	if (array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
	{
		throw new System::ArgumentException();
	}

	for (System::Int32 i=0; i<this->Count; i++)
	{
		array->set_Item(index + i, get_RealTypeItem(i));
	}
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::CopyTo(
                                                            NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *array[],
                                                            System::Int32 index)
{
	if (NULL == array)
	{
		throw new System::ArgumentNullException();
	}

	if (index < 0)
	{
		throw new System::ArgumentOutOfRangeException();
	}
	if (array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
	{
		throw new System::ArgumentException();
	}

	for (System::Int32 i=0;i<this->Count;i++)
	{
		array[index+i] = __try_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition*>(get_RealTypeItem(i));
	}
}

System::Object* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::ICollection::get_IsSynchronized()
{
	return false;
}


System::Collections::IEnumerator *NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::IndexOf(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == NULL ? NULL : value->GetImpObj())))

	return index;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::Contains(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = (!!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj())))

	return exist;
}

/*
Implementation for SignatureDefinitionCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoSignatureDefinition* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSignatureDefinition(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::get_RealTypeItem(System::Int32 index)
{
	FdoSignatureDefinition* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSignatureDefinition(upElement, true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection::get_RealTypeItem(System::String* name)
{
    return NULL;
	//FdoSignatureDefinition* upElement;

	//EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(StringToUni(name)))

	//return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSignatureDefinition(upElement, true);
}

