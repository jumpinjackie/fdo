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

class FdoOdbcOvPhysicalSchemaMapping;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC
ref class OvClassCollection;

///<summary>Concrete class offering schema overrides for
///the Odbc Provider.</summary>
public ref class OvPhysicalSchemaMapping : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping
{
public:
    ///<summary>Constructs a default of an FdoOdbcOvPhysicalSchemaMapping</summary>
    /// <returns>Returns FdoOdbcOvPhysicalSchemaMapping</returns>
    OvPhysicalSchemaMapping();

    /// <summary>Constructs a physical schema mapping instance using the 
    /// result returned from IConnection::CreateSchemaMapping OR using the result of 
    /// executing an IDescribeSchemaMapping command. This constructor is provided 
    /// in order that clients are provided a mechanism to downcast from the FDO 
    /// PhysicalSchemaMapping base class to an FDO Provider specific class 
    /// that derives from PhysicalSchemaMapping. This constructor is the only 
    /// mechanism provided by the FDO Managed API that will allow the client to correctly
    /// construct a provider specific PhysicalSchemaMapping class. 
    /// NOTE: If the Provider name associated to the input parameter schemaMapping does not
    /// correspond to the provider name of the expected FDO Provider, and InvalidArgumentException
    /// will be thrown.
    /// </summary>
    OvPhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ schemaMapping, System::Boolean autoDelete);

    ///<summary>Constructs an instance of an FdoOdbcOvPhysicalSchemaMapping using the specified arguments</summary>
    /// <param name="name">Input name</param>
    /// <returns>Returns FdoOdbcOvPhysicalSchemaMapping</returns>
    OvPhysicalSchemaMapping(System::String^ name);

    ///<summary>Gets the classes</summary>
    /// <returns>Returns the classes</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvClassCollection^ Classes
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvClassCollection^ get();
    }

    ///<summary>Gets the provider name</summary>
    /// <returns>Returns the provider name</returns>
    property System::String^ Provider
    {
        System::String^ get();
    }

internal:
    OvPhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete);

    inline FdoOdbcOvPhysicalSchemaMapping* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC

