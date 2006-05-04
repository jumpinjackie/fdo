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
	: NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoValueExpressionCollection::Create(), true))
}

NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::ValueExpressionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoValueExpressionCollection* NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::GetImpObj()
{
	return static_cast<FdoValueExpressionCollection*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
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

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::CopyTo(ValueExpression* array[], System::Int32 index)
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
		array[index+i] = __try_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression*>(get_Item(i));
	}
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Dispose(System::Boolean disposing)
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

System::Object* NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::ICollection::get_IsSynchronized()
{
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IList::get_IsFixedSize() 
{ 
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IList::get_IsReadOnly() 
{ 
	return false;
}

System::Object* NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IList::get_Item(System::Int32 index)
{
	return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IList::set_Item(System::Int32 index, System::Object* value)
{
	set_RealTypeItem(index,  __try_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression*>(value) );
}

System::Int32 NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IList::Add(System::Object* value)
{
	return Add(__try_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression*>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IList::Contains(System::Object* value)
{
	return Contains(__try_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression*>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IList::IndexOf(System::Object* value)
{
	return IndexOf(__try_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IList::Insert(System::Int32 index, System::Object* value)
{
	Insert(index,__try_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IList::Remove(System::Object* value)
{
	return Remove(__try_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

		return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Add(NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::IndexOf(NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Remove(NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Contains(NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

/*
Implementation for ArgumentDefinitionCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoValueExpression* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression*>(ObjectFactory::CreateExpression(upElement, true));
}

System::Boolean NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::get_RealTypeItem(System::Int32 index)
{
	FdoValueExpression* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression*>(ObjectFactory::CreateExpression(upElement, true));
}

System::Void  NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void  NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* value)
{
	set_RealTypeItem(index, value);
}
