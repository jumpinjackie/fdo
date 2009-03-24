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
#include <Fdo.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Filter\mgValueExpressionCollection.h"

#include "FDO\mgObjectFactory.h"
#include "FDO\Expression\mgValueExpression.h"

NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::ValueExpressionCollection() 
	: NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoValueExpressionCollection::Create()), true))
}

NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::ValueExpressionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{

}

FdoValueExpressionCollection* NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::GetImpObj()
{
	return static_cast<FdoValueExpressionCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::CopyTo(array<ValueExpression^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Add(NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IndexOf(NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Remove(NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Contains(NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Item::get(System::Int32 index)
{
	FdoValueExpression* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^>(ObjectFactory::CreateExpression(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}
