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

class FdoStringCollection;

BEGIN_NAMESPACE_OSGEO_COMMON
ref class StringElement;

/// \ingroup (OSGeoFDOCommon)
/// \brief
///     StringCollection is a collection of name-value pairs.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class StringCollection sealed  : public NAMESPACE_OSGEO_COMMON::CollectionBase
{
/// \cond DOXYGEN-IGNORE
internal:
	inline FdoStringCollection* GetImpObj();

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
    ///     Constructs a new empty string collection
    /// 
	StringCollection();

    /// \brief
    ///     Creates a copy of string collection
    /// 
    /// \param collection 
    ///     Input the source collection
    /// 
	StringCollection(StringCollection^ collection);

    /// \brief
    ///     Creates a string collection that is tokenized from a string.
    ///     The collection contains an element for each token.
    /// 
    /// \param data 
    ///     Input the string to tokenize.
    ///
    /// \param delimiters 
    ///     Input list of single character token delimiters.
    ///
	StringCollection(String^ data, String^ delimiters);

    /// \brief
    ///     Creates a string collection that is tokenized from a string.
    ///     The collection contains an element for each token.
    /// 
    /// \param data 
    ///     Input the string to tokenize.
    ///
    /// \param delimiters 
    ///     Input list of single character token delimiters.
    ///
    /// \param nullTokens 
    ///     true: include zero-length tokens in the collection.
    ///     false: exclude zero-length tokens
    /// 
	StringCollection(String^ data, String^ delimiters, Boolean nullTokens);

    /// \brief
    ///     Constructs a String Collection based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    ///     Input A Pointer to the unmanaged collection.
    /// 
    /// \param autoDelete 
    ///     Input Indicates if the constructed collection should be automatically deleted 
    ///     once it no longer referenced.
    /// 
	StringCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

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
    ///     Removes the index-th StringElement from this collection.
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
    ///     Adds a StringElement object into the collection.
    /// 
    /// \param value 
    ///     Input the StringElement object to add.
    /// 
    /// \return
    /// 	Returns the position into which the new element was inserted.
    /// 
	System::Int32 Add(StringElement^ value);

    /// \brief
    ///     Appends the strings from src to the end of this collection.
    /// 
    /// \param collection 
    ///     Input the source collection
    /// 
	System::Void Append(StringCollection^ collection);

    /// \brief
    ///     Gets a String in the collection.
    /// 
    /// \param index 
    ///     Input index of the String to retrieve.
    /// 
    /// \return
    /// 	Returns the requested String.
    /// 
    property System::String^ String[System::Int32]
    {
        System::String^ get(System::Int32 index);
    }

    /// \brief
    ///     Determines the index of a specific StringElement object.
    /// 
    /// \param value 
    ///     Input the StringElement object to locate in the collection.
    /// 
    /// \return
    ///     The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(StringElement^ value);

    /// \brief
    ///     Inserts a StringElement object into the collection at the specified position.
    /// 
    /// \param index 
    ///     Input the zero-based index at which value should be inserted.
    ///
    /// \param value 
    ///     Input the StringElement object to insert.
    /// 
	System::Void Insert(System::Int32 index, StringElement^ value);

    /// \brief
    ///     Removes the first occurrence of a specific StringElement object.
    /// 
    /// \param value 
    ///     Input the StringElement object to remove from the collection.
    /// 
	System::Void Remove(StringElement^ value);

    /// \brief
    ///     Determines whether the collection contains a specific StringElement object.
    /// 
    /// \param value 
    ///     Input The StringElement object to search in the collection.
    /// 
    /// \return
    ///     Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(StringElement^ value);

    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    ///     Output the one-dimensional Array that is the destination of the elements copied from this collection.
    ///
    /// \param index 
    ///     Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(array<StringElement^>^ pArray, System::Int32 index);

    /// \brief
    ///     Gets a StringElement in the collection.
    /// 
    /// \param index 
    ///     Input index of the StringElement to retrieve.
    /// 
    /// \return
    ///     Returns the StringElement at the specified index
    /// 
    /// \brief
    ///     Sets the value of the StringElement at the specified index
    /// 
    /// \param index 
    ///     Input index of the StringElement to set.
    /// 
    /// \param value 
    ///     Input the value of the StringElement
    /// 
    property StringElement^ Item[System::Int32]
    {
        StringElement^ get(System::Int32 index);
        System::Void set(System::Int32 index, StringElement^ value);
    }
};

END_NAMESPACE_OSGEO_COMMON


