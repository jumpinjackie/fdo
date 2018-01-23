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

class FdoOdbcOvClassDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC
ref class OvPropertyDefinitionCollection;
ref class OvTable;

///<summary>Concrete class defining physical schema overrides for a class definition.</summary>
public ref class OvClassDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition
{
public:
    ///<summary>Constructs a default of an FdoOdbcOvClassDefinition</summary>
    /// <returns>Returns FdoOdbcOvClassDefinition</returns>
    OvClassDefinition();

    ///<summary>Constructs an instance of an FdoOdbcOvClassDefinition using the specified arguments</summary>
    /// <param name="name">Input name</param>
    /// <returns>Returns FdoOdbcOvClassDefinition</returns>
	OvClassDefinition(System::String^ name);

    ///<summary>Gets a collection of Odbc property overrides</summary>
    /// <returns>Returns the collection of Odbc property overrides</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection^ Properties
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPropertyDefinitionCollection^ get();
    }

    ///<summary>Gets the Odbc table override for this class override</summary>
    /// <returns>Returns FdoOdbcOvTable</returns>
    ///<summary>Sets the Odbc table override for this class override</summary>
    /// <param name="name">Input Odbc table override</param>
    /// <returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvTable^ Table
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvTable^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvTable^ value);
    }

internal:
	OvClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoOdbcOvClassDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC

