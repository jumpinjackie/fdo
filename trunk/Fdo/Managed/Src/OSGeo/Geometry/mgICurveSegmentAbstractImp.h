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

#include "mgICurveSegmentAbstract.h"

class FdoICurveSegmentAbstract;
class FdoCurveSegmentCollection;

BEGIN_NAMESPACE_OSGEO_GEOMETRY

public __gc __interface IEnvelope;
public __gc __interface IDirectPosition;

/// \brief
/// The ICurveSegmentAbstractImp class is a concrete geometric Curve Segment object.  
/// This class is used strictly as a component of curves and, thus, does not inherit from IGeometry.
public __gc class ICurveSegmentAbstractImp
	: public NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract, public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
	ICurveSegmentAbstractImp(IntPtr unmanaged, Boolean autoDelete);
	
protected:
	System::Void ReleaseUnmanagedObject();

public:
    /// \brief
    /// Gets the envelope for the curve segment.
    /// 
    /// \return
    /// Returns the envelope
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::IEnvelope *get_Envelope();
	
    /// \brief
    /// Gets the starting position of this curve segment.
    /// 
    /// \return
    /// Returns the starting position
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *get_StartPosition();

    /// \brief
    /// Gets the ending position of this curve segment.
    /// 
    /// \return
    /// Returns the ending position
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *get_EndPosition();
	
    /// \brief
    /// Gets the closure state for the curve segment.
    /// 
    /// \remarks
    /// The meaning behind this method is not guaranteed
    /// to be uniform between derived types or between implementations
    /// of this package.
    /// It may represent a computed value, an explicit attribute, or be true by definition.
    /// As a computed value, the result is typically from simply testing the starting and 
    /// ending positions for exact equality.  This is only reliable in floating
    /// point arithmetic if these data have identical origins.
    /// As an explicit attribute, it would be persisted with the Geometry and 
    /// typically denoted by a parameter in the relevant factory method.  
    /// Some Geometry types are closed by definition.
    /// 
    /// \return
    /// Returns 'true' if the curve is closed, and false otherwise
    /// 
	__property System::Boolean get_IsClosed();
	
    /// \brief
    /// Gets the type of the most-derived interface 
    /// in the Geometry package for this object
    /// 
    /// \return
    /// Returns the derived type
    /// 
	__property NAMESPACE_OSGEO_COMMON::GeometryComponentType get_DerivedType();

    /// \brief
    /// Gets the dimensionality of ordinates in this object.
    /// 
    /// \remarks
    ///  Values are from the Dimensionality enumeration.
    /// A return type of "Int32" is used instead of the enumeration, catering to typical use with bit masking.
    /// 
    /// \return
    /// Returns the ordinate dimensionality
    /// 
	__property System::Int32 get_Dimensionality();

public private:
	FdoICurveSegmentAbstract *GetImpObj();
};

[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __gc __sealed class CurveSegmentCollection 
	: public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::IList
{
public:
	CurveSegmentCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	FdoCurveSegmentCollection *GetImpObj();

protected:
	__sealed System::Void ReleaseUnmanagedObject();

private:

	__gc class Enumerator : public System::Collections::IEnumerator
	{
		CurveSegmentCollection *m_pCol;
		System::Int32 m_nIdx;
	public:
		Enumerator(CurveSegmentCollection *col)
			: m_pCol(col), m_nIdx(-1)
		{}

		__property System::Object *get_Current();
		System::Boolean MoveNext();
		System::Void Reset();
	};
public:
    /// \brief
    ///  Creates an instance of CurveSegmentCollection with no contained elements.
    /// 
    /// \return
    /// Returns an empty collection
    /// 
	CurveSegmentCollection();

private:
    // System::Collections::ICollection
	System::Void System::Collections::ICollection::CopyTo(System::Array* array,System::Int32 index);
	__property System::Object* System::Collections::ICollection::get_SyncRoot(System::Void);
	__property System::Boolean System::Collections::ICollection::get_IsSynchronized(System::Void);
    // System::Collections::IList
	__property System::Boolean System::Collections::IList::get_IsFixedSize(System::Void);
	__property System::Boolean System::Collections::IList::get_IsReadOnly(System::Void);
	System::Int32 System::Collections::IList::Add(Object* value);
	System::Boolean System::Collections::IList::Contains(Object* value);
	System::Int32 System::Collections::IList::IndexOf(Object* value);
	System::Void System::Collections::IList::Insert(System::Int32 index, Object* value);
	System::Void System::Collections::IList::Remove(Object* value);
	__property Object* System::Collections::IList::get_Item(System::Int32 index);
	__property System::Void  System::Collections::IList::set_Item(System::Int32 index, Object* value);

public:
    /// \brief
    ///    Gets the count of items in collection.
    /// 
	__property System::Int32 get_Count(System::Void);

    /// \brief
    ///    Gets an enumerator that can iterate through a collection.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

    /// \brief
    ///     Removes the index-th ICurveSegmentAbstract from this collection.
    /// 
    /// \param index 
    /// Input index of the element to remove.
    /// 
	System::Void RemoveAt(System::Int32 index);
    /// \brief
    ///     Removes all elements from the collection.
    /// 
	System::Void  Clear();

public:
    /// \brief
    ///     Adds a ICurveSegmentAbstract object into the collection.
    /// 
    /// \param value 
    /// Input the ICurveSegmentAbstract object to add.
    /// 
    /// \return
    /// 		The position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract* value);
    /// \brief
    ///     Determines the index of a specific ICurveSegmentAbstract object.
    /// 
    /// \param value 
    /// Input the ICurveSegmentAbstract object to locate in the collection.
    /// 
    /// \return
    /// 		The index of value if found in the collection; otherwise, -1.
    /// 
    System::Int32 IndexOf(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract* value);
    /// \brief
    ///     Inserts an ICurveSegmentAbstract object to the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the ICurveSegmentAbstract object to insert.
    /// 
    System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract* value); 
    /// \brief
    ///     Removes the first occurrence of a specific ICurveSegmentAbstract object.
    /// 
    /// \param value 
    /// Input the ICurveSegmentAbstract object to remove from the collection.
    /// 
    System::Void Remove(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract* value);
    /// \brief
    ///     Determines whether the collection contains a specific ICurveSegmentAbstract object.
    /// 
    /// \param value 
    /// Input The ICurveSegmentAbstract object to locate in the collection.
    /// 
    /// \return
    /// 		True if the value is found in the collection; otherwise, false.
    /// 
    System::Boolean Contains(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract* value);
    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
    System::Void CopyTo(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract* array[],System::Int32 startAt);

    /// \brief
    ///     Gets or sets an ICurveSegmentAbstract in the collection.
    /// 
    /// \param index 
    /// Input index of the ICurveSegmentAbstract to retrieve or set (System::Int32).
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *get_RealTypeItem(System::Int32 index);
    __property System::Void set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *value);

	__property NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *get_Item(System::Int32 index);
    __property System::Void set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *value);
};
	
END_NAMESPACE_OSGEO_GEOMETRY


