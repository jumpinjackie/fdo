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
public __gc class DataPropertyDefinition;
public __gc class DataPropertyDefinitionCollection;

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// The ReadOnlyDataPropertyDefinitionCollection class represents a collection of DataPropertyDefinition objects.
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class ReadOnlyDataPropertyDefinitionCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::ICollection, public System::Collections::IEnumerable
{
/// \cond DOXYGEN-IGNORE
private:
    /// \brief
    /// A Nested class defined to provide enumeration of Dictionary elements
    ///
    /// Enumerators can be used to read the data in the collection, 
    /// but they cannot be used to modify the underlying collection.
    ///
    /// An enumerator remains valid as long as the collection remains unchanged. 
    /// If changes are made to the collection, such as adding, modifying, or deleting 
    /// elements, the enumerator is irrecoverably invalidated and the next call to 
    /// MoveNext or Reset throws an InvalidOperationException. If the collection is 
    /// modified between MoveNext and Current, Current returns the element that it is 
    /// set to, even if the enumerator is already invalidated.
    ///
    /// The enumerator does not have exclusive access to the collection; therefore, 
    /// enumerating through a collection is intrinsically not a thread-safe procedure. 
    /// Even when a collection is synchronized, other threads can still modify the 
    /// collection, which causes the enumerator to throw an exception. To guarantee 
    /// thread safety during enumeration, you can either lock the collection during 
    /// the entire enumeration or catch the exceptions resulting from changes made 
    /// by other threads.
    /// 
	__gc class Enumerator : public System::Collections::IEnumerator
	{
	private:
		ReadOnlyDataPropertyDefinitionCollection* m_pCol;
		System::Int32 m_nIdx;

	public:
        /// \brief
        /// Constructs a new Collection Enumerator
        /// 
        /// \param col 
        /// Input The collection to enumerate.
        /// 
		Enumerator(ReadOnlyDataPropertyDefinitionCollection* elements) : m_pCol(elements), m_nIdx(-1) 
		{

		}

        /// \brief
        /// Retrieves the current object at the enumerator location
        /// 
        /// \return
        /// Retuns the current object referenced by the enumerator
        /// 
		__property System::Object *get_Current();

        /// \brief
        /// Initially, the enumerator is positioned before the first object in the collection. 
        /// At this position, calling the Current property throws an exception. 
        /// Therefore, you must call the MoveNext method to advance the enumerator 
        /// to the first element of the collection before reading the value of Current.
        /// If MoveNext passes the end of the collection, the enumerator is positioned 
        /// after the last element in the collection and MoveNext returns false. 
        /// When the enumerator is at this position, subsequent calls to MoveNext also return false. 
        /// If the last call to MoveNext returned false, calling Current throws an exception. 
        /// To set Current to the first element of the collection again, you can call Reset 
        /// followed by MoveNext.
        /// 
        /// \return
        /// Retuns true if the Enumerator is able to move to a valid element
        /// otherwise false.
        /// 
		System::Boolean MoveNext();

        /// \brief
        /// Initially, the enumerator is positioned before the first element in the collection. 
        /// The Reset method brings the enumerator back to this position. 
        /// 
		System::Void Reset();
	};

public private:
	inline FdoReadOnlyDataPropertyDefinitionCollection* GetImpObj();

private:
    // System::Collections::ICollection interfaces
	System::Void System::Collections::ICollection::CopyTo(System::Array* array,System::Int32 index);

	__property System::Object* System::Collections::ICollection::get_SyncRoot();

	__property System::Boolean System::Collections::ICollection::get_IsSynchronized();

protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public:
    /// \brief
    /// Constructs a ReadOnlyDataPropertyDefinitionCollection object
    /// 
    /// \param parent 
    /// Input A Pointer to the parent schema object of the collection
    /// 
	ReadOnlyDataPropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection* parent);

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
	ReadOnlyDataPropertyDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

    /// \brief
    /// Gets the count of items in collection.
    /// 
    /// \return
    /// Returns the number of items in the collection.
    /// 
	__property System::Int32 get_Count(System::Void);

    /// \brief
    /// Gets an enumerator that can iterate through a collection.
    /// 
    /// \return
    /// Returns an enumerator on the dictionary.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

    /// \brief
    /// Determines the index of a specific DataPropertyDefinition object.
    /// 
    /// \param value 
    /// Input the DataPropertyDefinition object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition* value);

    /// \brief
    /// Determines whether the collection contains a specific DataPropertyDefinition object.
    /// 
    /// \param value 
    /// Input The DataPropertyDefinition object to search in the collection.
    /// 
    /// \return
    /// Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition* value);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition* array[],System::Int32 startAt);

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
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition* get_RealTypeItem(System::Int32 index);

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
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition* get_RealTypeItem(System::String* name);
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


