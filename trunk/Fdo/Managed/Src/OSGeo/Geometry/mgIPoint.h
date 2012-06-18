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

class FdoPointCollection;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
interface class IDirectPosition;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \interface OSGeo::FDO::Geometry::IPoint
/// \brief
/// The IPoint class is a point Geometry type.
public interface class IPoint : public NAMESPACE_OSGEO_GEOMETRY::IGeometry
{
public:
    /// \brief
    /// Gets the position of this point Geometry.
    /// 
    /// \return
    /// Returns the position
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ Position
    {
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ get();
    }
	
    /// \brief
    /// Gets the position of this point Geometry, by values of its member data.
    /// This is in aid of higher performance for any implementation that
    /// does not internally use FdoIDirectPosition objects for storage, or for
    /// an application seeking to avoid overhead of accessor methods.
    /// 
    /// \param coordinateX 
    /// Output X ordinate value.
    /// \param coordinateY 
    /// Output Y ordinate value.
    /// \param coordinateZ 
    /// Output Z ordinate value.
    /// \param coordinateM 
    /// Output M ordinate value.
    /// \param dimensionality 
    /// Output Dimensionality of ordinates in this position.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void GetPositionByMembers(
		System::Double% coordinateX,
		System::Double% coordinateY,
		System::Double% coordinateZ,
		System::Double% coordinateM,
		System::Int32% dimensionality) abstract;
};

/// \brief
/// The PointCollection class is a collection of Point objects.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class PointCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionBase
{
public:
    /// \brief
    /// Constructs a managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed element should be automatically deleted 
    /// once it no longer referenced.
    /// 
	PointCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	FdoPointCollection* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

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
    /// Creates an instance of PointCollection with no contained elements.
    /// 
    /// \return
    /// Returns an empty collection
    /// 
	PointCollection();

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
    /// Removes the index-th IPoint from this collection.
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
    /// Adds a IPoint object into the collection.
    /// 
    /// \param value 
    /// Input the IPoint object to add.
    /// 
    /// \return
    /// The position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_GEOMETRY::IPoint^ value);
    /// \brief
    /// Determines the index of a specific IPoint object.
    /// 
    /// \param value 
    /// Input the IPoint object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
    System::Int32 IndexOf(NAMESPACE_OSGEO_GEOMETRY::IPoint^ value);
    /// \brief
    /// Inserts an IPoint object to the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the IPoint object to insert.
    /// 
    System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPoint^ value); 
    /// \brief
    /// Removes the first occurrence of a specific IPoint object.
    /// 
    /// \param value 
    /// Input the IPoint object to remove from the collection.
    /// 
    System::Void Remove(NAMESPACE_OSGEO_GEOMETRY::IPoint^ value);
    /// \brief
    /// Determines whether the collection contains a specific IPoint object.
    /// 
    /// \param value 
    /// Input The IPoint object to locate in the collection.
    /// 
    /// \return
    /// True if the value is found in the collection; otherwise, false.
    /// 
    System::Boolean Contains(NAMESPACE_OSGEO_GEOMETRY::IPoint^ value);
    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::IPoint^>^ pArray, System::Int32 index);

    /// \brief
    /// Gets an IPoint object in the collection.
    /// 
    /// \param index 
    /// Input index of the IPoint object to retrieve.
    /// 
    /// \brief
    /// Sets the value of the IPoint object at the specified index
    /// 
    /// \param index 
    /// Input index of the IPoint object to set.
    /// 
    /// \param value 
    /// Input the value of the IPoint
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IPoint^ Item[System::Int32]
    {
        NAMESPACE_OSGEO_GEOMETRY::IPoint^ get(System::Int32 index);
        System::Void set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPoint^ value);
    }
};

END_NAMESPACE_OSGEO_GEOMETRY


