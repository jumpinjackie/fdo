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

#pragma  once

class FdoProviderCollection;

BEGIN_NAMESPACE_OSGEO_FDO_CLIENTSERVICES
public __gc class Provider;
END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES

BEGIN_NAMESPACE_OSGEO_FDO_CLIENTSERVICES

/// \ingroup (OSGeoFDOClientServices)
/// \brief
/// Class ProviderCollection is a read-only collection consisting of the FDO Providers that are currently registered. 
/// Each collected Provider object describes an installed and registered FDO Feature Provider.
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class ProviderCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable, 
                                                public System::Collections::ICollection, 
                                                public System::Collections::IEnumerable
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
		ProviderCollection* m_pCol;
		System::Int32 m_nIdx;

	public:
        /// \brief
        /// Constructs a new Collection Enumerator
        /// 
        /// \param elements 
        /// Input The collection to enumerate.
        /// 
		Enumerator(ProviderCollection* elements) : m_pCol(elements), m_nIdx(-1) 
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
        /// Initially, the enumerator is positioned before the first element in the collection. 
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
    /// \brief
    /// Constructs a ProviderCollection managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	ProviderCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
		: Disposable(unmanaged, autoDelete)
	{	
	}

	inline FdoProviderCollection* GetImpObj();

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
    /// Gets the number of items in the collection
    /// 
    /// \return
    /// Returns an int containing the number of items.
    /// Throws an instance of Exception if an error occurs.
    /// 
	__property System::Int32 get_Count(System::Void);

    /// \brief
    /// Gets an enumerator that can iterate through a collection.
    /// 
    /// \return
    /// Returns an enumerator for the collection.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);
    
    /// \brief
    /// Determines if the collection contains the specified item
    /// 
    /// \param name 
    /// The unique name of the feature provider. This name should be of the form 
    /// [Company].[Provider].[Version].
    /// 
    /// \return
    /// Returns true if the collection contains the specified item, otherwise false.
    /// Throws an instance of Exception if an error occurs.
    /// 
	System::Boolean Contains(System::String* name);
    
    /// \brief
    /// Returns the index of the specified item in the collection.
    /// 
    /// \param name 
    /// The unique name of the feature provider. This name should be of the form 
    /// [Company].[Provider].[Version].
    /// 
    /// \return
    /// Returns the index item or -1 if the item does not exist. Index is 0 based.
    /// Throws an instance of Exception if an error occurs.
    /// 
	System::Int32 IndexOf(System::String* name);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    ///
    /// \param index 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(System::String* array[],System::Int32 index);

    /// \brief
    /// Gets the Provider item in the collection at the specified index. 
    /// 
    /// \param index 
    /// The index of the provider in the collection. The index is 0 based.
    /// 
    /// \return
    /// Returns an instance of a Provider.
    /// Throws an instance of Exception if the index is out of range or an error occurs.
    /// 
	__property NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider *get_RealTypeItem(System::Int32 index);
};

END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES


