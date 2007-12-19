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
