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

class FdoDirectPositionCollection;

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \interface OSGeo::FDO::Geometry::IDirectPosition
/// \brief
/// The IDirectPosition class is a direct position object.  
/// IDirectPosition is a simple coordinate position in any allowed dimension.  
/// The actual object may have several implementations, depending on
/// the dimension and other factors.
/// This is a lightweight helper type for Geometries in the Geometry package.
/// Note: It does not derive from IGeometry.
public interface class IDirectPosition : public System::IDisposable
{
public:

    /// \brief
    /// Gets the X ordinate.
    /// 
    /// \return
    /// Returns the X ordinate; default is numeric_limits::quiet_NaN()
    /// 
    property System::Double X
    {
        System::Double get();
    }

    /// \brief
    /// Gets the Y ordinate.
    /// 
    /// \return
    /// Returns the Y ordinate; default is numeric_limits::quiet_NaN()
    /// 
    property System::Double Y
    {
        System::Double get();
    }

    /// \brief
    /// Gets the Z ordinate.
    /// 
    /// \return
    /// Returns the Z ordinate; default is numeric_limits::quiet_NaN()
    /// 
    property System::Double Z
    {
        System::Double get();
    }

    /// \brief
    /// Gets the M ordinate.
    /// 
    /// \return
    /// Returns the M ordinate; default is numeric_limits::quiet_NaN()
    /// 
    property System::Double M
    {
        System::Double get();
    }

    /// \brief
    /// Gets the dimensionality of ordinates in this position.
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
/// The DirectPositionCollection class is a collection of DirectPosition objects.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class DirectPositionCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionBase
{
public:
    /// \brief
    /// Creates an instance of DirectPositionCollection with no contained elements.
    /// 
    /// \return
    /// Returns an empty collection
    /// 
	DirectPositionCollection();

    /// \brief
    /// Constructs a DirectPositionCollection managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	DirectPositionCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	FdoDirectPositionCollection *GetImpObj();

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
    /// Removes the index-th IDirectPosition from this collection.
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
    /// Adds a IDirectPosition object into the collection.
    /// 
    /// \param value 
    /// Input the IDirectPosition object to add.
    /// 
    /// \return
    /// The position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value);

    /// \brief
    /// Determines the index of a specific IDirectPosition object.
    /// 
    /// \param value 
    /// Input the IDirectPosition object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
    System::Int32 IndexOf(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value);

    /// \brief
    /// Inserts an IDirectPosition object to the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    ///
    /// \param value 
    /// Input the IDirectPosition object to insert.
    /// 
    System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value); 

    /// \brief
    /// Removes the first occurrence of a specific IDirectPosition object.
    /// 
    /// \param value 
    /// Input the IDirectPosition object to remove from the collection.
    /// 
    System::Void Remove(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value);

    /// \brief
    /// Determines whether the collection contains a specific IDirectPosition object.
    /// 
    /// \param value 
    /// Input The IDirectPosition object to locate in the collection.
    /// 
    /// \return
    /// True if the value is found in the collection; otherwise, false.
    /// 
    System::Boolean Contains(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    ///
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
    System::Void CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^>^ pArray,System::Int32 startAt);

    /// \brief
    /// Gets an IDirectPosition in the collection.
    /// 
    /// \param index 
    /// Input index of the IDirectPosition to retrieve.
    /// 
    /// \brief
    /// Sets the value of the IDirectPosition at the specified index
    /// 
    /// \param index 
    /// Input index of the IDirectPosition to set.
    /// 
    /// \param value 
    /// Input the value of the IDirectPosition
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ Item[System::Int32]
    {
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ get(System::Int32 index);
        System::Void set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value);
    }
};

END_NAMESPACE_OSGEO_GEOMETRY


