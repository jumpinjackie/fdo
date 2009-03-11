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

class FdoGeometryCollection;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
interface class IEnvelope;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \interface OSGeo::FDO::Geometry::IGeometry
/// \brief
/// The IGeometry class defines the properties and methods common to all geometric
/// types.  IGeometry is an abstract type.
public interface class IGeometry : public System::IDisposable
{
public:
    /// \brief
    /// Gets the envelope for the Geometry.
    /// 
    /// \return
    /// Returns the envelope
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ Envelope
    {
        NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ get();
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

    /// \brief
    /// Gets the type of the most-derived interface in the Geometry package for this object
    /// 
    /// \remarks
    /// The derived type indicates to what class the object may safely be cast.
    /// 
    /// \return
    /// Returns the derived type
    /// 
    property NAMESPACE_OSGEO_COMMON::GeometryType DerivedType
    {
        NAMESPACE_OSGEO_COMMON::GeometryType get();
    }

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
    property System::String^ Text
    {
        System::String^ get();
    }
};

/// \brief
/// The GeometryCollection class is a collection of Geometry objects.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class GeometryCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionBase
{
public:
    /// \brief
    /// Constructs a GeometryCollection managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	GeometryCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	FdoGeometryCollection* GetImpObj();

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
    /// Creates an instance of GeometryCollection with no contained elements.
    /// 
    /// \return
    /// Returns an empty collection
    /// 
	GeometryCollection();

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
    /// Removes the index-th IGeometry from this collection.
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
    /// Adds a IGeometry object into the collection.
    /// 
    /// \param value 
    /// Input the IGeometry object to add.
    /// 
    /// \return
    /// The position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value);

    /// \brief
    /// Determines the index of a specific IGeometry object.
    /// 
    /// \param value 
    /// Input the IGeometry object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
    System::Int32 IndexOf(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value);

    /// \brief
    /// Inserts an IGeometry object to the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the IGeometry object to insert.
    /// 
    System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value); 

    /// \brief
    /// Removes the first occurrence of a specific IGeometry object.
    /// 
    /// \param value 
    /// Input the IGeometry object to remove from the collection.
    /// 
    System::Void Remove(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value);

    /// \brief
    /// Determines whether the collection contains a specific IGeometry object.
    /// 
    /// \param value 
    /// Input The IGeometry object to locate in the collection.
    /// 
    /// \return
    /// True if the value is found in the collection; otherwise, false.
    /// 
    System::Boolean Contains(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::IGeometry^>^ pArray, System::Int32 index);

    /// \brief
    /// Gets an IGeometry object in the collection.
    /// 
    /// \param index 
    /// Input index of the IGeometry object to retrieve.
    /// 
    /// \brief
    /// Sets the value of the IGeometry object at the specified index
    /// 
    /// \param index 
    /// Input index of the IGeometry object to set.
    /// 
    /// \param value 
    /// Input the value of the IGeometry
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IGeometry^ Item[System::Int32]
    {
        NAMESPACE_OSGEO_GEOMETRY::IGeometry^ get(System::Int32 index);
        System::Void set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value);
    }
};

END_NAMESPACE_OSGEO_GEOMETRY


