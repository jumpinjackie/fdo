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

/// \brief
/// The FdoProviderCollection class represents a collection of Provider string.
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class ProviderCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::ICollection, public System::Collections::IEnumerable
{
private:
    /// \brief
    /// 		nested class defined to provide Enumerator.
    /// 
	__gc class Enumerator : public System::Collections::IEnumerator
	{
	private:
		ProviderCollection* m_pCol;
		System::Int32 m_nIdx;

	public:
		Enumerator(ProviderCollection* elements) : m_pCol(elements), m_nIdx(-1) 
		{

		}

		__property System::Object* get_Current();

		System::Boolean MoveNext();

		System::Void Reset();
	};

public private:
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
	System::Void Dispose(System::Boolean disposing);

public:
    /// \brief
    /// Gets the number of items in the collection
    /// 
    /// \return
    /// Returns an int containing the number of items.
    /// Throws an instance of FdoException * if an error occurs.
    /// 
	__property System::Int32 get_Count(System::Void);

    /// \brief
    ///    Gets an enumerator that can iterate through a collection.
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
    /// Throws an instance of FdoException * if an error occurs.
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
    /// Throws an instance of FdoException * if an error occurs.
    /// 
	System::Int32 IndexOf(System::String* name);

    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(System::String* array[],System::Int32 size);

    /// \brief
    /// Gets the Provider item in the collection at the specified index. 
    /// 
    /// \param index 
    /// The index of the provider in the collection. The index is 0 based.
    /// 
    /// \return
    /// Returns an instance of an FdoProvider.
    /// Throws an instance of FdoException * if the index is out of range or an error occurs.
    /// 
	__property NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider *get_RealTypeItem(System::Int32 index);
};

END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES


