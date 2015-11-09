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

class FdoMySQLOvGeometricColumn;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

///<summary>Concrete class defining physical schema overrides for a geometric column.</summary>
public ref class OvGeometricColumn : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumn
{	
internal:
	OvGeometricColumn(System::IntPtr unmanaged, System::Boolean autoDelete);
	FdoMySQLOvGeometricColumn* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

public:
    ///<summary>Constructs a default of an OvGeometricColumn</summary>
    /// <returns>Returns OvGeometricColumn</returns>
    OvGeometricColumn();

    ///<summary>Constructs an instance of an OvGeometricColumn using the specified arguments</summary>
    /// <param name="name">Input name</param>
    /// <returns>Returns OvGeometricColumn</returns>
	OvGeometricColumn(System::String^ name);

};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

