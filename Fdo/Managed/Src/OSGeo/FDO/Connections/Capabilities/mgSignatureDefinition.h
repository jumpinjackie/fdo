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

#include "FDO\Schema\mgDataType.h"
#include "FDO\Schema\mgPropertyType.h"
#include "FDO\Connections\Capabilities\mgArgumentDefinitionCollection.h"
#include "FDO\Connections\Capabilities\mgReadOnlyArgDefColl.h"

class FdoSignatureDefinition;
class FdoReadOnlySignatureDefinitionCollection;

using namespace NAMESPACE_OSGEO_FDO_SCHEMA;
BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

public __gc class SignatureDefinition : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:

    /// \brief
    /// Constructs an instance of a FunctionDefinition using the specified arguments.
    SignatureDefinition (NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnDataType,
                         NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection *arguments);

    /// \brief
    /// Constructs an instance of a FunctionDefinition using the specified arguments.
    SignatureDefinition (NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType returnPropertyType,
                         NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnDataType,
                         NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection *arguments);

    /// \brief
    /// Gets an array of FdoArgumentDefinition objects required for the function. 
    __property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection *get_Arguments();

    /// \brief
    /// Gets the FdoPropertyType of the function return value.
    __property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType get_ReturnPropertyType();

    /// \brief
    /// Gets the FdoDataType of the function return value. This is only valid if the function return property type is
    /// FdoPropertyType_DataProperty.
    __property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_ReturnType();


protected:
	System::Void Dispose(System::Boolean disposing);

public private:
	SignatureDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSignatureDefinition* GetImpObj();
};

// -----------------------------------------------------------------------------------------------------


