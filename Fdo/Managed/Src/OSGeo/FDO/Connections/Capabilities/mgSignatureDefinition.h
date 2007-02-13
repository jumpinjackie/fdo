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

/// \brief
/// The FDO SignatureDefinition class allows the signatures of FDO FunctionDefinition objects to be defined.
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
    /// Gets an array of ArgumentDefinition objects required for the function. 
    __property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection *get_Arguments();

    /// \brief
    /// Gets the PropertyType of the function return value.
    __property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType get_ReturnPropertyType();

    /// \brief
    /// Gets the DataType of the function return value. This is only valid if the function return property type is
    /// PropertyType_DataProperty.
    __property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_ReturnType();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
	SignatureDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSignatureDefinition* GetImpObj();
};

// -----------------------------------------------------------------------------------------------------


/// \brief
/// SignatureDefinitionCollection is a collection of SignatureDefinition elements.
///
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class SignatureDefinitionCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::IList
{
/// \cond DOXYGEN-IGNORE
private:
    /// \brief
    /// A Nested class defined to provide enumeration of Dictionary elements
    ///
    /// Enumerators can be used to read the data in the collection, 
    /// but they cannot be used to modify the underlying collection.
    ///
    /// An enumerator remains valid as long as the collection remains unchanged. 
    /// If changes are made to the collection, such as adding, modifying, or deleting 
    /// elements, the enumerator is irrecoverably invalidated and the next call to 
    /// MoveNext or Reset throws an InvalidOperationException. If the collection is 
    /// modified between MoveNext and Current, Current returns the element that it is 
    /// set to, even if the enumerator is already invalidated.
    ///
    /// The enumerator does not have exclusive access to the collection; therefore, 
    /// enumerating through a collection is intrinsically not a thread-safe procedure. 
    /// Even when a collection is synchronized, other threads can still modify the 
    /// collection, which causes the enumerator to throw an exception. To guarantee 
    /// thread safety during enumeration, you can either lock the collection during 
    /// the entire enumeration or catch the exceptions resulting from changes made 
    /// by other threads.
    /// 
	__gc class Enumerator : public System::Collections::IEnumerator
	{
	private:
		SignatureDefinitionCollection *m_pCol;
		System::Int32 m_nIdx;

	public:
        /// \brief
        /// Constructs a new Collection Enumerator
        /// 
        /// \param col 
        /// Input The collection to enumerate.
        /// 
		Enumerator(SignatureDefinitionCollection* elements) : m_pCol(elements), m_nIdx(-1) 
		{

		}

        /// \brief
        /// Retrieves the current object at the enumerator location
        /// 
        /// \return
        /// Retuns the current object referenced by the enumerator
        /// 
		__property System::Object *get_Current();

        /// \brief
        /// Initially, the enumerator is positioned before the first object in the collection. 
        /// At this position, calling the Current property throws an exception. 
        /// Therefore, you must call the MoveNext method to advance the enumerator 
        /// to the first element of the collection before reading the value of Current.
        /// If MoveNext passes the end of the collection, the enumerator is positioned 
        /// after the last element in the collection and MoveNext returns false. 
        /// When the enumerator is at this position, subsequent calls to MoveNext also return false. 
        /// If the last call to MoveNext returned false, calling Current throws an exception. 
        /// To set Current to the first element of the collection again, you can call Reset 
        /// followed by MoveNext.
        /// 
        /// \return
        /// Retuns true if the Enumerator is able to move to a valid element
        /// otherwise false.
        /// 
		System::Boolean MoveNext();

        /// \brief
        /// Initially, the enumerator is positioned before the first element in the collection. 
        /// The Reset method brings the enumerator back to this position. 
        /// 
		System::Void Reset();
	};

public private:
	SignatureDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSignatureDefinitionCollection *GetImpObj();

private:
    // System::Collections::ICollection interface properties
    __property System::Object* System::Collections::ICollection::get_SyncRoot();
    __property System::Boolean System::Collections::ICollection::get_IsSynchronized();

    // System::Collections::ICollection interface methods
    System::Void System::Collections::ICollection::CopyTo(System::Array* array,System::Int32 index);

    // System::Collections::IList interface properties
    __property System::Boolean System::Collections::IList::get_IsFixedSize();
    __property System::Boolean System::Collections::IList::get_IsReadOnly();
    __property Object* System::Collections::IList::get_Item(System::Int32 index);
    __property System::Void  System::Collections::IList::set_Item(System::Int32 index, Object* value);

    // System::Collections::IList interface methods
    System::Int32 System::Collections::IList::Add(Object* value);
    System::Boolean System::Collections::IList::Contains(Object* value);
    System::Int32 System::Collections::IList::IndexOf(Object* value);
    System::Void System::Collections::IList::Insert(System::Int32 index, Object* value);
    System::Void System::Collections::IList::Remove(Object* value);
/// \endcond

public:
	SignatureDefinitionCollection();

    /// \brief
    /// Gets the count of items in collection.
    /// 
    /// \return
    /// Returns the number of items in the collection.
    /// 
	__property System::Int32 get_Count(System::Void);

    /// \brief
    /// Gets an enumerator that can iterate through a collection.
    /// 
    /// \return
    /// Returns an enumerator on the dictionary.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

    /// \brief
    /// Removes the index-th SignatureDefinition from this collection.
    /// 
    /// \param index 
    /// Input index of the element to remove.
    /// 
	System::Void RemoveAt(System::Int32 index);

    /// \brief
    /// Removes all elements from the collection.
    /// 
	System::Void Clear();

    /// \brief
    /// Adds a SignatureDefinition object into the collection.
    /// 
    /// \param value 
    /// Input the SignatureDefinition object to add.
    /// 
    /// \return
    /// Returns the position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    /// Determines the index of a specific SignatureDefinition object.
    /// 
    /// \param value 
    /// Input the SignatureDefinition object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    /// Inserts a SignatureDefinition object into the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the SignatureDefinition object to insert.
    /// 
	System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    /// Removes the first occurrence of a specific SignatureDefinition object.
    /// 
    /// \param value 
    /// Input the SignatureDefinition object to remove from the collection.
    /// 
	System::Void Remove(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    /// Determines whether the collection contains a specific SignatureDefinition object.
    /// 
    /// \param value 
    /// Input The SignatureDefinition object to search in the collection.
    /// 
    /// \return
    /// Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    ///
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *array[], System::Int32 startAt);

    /// \brief
    /// Gets the item in the collection at the specified index. 
    /// 
    /// \param index 
    /// The index of the item in the collection. The index is 0 based.
    /// 
    /// \return
    /// Returns an instance of a the collected item.
    /// Throws an instance of Exception if the index is out of range or an error occurs.
    /// 
	__property SignatureDefinition *get_RealTypeItem(System::Int32 index);

    /// \brief
    /// Sets the value of the item at the specified index
    /// 
    /// \param index 
    /// Input index of the item to set.
    /// 
    /// \param value 
    /// Input the value of the item
    /// 
	__property System::Void set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    /// Gets an item in the collection.
    /// 
    /// \param index 
    /// Input index of the item to retrieve.
    /// 
    /// \return
    /// Returns the item at the specified index
    /// 
	__property SignatureDefinition *get_Item(System::Int32 index);

    /// \brief
    /// Sets the value of the item at the specified index
    /// 
    /// \param index 
    /// Input index of the item to set.
    /// 
    /// \param value 
    /// Input the value of the item
    /// 
	__property System::Void set_Item(System::Int32 index, NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond
};

// -----------------------------------------------------------------------------------------------------

/// \brief
/// The ReadOnlySignatureDefinitionCollection class represents a collection of ReadOnlySignatureDefinition objects.
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class ReadOnlySignatureDefinitionCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable,
                                                                   public System::Collections::ICollection,
                                                                   public System::Collections::IEnumerable
{
/// \cond DOXYGEN-IGNORE
private:
    /// \brief
    /// A Nested class defined to provide enumeration of Dictionary elements
    ///
    /// Enumerators can be used to read the data in the collection, 
    /// but they cannot be used to modify the underlying collection.
    ///
    /// An enumerator remains valid as long as the collection remains unchanged. 
    /// If changes are made to the collection, such as adding, modifying, or deleting 
    /// elements, the enumerator is irrecoverably invalidated and the next call to 
    /// MoveNext or Reset throws an InvalidOperationException. If the collection is 
    /// modified between MoveNext and Current, Current returns the element that it is 
    /// set to, even if the enumerator is already invalidated.
    ///
    /// The enumerator does not have exclusive access to the collection; therefore, 
    /// enumerating through a collection is intrinsically not a thread-safe procedure. 
    /// Even when a collection is synchronized, other threads can still modify the 
    /// collection, which causes the enumerator to throw an exception. To guarantee 
    /// thread safety during enumeration, you can either lock the collection during 
    /// the entire enumeration or catch the exceptions resulting from changes made 
    /// by other threads.
    /// 
	__gc class Enumerator : public System::Collections::IEnumerator
	{
	private:
		ReadOnlySignatureDefinitionCollection *m_pCol;
		System::Int32 m_nIdx;

	public:
        /// \brief
        /// Constructs a new Collection Enumerator
        /// 
        /// \param col 
        /// Input The collection to enumerate.
        /// 
		Enumerator(ReadOnlySignatureDefinitionCollection *elements) : m_pCol(elements), m_nIdx(-1) 
		{

		}

        /// \brief
        /// Retrieves the current object at the enumerator location
        /// 
        /// \return
        /// Retuns the current object referenced by the enumerator
        /// 
		__property System::Object *get_Current();

        /// \brief
        /// Initially, the enumerator is positioned before the first object in the collection. 
        /// At this position, calling the Current property throws an exception. 
        /// Therefore, you must call the MoveNext method to advance the enumerator 
        /// to the first element of the collection before reading the value of Current.
        /// If MoveNext passes the end of the collection, the enumerator is positioned 
        /// after the last element in the collection and MoveNext returns false. 
        /// When the enumerator is at this position, subsequent calls to MoveNext also return false. 
        /// If the last call to MoveNext returned false, calling Current throws an exception. 
        /// To set Current to the first element of the collection again, you can call Reset 
        /// followed by MoveNext.
        /// 
        /// \return
        /// Retuns true if the Enumerator is able to move to a valid element
        /// otherwise false.
        /// 
		System::Boolean MoveNext();

        /// \brief
        /// Initially, the enumerator is positioned before the first element in the collection. 
        /// The Reset method brings the enumerator back to this position. 
        /// 
		System::Void Reset();
	};

public private:
	ReadOnlySignatureDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoReadOnlySignatureDefinitionCollection *GetImpObj();

private:
    // System::Collections::ICollection methods
	System::Void System::Collections::ICollection::CopyTo(System::Array *array, System::Int32 index);

    // System::Collections::ICollection properties
	__property System::Object *System::Collections::ICollection::get_SyncRoot();
	__property System::Boolean System::Collections::ICollection::get_IsSynchronized();

/// \endcond

public:
	ReadOnlySignatureDefinitionCollection();

    /// \brief
    /// Gets the count of items in collection.
    /// 
    /// \return
    /// Returns the number of items in the collection.
    /// 
	__property System::Int32 get_Count(System::Void);	

    /// \brief
    /// Gets an enumerator that can iterate through a collection.
    /// 
    /// \return
    /// Returns an enumerator on the dictionary.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    ///
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *array[], System::Int32 startAt);

    /// \brief
    /// Determines the index of a specific SignatureDefinition object.
    /// 
    /// \param value 
    /// Input the SignatureDefinition object to locate in the collection.
    /// 
    /// \return
    /// 	The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    /// Determines whether the collection contains a specific SignatureDefinition object.
    /// 
    /// \param value 
    /// Input The SignatureDefinition object to search in the collection.
    /// 
    /// \return
    /// 	Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition *value);

    /// \brief
    /// Gets the item in the collection at the specified index. 
    /// 
    /// \param index 
    /// The index of the item in the collection. The index is 0 based.
    /// 
    /// \return
    /// Returns an instance of a the collected item.
    /// Throws an instance of Exception if the index is out of range or an error occurs.
    /// 
	__property SignatureDefinition *get_RealTypeItem(System::Int32 index);

    /// \brief
    /// Gets the item in the collection at the specified index. 
    /// 
    /// \param index 
    /// The name of the item in the collection.
    /// 
    /// \return
    /// Returns an instance of a the collected item.
    /// 
__property SignatureDefinition *get_RealTypeItem(System::String* name);

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

