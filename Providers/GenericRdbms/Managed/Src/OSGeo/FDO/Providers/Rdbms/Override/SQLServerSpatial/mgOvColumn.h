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

class FdoSqlServerOvColumn;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL

///<summary>Concrete class defining physical schema overrides for a column.</summary>
public ref class OvColumn : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvColumn
{
public:
    ///<summary>Constructs a default of an FdoSqlServerOvColumn</summary>
	OvColumn();

    ///<summary>Constructs an instance of an FdoSqlServerOvColumn using the specified arguments</summary>
    /// <param name="name">Input name</param>
	OvColumn(System::String^ name);

    ///<summary>Gets the SQL Server formula for this column</summary>
    /// <remarks>An empty string denotes that there is no formula for this column;
    /// otherwise, the column's value is computed based on the formula.</remarks>
    /// <returns>Returns the SQL Server formula</returns>
    ///<summary>Sets the SQL Server formula for this column</summary>
    /// <remarks>An empty string denotes that there is no formula for this column;
    /// otherwise, the column's value is computed based on the formula.</remarks>
    /// <returns>Returns nothing</returns>
    property System::String^ Formula
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

internal:
	OvColumn(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSqlServerOvColumn* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
