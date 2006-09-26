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

#include "mgIGeometry.h"

class FdoIGeometry;
class FdoGeometryCollection;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
public __gc __interface IEnvelope;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \brief
/// The IGeometryImp class defines the properties and methods common to all geometric
/// types. 
public __gc class IGeometryImp 
	: public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_GEOMETRY::IGeometry
{
public:
	IGeometryImp(System::IntPtr unmanaged, System::Boolean autoDelete);

protected:
	System::Void ReleaseUnmanagedObject();

public:
    /// \brief
    /// Gets the envelope for the Geometry.
    /// 
    /// \return
    /// Returns the envelope
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::IEnvelope* get_Envelope();

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

    /// \brief
    /// Gets the type of the most-derived interface 
    /// in the Geometry package for this object
    /// 
    /// \remarks
    ///  The derived type indicates to what class the object may safely be cast.
    /// 
    /// \return
    /// Returns the derived type
    /// 
	__property NAMESPACE_OSGEO_COMMON::GeometryType get_DerivedType();

    /// \brief
    /// Gets the text string representation of this Geometry.
    /// 
    /// \remarks
    /// This object retains ownership of the string.
    /// The caller should NOT attempt to free it.
    /// The text string may be retained in memory by the object indefinitely.
    /// Calling this method on a large number of Geometries that are retained
    /// in memory may cause a noticable increase in memory consumption.
    /// 
    /// \return
    /// Returns the text string
    /// 
	__property System::String* get_Text();

public private:
	FdoIGeometry *GetImpObj();
};

/// \brief
///  Collection of CurvePolygon objects.
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __gc __sealed class GeometryCollection 
	: public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::IList
{
public:
	GeometryCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	FdoGeometryCollection *GetImpObj();

protected:
	__sealed System::Void ReleaseUnmanagedObject();

private:

	__gc class Enumerator : public System::Collections::IEnumerator
	{
		GeometryCollection *m_pCol;
		System::Int32 m_nIdx;
	public:
		Enumerator(GeometryCollection *col)
			: m_pCol(col), m_nIdx(-1)
		{}

		__property System::Object *get_Current();
		System::Boolean MoveNext();
		System::Void Reset();
	};
public:
    /// \brief
    ///  Creates an instance of FdoCurveSegmentCollection with no contained elements.
    /// 
    /// \return
    /// Returns an empty collection
    /// 
	GeometryCollection();

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
    ///     Removes the index-th IGeometry from this collection.
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
    ///     Adds a IGeometry object into the collection.
    /// 
    /// \param value 
    /// Input the IGeometry object to add.
    /// 
    /// \return
    /// 		The position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_GEOMETRY::IGeometry* value);
    /// \brief
    ///     Determines the index of a specific IGeometry object.
    /// 
    /// \param value 
    /// Input the IGeometry object to locate in the collection.
    /// 
    /// \return
    /// 		The index of value if found in the collection; otherwise, -1.
    /// 
    System::Int32 IndexOf(NAMESPACE_OSGEO_GEOMETRY::IGeometry* value);
    /// \brief
    ///     Inserts an IGeometry object to the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the IGeometry object to insert.
    /// 
    System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IGeometry* value); 
    /// \brief
    ///     Removes the first occurrence of a specific IGeometry object.
    /// 
    /// \param value 
    /// Input the IGeometry object to remove from the collection.
    /// 
    System::Void Remove(NAMESPACE_OSGEO_GEOMETRY::IGeometry* value);
    /// \brief
    ///     Determines whether the collection contains a specific IGeometry object.
    /// 
    /// \param value 
    /// Input The IGeometry object to locate in the collection.
    /// 
    /// \return
    /// 		True if the value is found in the collection; otherwise, false.
    /// 
    System::Boolean Contains(NAMESPACE_OSGEO_GEOMETRY::IGeometry* value);
    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
    System::Void CopyTo(NAMESPACE_OSGEO_GEOMETRY::IGeometry* array[],System::Int32 startAt);

    /// \brief
    ///     Gets or sets an IGeometry in the collection.
    /// 
    /// \param index 
    /// Input index of the IGeometry to retrieve or set (System::Int32).
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::IGeometry *get_RealTypeItem(System::Int32 index);
    __property System::Void set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IGeometry *value);

	__property NAMESPACE_OSGEO_GEOMETRY::IGeometry *get_Item(System::Int32 index);
    __property System::Void set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IGeometry *value);
};

END_NAMESPACE_OSGEO_GEOMETRY


