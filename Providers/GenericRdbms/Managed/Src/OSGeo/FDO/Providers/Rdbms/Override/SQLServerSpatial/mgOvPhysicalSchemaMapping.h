/*
* (C) Copyright 2005 by Autodesk, Inc. All Rights Reserved.
*
* By using this code, you are agreeing to the terms and conditions of
* the License Agreement included in the documentation for this code.
*
* AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
* CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
* IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
* DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
* DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
*
* Use, duplication, or disclosure by the U.S. Government is subject
* to restrictions set forth in FAR 52.227-19 (Commercial Computer
* Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
* (Rights in Technical Data and Computer Software), as applicable.
*
*
*/

#pragma once

class FdoSqlServerOvPhysicalSchemaMapping;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
public __gc class OvClassCollection;

/// <summary>Concrete class defining physical schema overrides for a table.</summary>
public __gc class OvPhysicalSchemaMapping : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping
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
    OvPhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* schemaMapping, System::Boolean autoDelete);

    /// <summary>Constructs an instance of an FdoSqlServerOvPhysicalSchemaMapping using the specified arguments</summary>
    /// <param name="name">Input name</param>
    OvPhysicalSchemaMapping(System::String* name);

    /// <summary>Gets the classes</summary>
    /// <returns>Returns the classes</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassCollection* get_Classes();

    /// <summary>Gets the table Filegroup</summary>
    /// <returns>Returns the table Filegroup</returns>
    __property System::String* get_TableFilegroup();

    /// <summary>Sets the table Filegroup</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_TableFilegroup(System::String* tableFileGroup);

    /// <summary>Gets the text Filegroup</summary>
    /// <returns>Returns the text Filegroup</returns>
    __property System::String* get_TextFilegroup();

    ///<summary>Sets the text Filegroup</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_TextFilegroup(System::String* textFileGroup);

    /// <summary>Gets the index Filegroup</summary>
    /// <returns>Returns the index Filegroup</returns>
    __property System::String* get_IndexFilegroup();

    /// <summary>Sets the index Filegroup</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_IndexFilegroup(System::String* indexFileGroup);

    /// <summary>Gets the text-in-row option</summary>
    /// <returns>Returns the text-in-row option</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTextInRowOption get_TextInRow();

    /// <summary>Sets the text-in-row option</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_TextInRow(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTextInRowOption textInRow);

    /// <summary>Gets the database</summary>
    /// <returns>Returns the database</returns>
    __property System::String* get_Database();

    /// <summary>Sets the database</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_Database(System::String* database);

    /// <summary>Gets the owner</summary>
    /// <returns>Returns the owner</returns>
    __property System::String* get_Owner();

    /// <summary>Sets the owner</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_Owner(System::String* owner);

public private:
    OvPhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete);

    inline FdoSqlServerOvPhysicalSchemaMapping* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
