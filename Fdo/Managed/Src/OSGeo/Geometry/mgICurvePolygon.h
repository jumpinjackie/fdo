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

#include "mgISurfaceAbstract.h"

class FdoCurvePolygonCollection;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
interface class IRing;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \interface OSGeo::FDO::Geometry::ICurvePolygon
/// \brief
/// The ICurvePolygon class is a CurvePolygon geometry type.  
/// This type is defined by one exterior boundary and zero or more 
/// interior boundaries.
/// Each of the boundaries must be a closed, simple Ring.
public interface class ICurvePolygon : public NAMESPACE_OSGEO_GEOMETRY::ISurfaceAbstract
{
public:
    /// \brief
    /// Gets the ring defining the outer boundary.
    /// 
    /// \return
    /// Returns the exterior ring
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IRing^ ExteriorRing
    {
        NAMESPACE_OSGEO_GEOMETRY::IRing^ get();
    }
	
    /// \brief
    /// Gets the number of interior rings.
    /// 
    /// \return
    /// Returns the number of interior rings
    /// 
    property System::Int32 InteriorRingCount
    {
        System::Int32 get();
    }
	
    /// \brief
    /// Gets the interior ring at the specified (zero-based) index.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of interior rings.
    /// 
    /// \return
    /// Returns the interior ring
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IRing^ InteriorRing[System::Int32]
    {
        NAMESPACE_OSGEO_GEOMETRY::IRing^ get(System::Int32 index);
    }
};


/// \brief
/// The CurvePolygonCollection class is a collection of CurvePolygon objects.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class CurvePolygonCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionBase
{
public:
    /// \brief
    /// Constructs a CurvePolygonCollection managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	CurvePolygonCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	FdoCurvePolygonCollection* GetImpObj();

private:
    virtual property System::Object^ IndexInternal[System::Int32]
    {
        private: System::Object^ get(System::Int32 index) sealed = IList::default::get;
        private: void set(System::Int32 index, System::Object^ value) sealed = IList::default::set;
    }

    // System::Collections::IList interface methods
    virtual System::Int32 Add(System::Object^ value) sealed = IList::Add;
    virtual System::Boolean Contains(System::Object^ value) sealed = IList::Contains;
    virtual System::Int32 IndexOf(System::Object^ value) sealed = IList::IndexOf;
    virtual System::Void Insert(System::Int32 index, System::Object^ value) sealed = IList::Insert;
    virtual System::Void Remove(System::Object^ value) sealed = IList::Remove;

/// \endcond

public:
    /// \brief
    /// Creates an instance of CurveSegmentCollection with no contained elements.
    /// 
    /// \return
    /// Returns an empty collection
    /// 
	CurvePolygonCollection();

public:
    /// \brief
    /// Gets the count of items in collection.
    /// 
    /// \return
    /// Returns the number of items in the collection.
    /// 
    property System::Int32 Count
    {
        virtual System::Int32 get() override;
    }

    /// \brief
    /// Removes the index-th ICurvePolygon from this collection.
    /// 
    /// \param index 
    /// Input index of the element to remove.
    /// 
	virtual System::Void RemoveAt(System::Int32 index) override;

    /// \brief
    /// Removes all elements from the collection.
    /// 
	virtual System::Void  Clear() override;

public:
    /// \brief
    /// Adds a ICurvePolygon object into the collection.
    /// 
    /// \param value 
    /// Input the ICurvePolygon object to add.
    /// 
    /// \return
    /// The position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value);

    /// \brief
    /// Determines the index of a specific ICurvePolygon object.
    /// 
    /// \param value 
    /// Input the ICurvePolygon object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
    System::Int32 IndexOf(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value);

    /// \brief
    /// Inserts an ICurvePolygon object to the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the ICurvePolygon object to insert.
    /// 
    System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value); 

    /// \brief
    /// Removes the first occurrence of a specific ICurvePolygon object.
    /// 
    /// \param value 
    /// Input the ICurvePolygon object to remove from the collection.
    /// 
    System::Void Remove(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value);

    /// \brief
    /// Determines whether the collection contains a specific ICurvePolygon object.
    /// 
    /// \param value 
    /// Input The ICurvePolygon object to locate in the collection.
    /// 
    /// \return
    /// True if the value is found in the collection; otherwise, false.
    /// 
    System::Boolean Contains(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
    System::Void CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^>^ pArray, System::Int32 startAt);

    /// \brief
    /// Gets an ICurvePolygon in the collection.
    /// 
    /// \param index 
    /// Input index of the ICurvePolygon to retrieve.
    /// 
    /// \brief
    /// Sets the value of the ICurvePolygon at the specified index
    /// 
    /// \param index 
    /// Input index of the ICurvePolygon to set.
    /// 
    /// \param value 
    /// Input the value of the ICurvePolygon
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ Item[System::Int32]
    {
        NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ get(System::Int32 index);
        System::Void set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value);
    }
};

END_NAMESPACE_OSGEO_GEOMETRY


