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
ref class Provider;
END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES

BEGIN_NAMESPACE_OSGEO_FDO_CLIENTSERVICES

/// \ingroup (OSGeoFDOClientServices)
/// \brief
/// Class ProviderCollection is a read-only collection consisting of the FDO Providers that are currently registered. 
/// Each collected Provider object describes an installed and registered FDO Feature Provider.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class ProviderCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionReadOnlyBase
{
/// \cond DOXYGEN-IGNORE
internal:
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
		: NAMESPACE_OSGEO_COMMON::CollectionReadOnlyBase(unmanaged, autoDelete)
	{	
	}

	inline FdoProviderCollection* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

private:
    virtual property System::Object^ IndexInternal[System::Int32]
    {
        private: System::Object^ get(System::Int32 index) sealed = NAMESPACE_OSGEO_COMMON::IListReadOnly::default::get;
    }

protected:
	virtual System::Void ReleaseUnmanagedObject() override;

/// \endcond

public:
    /// \brief
    /// Gets the number of items in the collection
    /// 
    /// \return
    /// Returns an int containing the number of items.
    /// Throws an instance of Exception if an error occurs.
    /// 
    virtual property System::Int32 Count
    {
        virtual System::Int32 get() override;
    }

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
	System::Boolean Contains(System::String^ name);
    
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
	System::Int32 IndexOf(System::String^ name);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    ///
    /// \param index 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(array<System::String^>^ pArray, System::Int32 index);

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
    property NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider^ Item[System::Int32]
    {
        NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider^ get(System::Int32 index);
    }
};

END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES


