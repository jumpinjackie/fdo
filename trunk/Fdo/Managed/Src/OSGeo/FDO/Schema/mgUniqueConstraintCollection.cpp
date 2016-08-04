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
#include <Fdo\Schema\UniqueConstraintCollection.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\ClassType.h>

#include "FDO\Schema\mgUniqueConstraint.h"
#include "FDO\Schema\mgUniqueConstraintCollection.h"
#include "FDO\mgObjectFactory.h"

FdoUniqueConstraintCollection* NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::GetImpObj()
{
	return static_cast<FdoUniqueConstraintCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::CopyTo(array<UniqueConstraint^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Add(UniqueConstraint^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::IndexOf(UniqueConstraint^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Insert(System::Int32 index, UniqueConstraint^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Remove(UniqueConstraint^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Contains(UniqueConstraint^ value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint^ NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Item::get(System::Int32 index)
{
	FdoUniqueConstraint* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateUniqueConstraint(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}
