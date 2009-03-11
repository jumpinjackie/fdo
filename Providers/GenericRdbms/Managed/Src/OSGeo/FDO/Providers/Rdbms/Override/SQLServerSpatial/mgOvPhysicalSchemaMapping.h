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

class FdoSqlServerOvPhysicalSchemaMapping;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
ref class OvClassCollection;

/// <summary>Concrete class defining physical schema overrides for a table.</summary>
public ref class OvPhysicalSchemaMapping : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping
{
public:
    /// <summary>Constructs a default of an FdoSqlServerOvPhysicalSchemaMapping</summary>
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

    /// <summary>Constructs an instance of an FdoSqlServerOvPhysicalSchemaMapping using the specified arguments</summary>
    /// <param name="name">Input name</param>
    OvPhysicalSchemaMapping(System::String^ name);

    /// <summary>Gets the classes</summary>
    /// <returns>Returns the classes</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassCollection^ Classes
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassCollection^ get();
    }

    /// <summary>Gets the table Filegroup</summary>
    /// <returns>Returns the table Filegroup</returns>
    /// <summary>Sets the table Filegroup</summary>
    /// <returns>Returns nothing</returns>
    property System::String^ TableFilegroup
    {
        System::String^ get();
        System::Void set(System::String^ tableFileGroup);
    }

    /// <summary>Gets the text Filegroup</summary>
    /// <returns>Returns the text Filegroup</returns>
    ///<summary>Sets the text Filegroup</summary>
    /// <returns>Returns nothing</returns>
    property System::String^ TextFilegroup
    {
        System::String^ get();
        System::Void set(System::String^ textFileGroup);
    }

    /// <summary>Gets the index Filegroup</summary>
    /// <returns>Returns the index Filegroup</returns>
    /// <summary>Sets the index Filegroup</summary>
    /// <returns>Returns nothing</returns>
    property System::String^ IndexFilegroup
    {
        System::String^ get();
        System::Void set(System::String^ indexFileGroup);
    }

    /// <summary>Gets the text-in-row option</summary>
    /// <returns>Returns the text-in-row option</returns>
    /// <summary>Sets the text-in-row option</summary>
    /// <returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTextInRowOption TextInRow
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTextInRowOption get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTextInRowOption textInRow);
    }

    /// <summary>Gets the database</summary>
    /// <returns>Returns the database</returns>
    /// <summary>Sets the database</summary>
    /// <returns>Returns nothing</returns>
    property System::String^ Database
    {
        System::String^ get();
        System::Void set(System::String^ database);
    }

    /// <summary>Gets the owner</summary>
    /// <returns>Returns the owner</returns>
    /// <summary>Sets the owner</summary>
    /// <returns>Returns nothing</returns>
    property System::String^ Owner
    {
        System::String^ get();
        System::Void set(System::String^ owner);
    }

internal:
    OvPhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete);

    inline FdoSqlServerOvPhysicalSchemaMapping* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
