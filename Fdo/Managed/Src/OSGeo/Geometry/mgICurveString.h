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

#include "mgICurveAbstract.h"

class FdoCurveStringCollection;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
public __gc __interface ICurveSegmentAbstract;
public __gc class CurveSegmentCollection;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \brief
/// The ICurveString class is a CurveString Geometry type. ICurveString is the most general non-abstract curve type.  
/// It is defined by an ordered collection of contiguous curve segments.
public __gc __interface ICurveString : public NAMESPACE_OSGEO_GEOMETRY::ICurveAbstract
{
public:
    /// \brief
    /// Gets the number of curve segments in this curve.  
    /// 
    /// \return
    /// Returns the number of curve segments
    /// 
	__property System::Int32 get_Count();
	
    /// \brief
    /// Gets the curve segment at the given zero-based index.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of curve segments.
    /// 
    /// \return
    /// Returns the requested curve segment
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *get_Item(System::Int32 index);
	
    /// \brief
    /// Gets all the curve segments.
    /// 
    /// \return
    /// Returns a collection of all the curve segments
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection *get_CurveSegments();
};

/// \brief
/// The CurveStringCollection class is a collection of CurvePolygon objects.
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __gc __sealed class CurveStringCollection 
	: public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::IList
{
public:
    /// \brief
    /// Constructs a CurveStringCollection managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	CurveStringCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	FdoCurveStringCollection *GetImpObj();

/// \cond DOXYGEN-IGNORE
protected:
	__sealed System::Void ReleaseUnmanagedObject();

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
		CurveStringCollection *m_pCol;
		System::Int32 m_nIdx;

    public:
        /// \brief
        /// Constructs a new Collection Enumerator
        /// 
        /// \param col 
        /// Input The collection to enumerate.
        /// 
		Enumerator(CurveStringCollection *col)
			: m_pCol(col), m_nIdx(-1)
		{}

        /// \brief
        /// Retrieves the current object at the enumerator location
        /// 
        /// \return
        /// Retuns the current object referenced by the enumerator
        /// 
		__property System::Object *get_Current();

        /// \brief
        /// Initially, the enumerator is positioned before the first element in the collection. 
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
/// \endcond

public:
    /// \brief
    /// Creates an instance of CurveSegmentCollection with no contained elements.
    /// 
    /// \return
    /// Returns an empty collection
    /// 
	CurveStringCollection();

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

public:
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
    /// Removes the index-th ICurveString from this collection.
    /// 
    /// \param index 
    /// Input index of the element to remove.
    /// 
	System::Void RemoveAt(System::Int32 index);

    /// \brief
    /// Removes all elements from the collection.
    /// 
	System::Void  Clear();

public:
    /// \brief
    /// Adds a ICurveString object into the collection.
    /// 
    /// \param value 
    /// Input the ICurveString object to add.
    /// 
    /// \return
    /// The position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_GEOMETRY::ICurveString* value);

    /// \brief
    /// Determines the index of a specific ICurveString object.
    /// 
    /// \param value 
    /// Input the ICurveString object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
    System::Int32 IndexOf(NAMESPACE_OSGEO_GEOMETRY::ICurveString* value);

    /// \brief
    /// Inserts an ICurveString object to the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the ICurveString object to insert.
    /// 
    System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveString* value);

    /// \brief
    /// Removes the first occurrence of a specific ICurveString object.
    /// 
    /// \param value 
    /// Input the ICurveString object to remove from the collection.
    /// 
    System::Void Remove(NAMESPACE_OSGEO_GEOMETRY::ICurveString* value);

    /// \brief
    /// Determines whether the collection contains a specific ICurveString object.
    /// 
    /// \param value 
    /// Input The ICurveString object to locate in the collection.
    /// 
    /// \return
    /// True if the value is found in the collection; otherwise, false.
    /// 
    System::Boolean Contains(NAMESPACE_OSGEO_GEOMETRY::ICurveString* value);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
    System::Void CopyTo(NAMESPACE_OSGEO_GEOMETRY::ICurveString* array[],System::Int32 startAt);

    /// \brief
    /// Gets an ICurveString in the collection.
    /// 
    /// \param index 
    /// Input index of the ICurveString to retrieve.
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::ICurveString *get_RealTypeItem(System::Int32 index);

    /// \brief
    /// Sets the value of the ICurveString at the specified index
    /// 
    /// \param index 
    /// Input index of the ICurveString to set.
    /// 
    /// \param value 
    /// Input the value of the ICurveString
    /// 
    __property System::Void set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveString *value);

    /// \brief
    /// Gets an ICurveString in the collection.
    /// 
    /// \param index 
    /// Input index of the ICurveString to retrieve.
    /// 
    /// \return
    /// Returns the ICurveString at the specified index
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::ICurveString *get_Item(System::Int32 index);

    /// \brief
    /// Sets the value of the ICurveString at the specified index
    /// 
    /// \param index 
    /// Input index of the ICurveString to set.
    /// 
    /// \param value 
    /// Input the value of the ICurveString
    /// 
    __property System::Void set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveString *value);
};

END_NAMESPACE_OSGEO_GEOMETRY


