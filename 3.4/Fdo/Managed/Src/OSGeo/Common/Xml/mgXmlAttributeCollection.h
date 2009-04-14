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

#include "Runtime\Disposable.h"

class FdoXmlAttributeCollection;

BEGIN_NAMESPACE_OSGEO_COMMON_XML

public __gc class XmlAttribute;

/// \ingroup (OSGeoFDOCommonXml)
/// \brief
/// The XmlAttributeCollection class represents a collection of XmlAttribute objects.
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class XmlAttributeCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::IList
{
/// \cond DOXYGEN-IGNORE
private:
    /// \brief
    /// 	A Nested class defined to provide enumeration of Dictionary elements
    ///
    ///     Enumerators can be used to read the data in the collection, 
    ///     but they cannot be used to modify the underlying collection.
    ///
    ///     An enumerator remains valid as long as the collection remains unchanged. 
    ///     If changes are made to the collection, such as adding, modifying, or deleting 
    ///     elements, the enumerator is irrecoverably invalidated and the next call to 
    ///     MoveNext or Reset throws an InvalidOperationException. If the collection is 
    ///     modified between MoveNext and Current, Current returns the element that it is 
    ///     set to, even if the enumerator is already invalidated.
    ///
    ///     The enumerator does not have exclusive access to the collection; therefore, 
    ///     enumerating through a collection is intrinsically not a thread-safe procedure. 
    ///     Even when a collection is synchronized, other threads can still modify the 
    ///     collection, which causes the enumerator to throw an exception. To guarantee 
    ///     thread safety during enumeration, you can either lock the collection during 
    ///     the entire enumeration or catch the exceptions resulting from changes made 
    ///     by other threads.
    /// 
	__gc class Enumerator : public System::Collections::IEnumerator
	{
	private:
		XmlAttributeCollection* m_pCol;
		System::Int32 m_nIdx;

	public:
        /// \brief
        ///     Constructs a new Collection Enumerator
        /// 
        /// \param elements 
        ///     Input The attribute collection to enumerate.
        /// 
		Enumerator(XmlAttributeCollection* elements) : m_pCol(elements), m_nIdx(-1) 
		{
		}

        /// \brief
        ///     Retrieves the current object at the enumerator location
        /// 
        /// \return
        /// 	Retuns the current object referenced by the enumerator
        /// 
		__property System::Object* get_Current();

        /// \brief
        ///     Initially, the enumerator is positioned before the first element in the collection. 
        ///     At this position, calling the Current property throws an exception. 
        ///     Therefore, you must call the MoveNext method to advance the enumerator 
        ///     to the first element of the collection before reading the value of Current.
        //      If MoveNext passes the end of the collection, the enumerator is positioned 
        ///     after the last element in the collection and MoveNext returns false. 
        ///     When the enumerator is at this position, subsequent calls to MoveNext also return false. 
        ///     If the last call to MoveNext returned false, calling Current throws an exception. 
        //      To set Current to the first element of the collection again, you can call Reset 
        ///     followed by MoveNext.
        /// 
        /// \return
        /// 	Retuns true if the Enumerator is able to move to a valid string element
        ///     otherwise false.
        /// 
		System::Boolean MoveNext();

        /// \brief
        ///     Initially, the enumerator is positioned before the first element in the collection. 
        ///     The Reset method brings the enumerator back to this position. 
        /// 
		System::Void Reset();
	};

public private:
	inline FdoXmlAttributeCollection* GetImpObj();

protected:
	virtual System::Void ReleaseUnmanagedObject();

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
    /// \brief
    ///     Constructs a new empty attribute collection
    /// 
	XmlAttributeCollection();

    /// \brief
    ///     Constructs a collection based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    ///     Input A Pointer to the unmanaged collection.
    /// 
    /// \param autoDelete 
    ///     Input Indicates if the constructed object should be automatically deleted 
    ///     once it no longer referenced.
    /// 
	XmlAttributeCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

    /// \brief
    ///    Gets the count of items in collection.
    /// 
    /// \return
    /// 	Returns the number of items in the collection.
    /// 
	__property System::Int32 get_Count(System::Void);

    /// \brief
    ///    Gets an enumerator that can iterate through a collection.
    /// 
    /// \return
    /// 	Returns an enumerator on the collection.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

    /// \brief
    ///     Removes the index-th XmlAttribute from this collection.
    /// 
    /// \param index 
    ///     Input index of the element to remove.
    /// 
	System::Void RemoveAt(System::Int32 index);

    /// \brief
    ///     Removes all elements from the collection.
    /// 
	System::Void  Clear();

    /// \brief
    ///     Adds a XmlAttribute object into the collection.
    /// 
    /// \param value 
    ///     Input the XmlAttribute object to add.
    /// 
    /// \return
    /// 		Returns the position into which the new element was inserted.
    /// 
	System::Int32 Add(XmlAttribute* value);

    /// \brief
    ///     Determines the index of a specific XmlAttribute object.
    /// 
    /// \param value 
    ///     Input the XmlAttribute object to locate in the collection.
    /// 
    /// \return
    ///     The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(XmlAttribute* value);

    /// \brief
    ///     Determines the index of a specific XmlAttribute object.
    /// 
    /// \param name 
    ///     Input the name of the XmlAttribute object to locate in the collection.
    /// 
    /// \return
    ///     The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(String* name);

    /// \brief
    ///     Inserts a XmlAttribute object into the collection at the specified position.
    /// 
    /// \param index 
    ///     Input the zero-based index at which value should be inserted.
    ///
    /// \param value 
    ///     Input the XmlAttribute object to insert.
    /// 
	System::Void Insert(System::Int32 index, XmlAttribute* value);

    /// \brief
    ///     Removes the first occurrence of a specific XmlAttribute object.
    /// 
    /// \param value 
    ///     Input the XmlAttribute object to remove from the collection.
    /// 
	System::Void Remove(XmlAttribute* value);

    /// \brief
    ///     Determines whether the collection contains a specific XmlAttribute object.
    /// 
    /// \param value 
    ///     Input The XmlAttribute object to search in the collection.
    /// 
    /// \return
    ///     Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(XmlAttribute* value);

    /// \brief
    ///     Determines whether the collection contains a specific XmlAttribute object.
    /// 
    /// \param name 
    ///     Input The name of the XmlAttribute object to search in the collection.
    /// 
    /// \return
    ///     Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(String* name);

    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    ///     Output the one-dimensional Array that is the destination of the elements copied from this collection.
    ///
    /// \param index 
    ///     Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(XmlAttribute* array[],System::Int32 index);

    /// \brief
    ///     Gets an XmlAttribute in the collection.
    /// 
    /// \param index 
    ///     Input index of the XmlAttribute to retrieve.
    /// 
    /// \return 
    ///     An XmlAttribute found in the collection
    /// 
	__property XmlAttribute* get_RealTypeItem(System::Int32 index);

    /// \brief
    ///     Sets an XmlAttribute in the collection.
    /// 
    /// \param index 
    ///     Input index of the XmlAttribute to set.
    /// 
    /// \param value 
    ///     Input the value of the XmlAttribute
    /// 
	__property System::Void  set_RealTypeItem(System::Int32 index, XmlAttribute* value);

    /// \brief
    ///     Gets an XmlAttribute in the collection.
    /// 
    /// \param index 
    ///     Input index of the XmlAttribute to retrieve.
    /// 
    /// \return 
    ///     An XmlAttribute found in the collection
    /// 
	__property XmlAttribute* get_Item(System::Int32 index);

    /// \brief
    ///     Sets an XmlAttribute in the collection.
    /// 
    /// \param index 
    ///     Input index of the XmlAttribute to set.
    /// 
    /// \param value 
    ///     Input the value of the XmlAttribute
    /// 
	__property System::Void  set_Item(System::Int32 index, XmlAttribute* value);
};

END_NAMESPACE_OSGEO_COMMON_XML


