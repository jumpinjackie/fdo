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

class FdoReadOnlyDataPropertyDefinitionCollection;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
ref class DataPropertyDefinition;
ref class DataPropertyDefinitionCollection;

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// The ReadOnlyDataPropertyDefinitionCollection class represents a collection of DataPropertyDefinition objects.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class ReadOnlyDataPropertyDefinitionCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionReadOnlyBase
{
/// \cond DOXYGEN-IGNORE
internal:
	inline FdoReadOnlyDataPropertyDefinitionCollection* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

private:
    virtual property System::Object^ IndexInternal[System::Int32]
    {
        private: System::Object^ get(System::Int32 index) sealed = NAMESPACE_OSGEO_COMMON::IListReadOnly::default::get;
    }

/// \endcond

public:
    /// \brief
    /// Constructs a ReadOnlyDataPropertyDefinitionCollection object
    /// 
    /// \param parent 
    /// Input A Pointer to the parent schema object of the collection
    /// 
	ReadOnlyDataPropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection^ parent);

    /// \brief
    /// Constructs a ReadOnlyDataPropertyDefinitionCollection object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	ReadOnlyDataPropertyDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionReadOnlyBase(unmanaged, autoDelete)
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
    /// Determines the index of a specific DataPropertyDefinition object.
    /// 
    /// \param value 
    /// Input the DataPropertyDefinition object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ value);

    /// \brief
    /// Determines whether the collection contains a specific DataPropertyDefinition object.
    /// 
    /// \param value 
    /// Input The DataPropertyDefinition object to search in the collection.
    /// 
    /// \return
    /// Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ value);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(array<NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^>^ pArray, System::Int32 index);

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
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ Item[System::Int32]
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ get(System::Int32 index);
    }

    /// \brief
    /// Gets the DataPropertyDefinition item in the collection at the specified index. 
    /// 
    /// \param name 
    /// The name of the DataPropertyDefinition item in the collection.
    /// 
    /// \return
    /// Returns an instance of a the collected item.
    /// Throws an instance of Exception if the index is out of range or an error occurs.
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ Item[System::String^]
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ get(System::String^ name);
    }
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