[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class SignatureDefinitionCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::IList
{
private:
    /// \brief
    /// 		nested class defined to provide Enumerator.
    /// 
	__gc class Enumerator : public System::Collections::IEnumerator
	{
	private:
		SignatureDefinitionCollection *m_pCol;
		System::Int32 m_nIdx;

	public:
		Enumerator(SignatureDefinitionCollection* elements) : m_pCol(elements), m_nIdx(-1) 
		{

		}

		__property System::Object* get_Current();

		System::Boolean MoveNext();

		System::Void Reset();
	};

public private:
	SignatureDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSignatureDefinitionCollection *GetImpObj();

private:
    /// System::Collections::ICollection interfaces
	System::Void System::Collections::ICollection::CopyTo(System::Array *array, System::Int32 index);

	__property System::Object *System::Collections::ICollection::get_SyncRoot();

	__property System::Boolean System::Collections::ICollection::get_IsSynchronized();

    // System::Collections::IList interfaces
	__property System::Boolean System::Collections::IList::get_IsFixedSize();

	__property System::Boolean System::Collections::IList::get_IsReadOnly();

	System::Int32 System::Collections::IList::Add(Object *value);

	System::Boolean System::Collections::IList::Contains(Object *value);

	System::Int32 System::Collections::IList::IndexOf(Object *value);

	System::Void System::Collections::IList::Insert(System::Int32 index, Object *value);

	System::Void System::Collections::IList::Remove(Object *value);

	__property Object *System::Collections::IList::get_Item(System::Int32 index);

	__property System::Void System::Collections::IList::set_Item(System::Int32 index, Object *value);

public:
	SignatureDefinitionCollection();

    /// \brief
    ///    Gets the count of items in collection.
    /// 
	__property System::Int32 get_Count(System::Void);

    /// \brief
    ///    Gets an enumerator that can iterate through a collection.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

    /// \brief
    ///     Removes the index-th SignatureDefinition from this collection.
    /// 
    /// \param index 
    ///     Input index of the element to remove.
    /// 
	System::Void RemoveAt(System::Int32 index);

    /// \brief
    ///     Removes all elements from the collection.
    /// 
	System::Void Clear();

    /// \brief
    ///     Adds a SignatureDefinition object into the collection.
    /// 
    /// \param value 
    ///     Input the SignatureDefinition object to add.
    /// 
    /// \return
    ///     Returns the position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    ///     Determines the index of a specific SignatureDefinition object.
    /// 
    /// \param value 
    ///     Input the SignatureDefinition object to locate in the collection.
    /// 
    /// \return
    /// 		The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    ///     Inserts a SignatureDefinition object into the collection at the specified position.
    /// 
    /// \param index 
    ///     Input the zero-based index at which value should be inserted.
    /// \param value 
    ///     Input the SignatureDefinition object to insert.
    /// 
	System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    ///     Removes the first occurrence of a specific SignatureDefinition object.
    /// 
    /// \param value 
    ///     Input the SignatureDefinition object to remove from the collection.
    /// 
	System::Void Remove(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    ///     Determines whether the collection contains a specific SignatureDefinition object.
    /// 
    /// \param value 
    ///     Input The SignatureDefinition object to search in the collection.
    /// 
    /// \return
    /// 		Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    ///     Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    ///     Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *array[], System::Int32 size);


    /// \brief
    ///     Gets or sets a SignatureDefinition in the collection.
    /// 
    /// \param index 
    ///     Input index of the SignatureDefinition to retrieve or set (System::Int32).
    /// 
	__property SignatureDefinition *get_RealTypeItem(System::Int32 index);

	__property System::Void set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

	__property SignatureDefinition *get_Item(System::Int32 index);

	__property System::Void set_Item(System::Int32 index, NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

protected:
	System::Void Dispose(System::Boolean disposing);
};

// -----------------------------------------------------------------------------------------------------

[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class ReadOnlySignatureDefinitionCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable,
                                                                   public System::Collections::ICollection,
                                                                   public System::Collections::IEnumerable
{
private:
    /// \brief
    /// 		nested class defined to provide Enumerator.
    /// 
	__gc class Enumerator : public System::Collections::IEnumerator
	{
	private:
		ReadOnlySignatureDefinitionCollection *m_pCol;
		System::Int32 m_nIdx;

	public:
		Enumerator(ReadOnlySignatureDefinitionCollection *elements) : m_pCol(elements), m_nIdx(-1) 
		{

		}

		__property System::Object *get_Current();

		System::Boolean MoveNext();

		System::Void Reset();
	};

public private:
	ReadOnlySignatureDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoReadOnlySignatureDefinitionCollection *GetImpObj();

private:
    // System::Collections::ICollection interfaces
	System::Void System::Collections::ICollection::CopyTo(System::Array *array, System::Int32 index);
	__property System::Object *System::Collections::ICollection::get_SyncRoot();
	__property System::Boolean System::Collections::ICollection::get_IsSynchronized();


public:
	ReadOnlySignatureDefinitionCollection();

    /// \brief
    ///    Gets the count of items in collection.
    /// 
	__property System::Int32 get_Count(System::Void);	

    /// \brief
    ///    Gets an enumerator that can iterate through a collection.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    ///     Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    ///     Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *array[],
                        System::Int32 index);

    /// \brief
    ///     Determines the index of a specific SignatureDefinition object.
    /// 
    /// \param value 
    ///     Input the SignatureDefinition object to locate in the collection.
    /// 
    /// \return
    /// 	The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    ///     Determines whether the collection contains a specific SignatureDefinition object.
    /// 
    /// \param value 
    ///     Input The SignatureDefinition object to search in the collection.
    /// 
    /// \return
    /// 	Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    ///     Gets or sets a SignatureDefinition in the collection.
    /// 
    /// \param index 
    ///     Input index of the SignatureDefinition to retrieve or set (System::Int32).
    /// 
	__property SignatureDefinition *get_RealTypeItem(System::Int32 index);
	__property SignatureDefinition *get_RealTypeItem(System::String* name);

protected:
	System::Void Dispose(System::Boolean disposing);

};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

