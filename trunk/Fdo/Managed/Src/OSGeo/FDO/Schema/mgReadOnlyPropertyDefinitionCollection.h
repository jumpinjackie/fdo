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

class FdoReadOnlyPropertyDefinitionCollection;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class PropertyDefinition;
public __gc class PropertyDefinitionCollection;

/// \brief
/// The FdoReadOnlyPropertyDefinitionCollection class represents a collection of FdoPropertyDefinition objects.
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class ReadOnlyPropertyDefinitionCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::ICollection, public System::Collections::IEnumerable
{
private:
    /// \brief
    /// 		nested class defined to provide Enumerator.
    /// 
	__gc class Enumerator : public System::Collections::IEnumerator
	{
	private:
		ReadOnlyPropertyDefinitionCollection* m_pCol;
		System::Int32 m_nIdx;

	public:
		Enumerator(ReadOnlyPropertyDefinitionCollection* elements) : m_pCol(elements), m_nIdx(-1) 
		{

		}

		__property System::Object* get_Current();

		System::Boolean MoveNext();

		System::Void Reset();
	};

public private:
	FdoReadOnlyPropertyDefinitionCollection* GetImpObj();

private:
    // System::Collections::ICollection interfaces
	System::Void System::Collections::ICollection::CopyTo(System::Array* array,System::Int32 index);

	__property System::Object* System::Collections::ICollection::get_SyncRoot();

	__property System::Boolean System::Collections::ICollection::get_IsSynchronized();

protected:
	System::Void ReleaseUnmanagedObject();

public:
	ReadOnlyPropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection* parent);

	ReadOnlyPropertyDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

    /// \brief
    ///    Gets the count of items in collection.
    /// 
	__property System::Int32 get_Count(System::Void);

    /// \brief
    ///    Gets an enumerator that can iterate through a collection.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

    /// \brief
    ///     Determines the index of a specific PropertyDefinition object.
    /// 
    /// \param value 
    /// Input the PropertyDefinition object to locate in the collection.
    /// 
    /// \return
    /// 		The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition* value);

    /// \brief
    ///     Determines whether the collection contains a specific PropertyDefinition object.
    /// 
    /// \param value 
    /// Input The PropertyDefinition object to search in the collection.
    /// 
    /// \return
    /// 		Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition* value);

    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition* array[],System::Int32 index);


    /// \brief
    ///     Gets or sets a PropertyDefinition in the collection.
    /// 
    /// \param index 
    /// Input index of the PropertyDefinition to retrieve or set (System::Int32).
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition* get_RealTypeItem(System::Int32 index);

	__property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition* get_RealTypeItem(System::String* name);
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


