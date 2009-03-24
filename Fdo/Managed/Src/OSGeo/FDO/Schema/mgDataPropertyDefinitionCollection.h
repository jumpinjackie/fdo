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

class FdoDataPropertyDefinitionCollection;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
ref class DataPropertyDefinition;
ref class SchemaElement;

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// The DataPropertyDefinitionCollection class represents a collection of DataPropertyDefinition objects.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class DataPropertyDefinitionCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionBase
{
/// \cond DOXYGEN-IGNORE
internal:
	inline FdoDataPropertyDefinitionCollection* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

private:
    virtual property System::Object^ IndexInternal[System::Int32]
    {
        System::Object^ get(System::Int32 index) sealed = IList::default::get;
        void set(System::Int32 index, System::Object^ value) sealed = IList::default::set;
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
    /// Constructs a DataPropertyDefinitionCollection object
    /// 
    /// \param parent 
    /// Input A Pointer to the parent schema object of the collection
    /// 
	DataPropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ parent);

    /// \brief
    /// Constructs a DataPropertyDefinitionCollection object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
    DataPropertyDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
	{

	}

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
    /// Removes the index-th DataPropertyDefinition from this collection.
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
    /// Adds a DataPropertyDefinition object into the collection.
    /// 
    /// \param value 
    /// Input the DataPropertyDefinition object to add.
    /// 
    /// \return
    /// Returns the position into which the new element was inserted.
    /// 
	System::Int32 Add(DataPropertyDefinition^ value);

    /// \brief
    /// Determines the index of a specific DataPropertyDefinition object.
    /// 
    /// \param value 
    /// Input the DataPropertyDefinition object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(DataPropertyDefinition^ value);

	System::Int32 IndexOf(String^ name);

    /// \brief
    /// Inserts a DataPropertyDefinition object into the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the DataPropertyDefinition object to insert.
    /// 
	System::Void Insert(System::Int32 index, DataPropertyDefinition^ value);

    /// \brief
    /// Removes the first occurrence of a specific DataPropertyDefinition object.
    /// 
    /// \param value 
    /// Input the DataPropertyDefinition object to remove from the collection.
    /// 
	System::Void Remove(DataPropertyDefinition^ value);

    /// \brief
    /// Determines whether the collection contains a specific DataPropertyDefinition object.
    /// 
    /// \param value 
    /// Input The DataPropertyDefinition object to search in the collection.
    /// 
    /// \return
    /// Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(DataPropertyDefinition^ value);

    /// \brief
    /// Determines whether the collection contains a specific DataPropertyDefinition object.
    /// 
    /// \param name 
    /// Input The name of the DataPropertyDefinition object to search in the collection.
    /// 
    /// \return
    /// Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(String^ name);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
    System::Void CopyTo(array<DataPropertyDefinition^>^ pArray, System::Int32 index);

    /// \brief
    /// Gets the item in the collection by name. 
    /// 
    /// \param index 
    /// The name of the item in the collection.
    /// 
    /// \return
    /// Returns an instance of a the collected item.
    /// 
    property DataPropertyDefinition^ Item[System::String^]
    {
        DataPropertyDefinition^ get(System::String^ index);
    }

    /// \brief
    /// Gets the item in the collection at the specified index. 
    /// 
    /// \param index 
    /// The index of the item in the collection. The index is 0 based.
    /// 
    /// \return
    /// Returns an instance of a the collected item.
    /// Throws an instance of Exception if the index is out of range or an error occurs.
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
    property DataPropertyDefinition^ Item[System::Int32]
    {
        DataPropertyDefinition^ get(System::Int32 index);
        System::Void set(System::Int32 index, DataPropertyDefinition^ value);
    }
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


