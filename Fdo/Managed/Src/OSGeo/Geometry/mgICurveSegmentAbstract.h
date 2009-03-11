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

class FdoCurveSegmentCollection;

BEGIN_NAMESPACE_OSGEO_GEOMETRY

interface class IEnvelope;
interface class IDirectPosition;

/// \ingroup (OSGeoFDOGeometry)
/// \interface OSGeo::FDO::Geometry::ICurveSegmentAbstract
/// \brief
/// The ICurveSegmentAbstract class is an abstract geometric Curve Segment object.  
/// This class is used strictly as a component of curves 
/// and, thus, does not inherit from IGeometry.
public interface class ICurveSegmentAbstract : public System::IDisposable
{
public:
    /// \brief
    /// Gets the envelope for the curve segment.
    /// 
    /// \return
    /// Returns the envelope
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ Envelope
    {
        NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ get();
    }
	
    /// \brief
    /// Gets the starting position of this curve segment.
    /// 
    /// \return
    /// Returns the starting position
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ StartPosition
    {
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ get();
    }

    /// \brief
    /// Gets the ending position of this curve segment.
    /// 
    /// \return
    /// Returns the ending position
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ EndPosition
    {
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ get();
    }
	
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
    property System::Boolean IsClosed
    {
        System::Boolean get();
    }
	
    /// \brief
    /// Gets the type of the most-derived interface 
    /// in the Geometry package for this object
    /// 
    /// \return
    /// Returns the derived type
    /// 
    property NAMESPACE_OSGEO_COMMON::GeometryComponentType DerivedType
    {
        NAMESPACE_OSGEO_COMMON::GeometryComponentType get();
    }

    /// \brief
    /// Gets the dimensionality of ordinates in this object.
    /// 
    /// \remarks
    /// Values are from the Dimensionality enumeration.
    /// A return type of "Int32" is used instead of the enumeration, catering to typical use with bit masking.
    /// 
    /// \return
    /// Returns the ordinate dimensionality
    /// 
    property System::Int32 Dimensionality
    {
        System::Int32 get();
    }
};

/// \brief
/// The CurveSegmentCollection class represents a collection of CurveSegment objects.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class CurveSegmentCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionBase
{
public:
    /// \brief
    /// Constructs a CurveSegmentCollection managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	CurveSegmentCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	FdoCurveSegmentCollection* GetImpObj();


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
	CurveSegmentCollection();

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
    /// Removes the index-th ICurveSegmentAbstract from this collection.
    /// 
    /// \param index 
    /// Input index of the element to remove.
    /// 
	virtual System::Void RemoveAt(System::Int32 index) override;

    /// \brief
    /// Removes all elements from the collection.
    /// 
	virtual System::Void  Clear() override;

    /// \brief
    /// Adds a ICurveSegmentAbstract object into the collection.
    /// 
    /// \param value 
    /// Input the ICurveSegmentAbstract object to add.
    /// 
    /// \return
    /// The position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value);

    /// \brief
    /// Determines the index of a specific ICurveSegmentAbstract object.
    /// 
    /// \param value 
    /// Input the ICurveSegmentAbstract object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
    System::Int32 IndexOf(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value);

    /// \brief
    /// Inserts an ICurveSegmentAbstract object to the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the ICurveSegmentAbstract object to insert.
    /// 
    System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value); 

    /// \brief
    /// Removes the first occurrence of a specific ICurveSegmentAbstract object.
    /// 
    /// \param value 
    /// Input the ICurveSegmentAbstract object to remove from the collection.
    /// 
    System::Void Remove(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value);

    /// \brief
    /// Determines whether the collection contains a specific ICurveSegmentAbstract object.
    /// 
    /// \param value 
    /// Input The ICurveSegmentAbstract object to locate in the collection.
    /// 
    /// \return
    /// True if the value is found in the collection; otherwise, false.
    /// 
    System::Boolean Contains(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
    System::Void CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^>^ pAarray, System::Int32 startAt);

    /// \brief
    /// Gets an ICurveSegmentAbstract in the collection.
    /// 
    /// \param index 
    /// Input index of the ICurveSegmentAbstract to retrieve
    /// 
    /// \brief
    /// Sets the value of the ICurveSegmentAbstract object at the specified index
    /// 
    /// \param index 
    /// Input index of the ICurveSegmentAbstract object to set.
    /// 
    /// \param value 
    /// Input the value of the ICurveSegmentAbstract object
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ Item[System::Int32]
    {
        NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ get(System::Int32 index);
        System::Void set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value);
    }
};

END_NAMESPACE_OSGEO_GEOMETRY


