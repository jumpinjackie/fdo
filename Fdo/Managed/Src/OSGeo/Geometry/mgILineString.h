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

class FdoLineStringCollection;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
interface class IDirectPosition;
ref class DirectPositionCollection;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \interface OSGeo::FDO::Geometry::ILineString
/// \brief
/// The ILineString class is a LineString Geometry type.  
/// The shape of ILineString is the set of positions defined by the contained collection, 
/// plus linear interpolation between consecutive points.
public interface class ILineString : public NAMESPACE_OSGEO_GEOMETRY::ICurveAbstract
{
public:
    /// \brief
    /// Gets the number of positions in this object.
    /// 
    /// \return
    /// Returns the number of positions
    /// 
    property System::Int32 Count
    {
        System::Int32 get();
    }
	
    /// \brief
    /// Gets the position at the specified (zero-based) index.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of positions.
    /// 
    /// \return
    /// Returns the position
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ default[System::Int32]
    {
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ get(System::Int32 index);
    }

    /// \brief
    /// Gets the position at the specified (zero-based) index, by values of its member data.
    /// This is in aid of higher performance for any implementation that
    /// does not internally use FdoIDirectPosition objects for storage, or for
    /// an application seeking to avoid overhead of accessor methods.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of positions.
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
	virtual System::Void GetItemByMembers(
		System::Int32 index, 
		System::Double% coordinateX, 
		System::Double% coordinateY, 
		System::Double% coordinateZ, 
		System::Double% coordinateM, 
		System::Int32% dimensionality) abstract;
	
    /// \brief
    /// Gets a collection of all of the positions in this object.
    /// 
    /// \return
    /// Returns the positions
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ Positions
    {
        NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ get();
    }
};

/// \brief
///  The LineStringCollection class is a collection of LineString objects.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class LineStringCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionBase
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
	LineStringCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	FdoLineStringCollection* GetImpObj();

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
    ///  Creates an instance of LineStringCollection with no contained elements.
    /// 
    /// \return
    /// Returns an empty collection
    /// 
	LineStringCollection();

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
    /// Removes the index-th ILineString from this collection.
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
    /// Adds a ILineString object into the collection.
    /// 
    /// \param value 
    /// Input the ILineString object to add.
    /// 
    /// \return
    /// 		The position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_GEOMETRY::ILineString^ value);

    /// \brief
    /// Determines the index of a specific ILineString object.
    /// 
    /// \param value 
    /// Input the ILineString object to locate in the collection.
    /// 
    /// \return
    /// 		The index of value if found in the collection; otherwise, -1.
    /// 
    System::Int32 IndexOf(NAMESPACE_OSGEO_GEOMETRY::ILineString^ value);

    /// \brief
    /// Inserts an ILineString object to the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the ILineString object to insert.
    /// 
    System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILineString^ value); 

    /// \brief
    /// Removes the first occurrence of a specific ILineString object.
    /// 
    /// \param value 
    /// Input the ILineString object to remove from the collection.
    /// 
    System::Void Remove(NAMESPACE_OSGEO_GEOMETRY::ILineString^ value);

    /// \brief
    /// Determines whether the collection contains a specific ILineString object.
    /// 
    /// \param value 
    /// Input The ILineString object to locate in the collection.
    /// 
    /// \return
    /// 		True if the value is found in the collection; otherwise, false.
    /// 
    System::Boolean Contains(NAMESPACE_OSGEO_GEOMETRY::ILineString^ value);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::ILineString^>^ pArray, System::Int32 index);

    /// \brief
    /// Gets an ILineString object in the collection.
    /// 
    /// \param index 
    /// Input index of the ILineString object to retrieve.
    /// 
    /// \brief
    /// Sets the value of the ILineString object at the specified index
    /// 
    /// \param index 
    /// Input index of the ILineString object to set.
    /// 
    /// \param value 
    /// Input the value of the ILineString
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::ILineString^ Item[System::Int32]
    {
        NAMESPACE_OSGEO_GEOMETRY::ILineString^ get(System::Int32 index);
        System::Void set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILineString^ value);
    }
};

END_NAMESPACE_OSGEO_GEOMETRY


