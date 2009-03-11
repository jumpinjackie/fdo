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

class FdoMySQLOvTable;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

///<summary>Concrete class defining physical schema overrides for a table.</summary>
public ref class OvTable : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable
{
internal:
	FdoMySQLOvTable* GetImpObj();
	
	OvTable(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    ///<summary>Constructs a default of an OvTable</summary>
    /// <returns>Returns OvTable</returns>
    OvTable();

    ///<summary>Constructs an instance of an OvTable using the specified arguments</summary>
    /// <param name="name">Input name</param>
    /// <returns>Returns OvTable</returns>
    OvTable(System::String^ name);

    ///<summary>Gets the directory where table data is stored</summary>
    /// <returns>Returns the directory where table data is stored</returns>
    ///<summary>Sets the directory where table data is stored</summary>
    /// <returns>Returns nothing</returns>
    property System::String^ DataDirectory
    {
        System::String^ get();
        System::Void set(System::String^ dataDirectory);
    }

    ///<summary>Gets the directory where indexes are stored</summary>
    /// <returns>Returns the directory where indexes are stored</returns>
    ///<summary>Sets the directory where indexes are stored</summary>
    /// <returns>Returns nothing</returns>
    property System::String^ IndexDirectory
    {
        System::String^ get();
        System::Void set(System::String^ indexDirectory);
    }

    ///<summary>Gets the storage engine type</summary>
    /// <returns>Returns the storage engine type</returns>
    ///<summary>Sets the storage engine type</summary>
    /// <returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType StorageEngine
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType storageEngine);
    }

    ///<summary>Gets the database</summary>
    /// <returns>Returns the database</returns>
    ///<summary>Sets the database</summary>
    /// <returns>Returns nothing</returns>
    property System::String^ Database
    {
        System::String^ get();
        System::Void set(System::String^ database);
    }
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

