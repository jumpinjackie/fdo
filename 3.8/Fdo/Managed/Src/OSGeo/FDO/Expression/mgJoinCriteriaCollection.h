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

class FdoJoinCriteriaCollection;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
ref class JoinCriteria;

/// \ingroup (OSGeoFDOCommands)
/// \brief
/// The JoinCriteriaCollection class represents a collection of FDO JoinCriteria objects.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class JoinCriteriaCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionBase
{
/// \cond DOXYGEN-IGNORE
internal:
    JoinCriteriaCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
	{

	}

	inline FdoJoinCriteriaCollection* GetImpObj();
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
    /// Constructs a new empty string collection
    /// 
	JoinCriteriaCollection();

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
    /// Removes the index-th JoinCriteria from this collection.
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
    /// Adds a JoinCriteria object into the collection.
    /// 
    /// \param value 
    /// Input the JoinCriteria object to add.
    /// 
    /// \return
    /// Returns the position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^ value);

    /// \brief
    /// Determines the index of a specific JoinCriteria object.
    /// 
    /// \param value 
    /// Input the JoinCriteria object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^ value);

    /// \brief
    /// Inserts a JoinCriteria object into the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the JoinCriteria object to insert.
    /// 
	System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^ value);

    /// \brief
    /// Removes the first occurrence of a specific JoinCriteria object.
    /// 
    /// \param value 
    /// Input the JoinCriteria object to remove from the collection.
    /// 
	System::Void Remove(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^ value);

    /// \brief
    /// Determines whether the collection contains a specific JoinCriteria object.
    /// 
    /// \param value 
    /// Input The JoinCriteria object to search in the collection.
    /// 
    /// \return
    /// Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^ value);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    ///
    /// \param index 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(array<NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^>^ pArray, System::Int32 index);

    /// \brief
    /// Gets or sets a JoinCriteria in the collection.
    /// 
    /// \param index 
    /// Input index of the JoinCriteria to retrieve or set (System::Int32).
    /// 
    /// \brief
    /// Sets the value of the item at the specified index
    /// 
    /// \param index 
    /// Input index of the item to set.
    /// 
    /// \param value 
    /// Input the value of the item
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^ Item[System::Int32]
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^ get(System::Int32 index);
        System::Void set(System::Int32 index, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^ value);
    }

    /// \brief
    /// Gets the item in the collection with the specified name. Throws an invalid 
    /// argument exception if an item with the specified name does not exist in 
    /// the collection.
    /// 
    /// \param name 
    /// Input the item name
    /// 
    /// \return
    /// Returns JoinCriteria
    /// 
	NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^ GetItem(System::String^ name);

    /// \brief
    /// Finds the item in the collection with the specified name. Returns nullptr 
    /// if an item with the specified name does not exist in the collection.
    /// 
    /// \param name 
    /// Input the item name
    /// 
    /// \return
    /// Returns JoinCriteria
    /// 
	NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^ FindItem(System::String^ name);

    /// \brief
    /// Gets the item in the collection at the specified index. Throws an
    /// invalid argument exception if the index is out of range.
    /// 
    /// \param index 
    /// Input index of item
    /// 
    /// \return
    /// Returns JoinCriteria
    /// 
	NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria^ GetItem(System::Int32 index);
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION
