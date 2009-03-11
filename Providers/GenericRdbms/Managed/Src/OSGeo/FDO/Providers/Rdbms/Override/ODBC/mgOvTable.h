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

class FdoOdbcOvTable;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC

///<summary>Concrete class defining physical schema overrides for a table.</summary>
public ref class OvTable : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable
{
public:
    ///<summary>Constructs a default of an FdoOdbcOvTable</summary>
    /// <returns>Returns FdoOdbcOvTable</returns>
    OvTable();

    ///<summary>Constructs an instance of an FdoOdbcOvTable using the specified arguments</summary>
    /// <param name="name">Input name</param>
    /// <returns>Returns FdoOdbcOvTable</returns>
	OvTable(System::String^ name);

internal:
	OvTable(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoOdbcOvTable* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC

