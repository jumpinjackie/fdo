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

#include <Runtime\Disposable.h>
#include <mgCollectionBase.h>

class FdoXmlAttributeCollection;

BEGIN_NAMESPACE_OSGEO_COMMON_XML

ref class XmlAttribute;

/// \ingroup (OSGeoFDOCommonXml)
/// \brief
/// The XmlAttributeCollection class represents a collection of XmlAttribute objects.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class XmlAttributeCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionBase
{
/// \cond DOXYGEN-IGNORE
internal:
	inline FdoXmlAttributeCollection* GetImpObj();

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
    ///     Constructs a new empty attribute collection
    /// 
	XmlAttributeCollection();

    /// \brief
    ///     Constructs a collection based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    ///     Input A Pointer to the unmanaged collection.
    /// 
    /// \param autoDelete 
    ///     Input Indicates if the constructed object should be automatically deleted 
    ///     once it no longer referenced.
    /// 
	XmlAttributeCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

    /// \brief
    ///    Gets the count of items in collection.
    /// 
    /// \return
    /// 	Returns the number of items in the collection.
    /// 
	property System::Int32 Count
    {
        virtual System::Int32 get() override;
    }

    /// \brief
    ///     Removes the index-th XmlAttribute from this collection.
    /// 
    /// \param index 
    ///     Input index of the element to remove.
    /// 
	virtual System::Void RemoveAt(System::Int32 index) override;

    /// \brief
    ///     Removes all elements from the collection.
    /// 
	virtual System::Void  Clear() override;

    /// \brief
    ///     Adds a XmlAttribute object into the collection.
    /// 
    /// \param value 
    ///     Input the XmlAttribute object to add.
    /// 
    /// \return
    /// 		Returns the position into which the new element was inserted.
    /// 
	System::Int32 Add(XmlAttribute^ value);

    /// \brief
    ///     Determines the index of a specific XmlAttribute object.
    /// 
    /// \param value 
    ///     Input the XmlAttribute object to locate in the collection.
    /// 
    /// \return
    ///     The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(XmlAttribute^ value);

    /// \brief
    ///     Determines the index of a specific XmlAttribute object.
    /// 
    /// \param name 
    ///     Input the name of the XmlAttribute object to locate in the collection.
    /// 
    /// \return
    ///     The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(String^ name);

    /// \brief
    ///     Inserts a XmlAttribute object into the collection at the specified position.
    /// 
    /// \param index 
    ///     Input the zero-based index at which value should be inserted.
    ///
    /// \param value 
    ///     Input the XmlAttribute object to insert.
    /// 
	System::Void Insert(System::Int32 index, XmlAttribute^ value);

    /// \brief
    ///     Removes the first occurrence of a specific XmlAttribute object.
    /// 
    /// \param value 
    ///     Input the XmlAttribute object to remove from the collection.
    /// 
	System::Void Remove(XmlAttribute^ value);

    /// \brief
    ///     Determines whether the collection contains a specific XmlAttribute object.
    /// 
    /// \param value 
    ///     Input The XmlAttribute object to search in the collection.
    /// 
    /// \return
    ///     Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(XmlAttribute^ value);

    /// \brief
    ///     Determines whether the collection contains a specific XmlAttribute object.
    /// 
    /// \param name 
    ///     Input The name of the XmlAttribute object to search in the collection.
    /// 
    /// \return
    ///     Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(String^ name);

    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    ///     Output the one-dimensional Array that is the destination of the elements copied from this collection.
    ///
    /// \param index 
    ///     Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(array<XmlAttribute^>^ pArray,System::Int32 index);

    /// \brief
    ///     Gets an XmlAttribute in the collection.
    /// 
    /// \param index 
    ///     Input index of the XmlAttribute to retrieve.
    /// 
    /// \return 
    ///     An XmlAttribute found in the collection
    /// 
    /// \brief
    ///     Sets an XmlAttribute in the collection.
    /// 
    /// \param index 
    ///     Input index of the XmlAttribute to set.
    /// 
    /// \param value 
    ///     Input the value of the XmlAttribute
    /// 
    property XmlAttribute^ Item[System::Int32]
    {
        XmlAttribute^ get(System::Int32 index);
        System::Void set(System::Int32 index, XmlAttribute^ value);
    }
};

END_NAMESPACE_OSGEO_COMMON_XML


