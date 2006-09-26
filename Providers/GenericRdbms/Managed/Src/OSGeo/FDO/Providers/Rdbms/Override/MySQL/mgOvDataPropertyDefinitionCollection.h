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

class FdoMySQLOvDataPropertyDefinitionCollection;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
public __gc class PhysicalElementMapping;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

public __gc class OvDataPropertyDefinition;

/// <summary>
///     OvDataPropertyDefinitionCollection is a collection of name-value pairs.
/// </summary>
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class OvDataPropertyDefinitionCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::IList
{
private:
	/// <summary>
	///		nested class defined to provide Enumerator.
	/// </summary>
	__gc class Enumerator : public System::Collections::IEnumerator
	{
	private:
		OvDataPropertyDefinitionCollection* m_pCol;
		System::Int32 m_nIdx;

	public:
		Enumerator(OvDataPropertyDefinitionCollection* elements) : m_pCol(elements), m_nIdx(-1) 
		{

		}

		__property System::Object* get_Current();

		System::Boolean MoveNext();

		System::Void Reset();
	};

public private:
	OvDataPropertyDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoMySQLOvDataPropertyDefinitionCollection* GetImpObj();

private:
	// System::Collections::ICollection interfaces
	System::Void System::Collections::ICollection::CopyTo(System::Array* array,System::Int32 index);

	__property System::Object* System::Collections::ICollection::get_SyncRoot();

	__property System::Boolean System::Collections::ICollection::get_IsSynchronized();

	//System::Collections::IList interfaces
	__property System::Boolean System::Collections::IList::get_IsFixedSize();

	__property System::Boolean System::Collections::IList::get_IsReadOnly();

	System::Int32 System::Collections::IList::Add(Object* value);

	System::Boolean System::Collections::IList::Contains(Object* value);

	System::Int32 System::Collections::IList::IndexOf(Object* value);

	System::Void System::Collections::IList::Insert(System::Int32 index, Object* value);

	System::Void System::Collections::IList::Remove(Object* value);

	__property Object* System::Collections::IList::get_Item(System::Int32 index);

	__property System::Void  System::Collections::IList::set_Item(System::Int32 index, Object* value);

public:
	OvDataPropertyDefinitionCollection();

	OvDataPropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping* parent);

	/// <summary>
	///    Gets the count of items in collection.
	/// </summary>
	__property System::Int32 get_Count(System::Void);

	/// <summary>
	///    Gets an enumerator that can iterate through a collection.
	/// </summary>
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

	/// <summary>
	///     Removes the index-th OvDataPropertyDefinition from this collection.
	/// </summary>
	/// <param name="index">
	///		Input index of the element to remove.
	/// </param>
	System::Void RemoveAt(System::Int32 index);

	/// <summary>
	///     Removes all elements from the collection.
	/// </summary>
	System::Void  Clear();

	/// <summary>
	///     Adds a OvDataPropertyDefinition object into the collection.
	/// </summary>
	/// <param name="value">
	///		Input the OvDataPropertyDefinition object to add.
	/// </param>
	/// <returns>
	///		Returns the position into which the new element was inserted.
	/// </returns>
	System::Int32 Add(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* value);

	/// <summary>
	///     Determines the index of a specific OvDataPropertyDefinition object.
	/// </summary>
	/// <param name="value">
	///		Input the OvDataPropertyDefinition object to locate in the collection.
	/// </param>
	/// <returns>
	///		The index of value if found in the collection; otherwise, -1.
	/// </returns>
	System::Int32 IndexOf(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* value);

	/// <summary>
	///     Inserts a OvDataPropertyDefinition object into the collection at the specified position.
	/// </summary>
	/// <param name="index">
	///		Input the zero-based index at which value should be inserted. 
	/// </param>
	/// <param name="value">
	///		Input the OvDataPropertyDefinition object to insert.
	/// </param>
	System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* value);

	/// <summary>
	///     Removes the first occurrence of a specific OvDataPropertyDefinition object.
	/// </summary>
	/// <param name="value">
	///		Input the OvDataPropertyDefinition object to remove from the collection.
	/// </param>
	System::Void Remove(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* value);

	/// <summary>
	///     Determines whether the collection contains a specific OvDataPropertyDefinition object.
	/// </summary>
	/// <param name="value">
	///		Input The OvDataPropertyDefinition object to search in the collection.
	/// </param>
	/// <returns>
	///		Returns true if the value is found in the collection; otherwise, false.
	/// </returns>
	System::Boolean Contains(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* value);

	/// <summary>
	///     Copies the elements of the collection to an array.
	/// </summary>
	/// <param name="array">
	///		Output the one-dimensional Array that is the destination of the elements copied from this collection. 
	/// </param>
	/// <param name="startAt">
	///		Input an integer that represents the index in array at which copying begins. 
	/// </param>
	System::Void CopyTo(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* array[],System::Int32 size);

	//this is a hedious workaround for a bug in VS2002. 
	/// <summary>
	///     Gets or sets a OvDataPropertyDefinition in the collection.
	/// </summary>
	/// <param name="index">
	///		Input index of the OvDataPropertyDefinition to retrieve or set (System::Int32). 
	/// </param>
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* get_RealTypeItem(System::Int32 index);
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* get_RealTypeItem(System::String* index);

	__property System::Void  set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* value);

	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* get_Item(System::Int32 index);

	__property System::Void  set_Item(System::Int32 index, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* value);

protected:
	System::Void ReleaseUnmanagedObject();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

