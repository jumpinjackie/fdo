//
// Copyright (C) 2004-2008  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#pragma once

class FdoSqlServerOvTable;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL

///<summary>Concrete class defining physical schema overrides for a table.</summary>
public __gc class OvTable : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable
{
public:
	///<summary>Constructs a default of an FdoSqlServerOvTable</summary>
	OvTable();

    ///<summary>Constructs an instance of an FdoSqlServerOvTable using the specified arguments</summary>
    /// <param name="name">Input name</param>
	OvTable(System::String* name);

    ///<summary>Gets the table filegroup</summary>
    /// <returns>Returns the table filegroup</returns>
    __property System::String* get_TableFilegroup();

    ///<summary>Sets the table filegroup</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_TableFilegroup(System::String* tableFileGroup);

    ///<summary>Gets the text filegroup</summary>
    /// <returns>Returns the text filegroup</returns>
    __property System::String* get_TextFilegroup();

    ///<summary>Sets the text filegroup</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_TextFilegroup(System::String* textFileGroup);

    ///<summary>Gets the index filegroup</summary>
    /// <returns>Returns the index filegroup</returns>
    __property System::String* get_IndexFilegroup();

    ///<summary>Sets the index filegroup</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_IndexFilegroup(System::String* indexFileGroup);

    ///<summary>Gets the text-in-row option</summary>
    /// <returns>Returns the text-in-row option</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTextInRowOption get_TextInRow();

    ///<summary>Sets the text-in-row option</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_TextInRow(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTextInRowOption textInRow);

    ///<summary>Gets the database</summary>
    /// <returns>Returns the database</returns>
    __property System::String* get_Database();

    ///<summary>Sets the database</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_Database(System::String* database);

    ///<summary>Gets the owner</summary>
    /// <returns>Returns the owner</returns>
    __property System::String* get_Owner();

    ///<summary>Sets the owner</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_Owner(System::String* owner);

public private:
	OvTable(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSqlServerOvTable* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
