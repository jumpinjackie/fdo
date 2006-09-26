/*
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

#pragma once

class FdoClassCollection;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class ClassDefinition;
public __gc class SchemaElement;

/// \brief
/// The FdoClassCollection class represents a collection of FdoClassDefinition objects.
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class ClassCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::IList
{
private:
    /// \brief
    /// 		nested class defined to provide Enumerator.
    /// 
	__gc class Enumerator : public System::Collections::IEnumerator
	{
	private:
		ClassCollection* m_pCol;
		System::Int32 m_nIdx;

	public:
		Enumerator(ClassCollection* elements) : m_pCol(elements), m_nIdx(-1) 
		{

		}

		__property System::Object* get_Current();

		System::Boolean MoveNext();

		System::Void Reset();
	};

public private:
	inline FdoClassCollection* GetImpObj();

private:
    // System::Collections::ICollection interfaces
	System::Void System::Collections::ICollection::CopyTo(System::Array* array,System::Int32 index);

	__property System::Object* System::Collections::ICollection::get_SyncRoot();

	__property System::Boolean System::Collections::ICollection::get_IsSynchronized();

    // System::Collections::IList interfaces
	__property System::Boolean System::Collections::IList::get_IsFixedSize();

	__property System::Boolean System::Collections::IList::get_IsReadOnly();

	System::Int32 System::Collections::IList::Add(Object* value);

	System::Boolean System::Collections::IList::Contains(Object* value);

	System::Int32 System::Collections::IList::IndexOf(Object* value);

	System::Void System::Collections::IList::Insert(System::Int32 index, Object* value);

	System::Void System::Collections::IList::Remove(Object* value);

	__property Object* System::Collections::IList::get_Item(System::Int32 index);

	__property System::Void  System::Collections::IList::set_Item(System::Int32 index, Object* value);

protected:
	System::Void ReleaseUnmanagedObject();

public:
	ClassCollection(NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement* parent);

	ClassCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

    /// \brief
    ///    Gets the count of items in collection.
    /// 
	__property System::Int32 get_Count(System::Void);

    /// \brief
    ///    Gets an enumerator that can iterate through a collection.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

    /// \brief
    ///     Removes the index-th ClassDefinition from this collection.
    /// 
    /// \param index 
    /// Input index of the element to remove.
    /// 
	System::Void RemoveAt(System::Int32 index);

    /// \brief
    ///     Removes all elements from the collection.
    /// 
	System::Void  Clear();

    /// \brief
    ///     Adds a ClassDefinition object into the collection.
    /// 
    /// \param value 
    /// Input the ClassDefinition object to add.
    /// 
    /// \return
    /// 		Returns the position into which the new element was inserted.
    /// 
	System::Int32 Add(ClassDefinition* value);

    /// \brief
    ///     Determines the index of a specific ClassDefinition object.
    /// 
    /// \param value 
    /// Input the ClassDefinition object to locate in the collection.
    /// 
    /// \return
    /// 		The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(ClassDefinition* value);

	System::Int32 IndexOf(String* name);

    /// \brief
    ///     Inserts a ClassDefinition object into the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the ClassDefinition object to insert.
    /// 
	System::Void Insert(System::Int32 index, ClassDefinition* value);

    /// \brief
    ///     Removes the first occurrence of a specific ClassDefinition object.
    /// 
    /// \param value 
    /// Input the ClassDefinition object to remove from the collection.
    /// 
	System::Void Remove(ClassDefinition* value);

    /// \brief
    ///     Determines whether the collection contains a specific ClassDefinition object.
    /// 
    /// \param value 
    /// Input The ClassDefinition object to search in the collection.
    /// 
    /// \return
    /// 		Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(ClassDefinition* value);

	System::Boolean Contains(String* name);

    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(ClassDefinition* array[],System::Int32 size);


    /// \brief
    ///     Gets or sets a ClassDefinition in the collection.
    /// 
    /// \param index 
    /// Input index of the ClassDefinition to retrieve or set (System::Int32).
    /// 
	__property ClassDefinition* get_RealTypeItem(System::Int32 index);

	__property ClassDefinition* get_RealTypeItem(System::String* index);

	__property System::Void  set_RealTypeItem(System::Int32 index, ClassDefinition* value);

	__property ClassDefinition* get_Item(System::Int32 index);

	__property System::Void  set_Item(System::Int32 index, ClassDefinition* value);
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


