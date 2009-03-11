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

class FdoOdbcOvPropertyDefinitionCollection;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC
interface class IOvPropertyDefinition;

/// <summary> Concrete class defining a collection of FDO class definitions
/// with overrides in the physical schema.</summary>
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class OvPropertyDefinitionCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionBase
{
/// \cond DOXYGEN-IGNORE
internal:
	OvPropertyDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoOdbcOvPropertyDefinitionCollection* GetImpObj();

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
    /// <summary>Constructs a new empty collection of Raster Band Definition.</summary>
    /// <param name="parent">Input parent object</param> 
	OvPropertyDefinitionCollection();

	OvPropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinitionCollection^ baseCollection);

	/// <summary>
	///    Gets the count of items in collection.
	/// </summary>
    property System::Int32 Count
    {
        virtual System::Int32 get() override;
    }

	/// <summary>
	///     Removes the index-th IOvPropertyDefinition from this collection.
	/// </summary>
	/// <param name="index">
	///		Input index of the element to remove.
	/// </param>
    virtual System::Void RemoveAt(System::Int32 index) override;

	/// <summary>
	///     Removes all elements from the collection.
	/// </summary>
    virtual System::Void  Clear() override;

	/// <summary>
	///     Adds a IOvPropertyDefinition object into the collection.
	/// </summary>
	/// <param name="value">
	///		Input the IOvPropertyDefinition object to add.
	/// </param>
	/// <returns>
	///		Returns the position into which the new element was inserted.
	/// </returns>
	System::Int32 Add(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value);

	/// <summary>
	///     Determines the index of a specific IOvPropertyDefinition object.
	/// </summary>
	/// <param name="value">
	///		Input the IOvPropertyDefinition object to locate in the collection.
	/// </param>
	/// <returns>
	///		The index of value if found in the collection; otherwise, -1.
	/// </returns>
	System::Int32 IndexOf(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value);

	/// <summary>
	///     Inserts a IOvPropertyDefinition object into the collection at the specified position.
	/// </summary>
	/// <param name="index">
	///		Input the zero-based index at which value should be inserted. 
	/// </param>
	/// <param name="value">
	///		Input the IOvPropertyDefinition object to insert.
	/// </param>
	System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value);

	/// <summary>
	///     Removes the first occurrence of a specific IOvPropertyDefinition object.
	/// </summary>
	/// <param name="value">
	///		Input the IOvPropertyDefinition object to remove from the collection.
	/// </param>
	System::Void Remove(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value);

	/// <summary>
	///     Determines whether the collection contains a specific IOvPropertyDefinition object.
	/// </summary>
	/// <param name="value">
	///		Input The IOvPropertyDefinition object to search in the collection.
	/// </param>
	/// <returns>
	///		Returns true if the value is found in the collection; otherwise, false.
	/// </returns>
	System::Boolean Contains(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value);

	/// <summary>
	///     Copies the elements of the collection to an array.
	/// </summary>
	/// <param name="array">
	///		Output the one-dimensional Array that is the destination of the elements copied from this collection. 
	/// </param>
	/// <param name="startAt">
	///		Input an integer that represents the index in array at which copying begins. 
	/// </param>
    System::Void CopyTo(array<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^>^ pArray, System::Int32 index);

	//this is a hedious workaround for a bug in VS2002. 
	/// <summary>
	///     Gets or sets a IOvPropertyDefinition in the collection.
	/// </summary>
	/// <param name="index">
	///		Input index of the IOvPropertyDefinition to retrieve or set (System::Int32). 
	/// </param>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ Item[System::String^]
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ get(System::String^ index);
    }
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ Item[System::Int32]
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ get(System::Int32 index);
        System::Void set(System::Int32 index, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition^ value);
    }
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC

