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

BEGIN_NAMESPACE_OSGEO_COMMON
ref class Disposable;

public ref class CollectionBase abstract : 
    public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::IList
{
/// \cond DOXYGEN-IGNORE
internal:
    /// \brief
    /// 	A Nested class defined to provide enumeration of Dictionary elements
    ///
    ///     Enumerators can be used to read the data in the collection, 
    ///     but they cannot be used to modify the underlying collection.
    ///
    ///     An enumerator remains valid as long as the collection remains unchanged. 
    ///     If changes are made to the collection, such as adding, modifying, or deleting 
    ///     elements, the enumerator is irrecoverably invalidated and the next call to 
    ///     MoveNext or Reset throws an InvalidOperationException. If the collection is 
    ///     modified between MoveNext and Current, Current returns the element that it is 
    ///     set to, even if the enumerator is already invalidated.
    ///
    ///     The enumerator does not have exclusive access to the collection; therefore, 
    ///     enumerating through a collection is intrinsically not a thread-safe procedure. 
    ///     Even when a collection is synchronized, other threads can still modify the 
    ///     collection, which causes the enumerator to throw an exception. To guarantee 
    ///     thread safety during enumeration, you can either lock the collection during 
    ///     the entire enumeration or catch the exceptions resulting from changes made 
    ///     by other threads.
    /// 
	ref class Enumerator : public System::Collections::IEnumerator	
    {
	private:
		System::Collections::IList^ m_pCol;
		System::Int32 m_nIdx;

	public:
        /// \brief
        ///     Constructs a new dictionary Enumerator
        /// 
        /// \param elements 
        ///     Input The Dictionary collection to enumerate.
        /// 
		Enumerator(System::Collections::IList^ elements) : m_pCol(elements), m_nIdx(-1) 
		{
		}

        /// \brief
        ///     Retrieves the current dictionay element at the enumerator location
        /// 
        /// \return
        /// 	Retuns the current dictionary element referenced by the enumerator
        /// 
        virtual property System::Object^ Current
        {
            System::Object^ get()
            {
	            if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	            {
		            throw gcnew InvalidOperationException();
	            }
                return m_pCol[m_nIdx];
            }
        }
		
        /// \brief
        ///     Initially, the enumerator is positioned before the first element in the collection. 
        ///     At this position, calling the Current property throws an exception. 
        ///     Therefore, you must call the MoveNext method to advance the enumerator 
        ///     to the first element of the collection before reading the value of Current.
        //      If MoveNext passes the end of the collection, the enumerator is positioned 
        ///     after the last element in the collection and MoveNext returns false. 
        ///     When the enumerator is at this position, subsequent calls to MoveNext also return false. 
        ///     If the last call to MoveNext returned false, calling Current throws an exception. 
        //      To set Current to the first element of the collection again, you can call Reset 
        ///     followed by MoveNext.
        /// 
        /// \return
        /// 	Retuns true if the enumerator is able to move to a valid dictionary element
        ///     otherwise false.
        /// 
		virtual System::Boolean MoveNext()
        {
	        ++m_nIdx;
	        return m_nIdx < m_pCol->Count;
        }
		
        /// \brief
        ///     Initially, the enumerator is positioned before the first element in the collection. 
        ///     The Reset method brings the enumerator back to this position. 
        /// 
		virtual System::Void Reset()
        {
        	m_nIdx = -1;
        }
	};


    
private:
    virtual property System::Object^ IndexInternalBase[System::Int32]
    {
        private: System::Object^ get(System::Int32 index) sealed = IList::default::get
        { 
            throw gcnew NotImplementedException(); 
        }
        private: void set(System::Int32 index, System::Object^ value) sealed = IList::default::set 
        { 
            throw gcnew NotImplementedException(); 
        }
    }
    // System::Collections::IList interface methods
    virtual System::Int32 Add(System::Object^ value) sealed = IList::Add
    {
        throw gcnew NotImplementedException(); 
    }

    virtual System::Void Insert(System::Int32 index, System::Object^ value) sealed = IList::Insert
    {
        throw gcnew NotImplementedException(); 
    }

    virtual System::Boolean Contains(System::Object^ value) sealed = IList::Contains
    {
        throw gcnew NotImplementedException(); 
    }

    virtual System::Int32 IndexOf(System::Object^ value) sealed = IList::IndexOf
    {
        throw gcnew NotImplementedException(); 
    }

    virtual System::Void Remove(System::Object^ value) sealed = IList::Remove
    {
        throw gcnew NotImplementedException(); 
    }

/// \endcond

public:
    // System::Collections::ICollection interface properties
    virtual property System::Object^ SyncRoot
    {
        System::Object^ get()
        {
            return nullptr;
        }
    }
    virtual property System::Boolean IsSynchronized
    {
        System::Boolean get() 
        {
            return false;
        }
    }
    
    virtual property System::Boolean IsFixedSize
    {
        System::Boolean get() 
        {
            return false;
        }
    }
    virtual property System::Boolean IsReadOnly
    {
        System::Boolean get() 
        {
            return false;
        }
    }
    
    /// \brief
    ///    Gets an enumerator that can iterate through a collection.
    /// 
    /// \return
    /// 	Returns an enumerator on the dictionary.
    /// 
	virtual System::Collections::IEnumerator^ GetEnumerator(System::Void) sealed
    {
        return gcnew Enumerator(this);
    }

    virtual System::Void CopyTo(System::Array^ pArray, System::Int32 index) sealed
    {
	    if (nullptr == pArray)
		    throw gcnew System::ArgumentNullException();
	    if (index < 0)
		    throw gcnew System::ArgumentOutOfRangeException();
	    if (pArray->Rank != 1 || index >= pArray->Length || this->Count + index > pArray->Length)
		    throw gcnew System::ArgumentException();

        System::Collections::IList^ lst = this;
	    for (System::Int32 i = 0; i < this->Count; i++)
		    pArray->SetValue(lst[i], index + i);
    }
    virtual System::Void RemoveAt(System::Int32 index) abstract;
    virtual System::Void  Clear() abstract;
    virtual property System::Int32 Count
    {
        virtual System::Int32 get() abstract;
    }
public:
    /// \brief
    ///     Constructs a dictionary object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    ///     Input A Pointer to the unmanaged collection.
    /// 
    /// \param autoDelete 
    ///     Input Indicates if the constructed object should be automatically deleted 
    ///     once it no longer referenced.
    /// 
    CollectionBase(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
    {
    }
};


public interface class IListReadOnly
{
    property System::Object^ default[System::Int32]
    {
        System::Object^ get(System::Int32 index);
    }
    property System::Int32 Count 
    {
        System::Int32 get(); 
    }
};

public ref class CollectionReadOnlyBase abstract : 
    public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::ICollection, public System::Collections::IEnumerable , public NAMESPACE_OSGEO_COMMON::IListReadOnly
{
/// \cond DOXYGEN-IGNORE
internal:
    /// \brief
    /// 	A Nested class defined to provide enumeration of Dictionary elements
    ///
    ///     Enumerators can be used to read the data in the collection, 
    ///     but they cannot be used to modify the underlying collection.
    ///
    ///     An enumerator remains valid as long as the collection remains unchanged. 
    ///     If changes are made to the collection, such as adding, modifying, or deleting 
    ///     elements, the enumerator is irrecoverably invalidated and the next call to 
    ///     MoveNext or Reset throws an InvalidOperationException. If the collection is 
    ///     modified between MoveNext and Current, Current returns the element that it is 
    ///     set to, even if the enumerator is already invalidated.
    ///
    ///     The enumerator does not have exclusive access to the collection; therefore, 
    ///     enumerating through a collection is intrinsically not a thread-safe procedure. 
    ///     Even when a collection is synchronized, other threads can still modify the 
    ///     collection, which causes the enumerator to throw an exception. To guarantee 
    ///     thread safety during enumeration, you can either lock the collection during 
    ///     the entire enumeration or catch the exceptions resulting from changes made 
    ///     by other threads.
    /// 
	ref class Enumerator : public System::Collections::IEnumerator	
    {
	private:
		NAMESPACE_OSGEO_COMMON::IListReadOnly^ m_pCol;
		System::Int32 m_nIdx;

	public:
        /// \brief
        ///     Constructs a new dictionary Enumerator
        /// 
        /// \param elements 
        ///     Input The Dictionary collection to enumerate.
        /// 
		Enumerator(NAMESPACE_OSGEO_COMMON::IListReadOnly^ elements) : m_pCol(elements), m_nIdx(-1) 
		{
		}

        /// \brief
        ///     Retrieves the current dictionay element at the enumerator location
        /// 
        /// \return
        /// 	Retuns the current dictionary element referenced by the enumerator
        /// 
        virtual property System::Object^ Current
        {
            System::Object^ get()
            {
	            if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	            {
		            throw gcnew InvalidOperationException();
	            }
                return m_pCol[m_nIdx];
            }
        }
		
        /// \brief
        ///     Initially, the enumerator is positioned before the first element in the collection. 
        ///     At this position, calling the Current property throws an exception. 
        ///     Therefore, you must call the MoveNext method to advance the enumerator 
        ///     to the first element of the collection before reading the value of Current.
        //      If MoveNext passes the end of the collection, the enumerator is positioned 
        ///     after the last element in the collection and MoveNext returns false. 
        ///     When the enumerator is at this position, subsequent calls to MoveNext also return false. 
        ///     If the last call to MoveNext returned false, calling Current throws an exception. 
        //      To set Current to the first element of the collection again, you can call Reset 
        ///     followed by MoveNext.
        /// 
        /// \return
        /// 	Retuns true if the enumerator is able to move to a valid dictionary element
        ///     otherwise false.
        /// 
		virtual System::Boolean MoveNext()
        {
	        ++m_nIdx;
	        return m_nIdx < m_pCol->Count;
        }
		
        /// \brief
        ///     Initially, the enumerator is positioned before the first element in the collection. 
        ///     The Reset method brings the enumerator back to this position. 
        /// 
		virtual System::Void Reset()
        {
        	m_nIdx = -1;
        }
	};

private:
    virtual property System::Object^ IndexInternalBase[System::Int32]
    {
        private: System::Object^ get(System::Int32 index) sealed = IListReadOnly::default::get
        { 
            throw gcnew NotImplementedException(); 
        }
    }

/// \endcond

public:
    // System::Collections::ICollection interface properties
    virtual property System::Object^ SyncRoot
    {
        System::Object^ get()
        {
            return nullptr;
        }
    }
    virtual property System::Boolean IsSynchronized
    {
        System::Boolean get() 
        {
            return false;
        }
    }
    
    /// \brief
    ///    Gets an enumerator that can iterate through a collection.
    /// 
    /// \return
    /// 	Returns an enumerator on the dictionary.
    /// 
	virtual System::Collections::IEnumerator^ GetEnumerator(System::Void) sealed
    {
        return gcnew Enumerator(this);
    }

    virtual System::Void CopyTo(System::Array^ pArray, System::Int32 index) sealed
    {
	    if (nullptr == pArray)
		    throw gcnew System::ArgumentNullException();
	    if (index < 0)
		    throw gcnew System::ArgumentOutOfRangeException();
	    if (pArray->Rank != 1 || index >= pArray->Length || this->Count + index > pArray->Length)
		    throw gcnew System::ArgumentException();

        NAMESPACE_OSGEO_COMMON::IListReadOnly^ coll = this;
	    for (System::Int32 i = 0; i < this->Count; i++)
		    pArray->SetValue(coll[i], index + i);
    }

    virtual property System::Int32 Count
    {
        virtual System::Int32 get() abstract;
    }

public:
    /// \brief
    ///     Constructs a dictionary object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    ///     Input A Pointer to the unmanaged collection.
    /// 
    /// \param autoDelete 
    ///     Input Indicates if the constructed object should be automatically deleted 
    ///     once it no longer referenced.
    /// 
    CollectionReadOnlyBase(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
    {
    }
};


END_NAMESPACE_OSGEO_COMMON

