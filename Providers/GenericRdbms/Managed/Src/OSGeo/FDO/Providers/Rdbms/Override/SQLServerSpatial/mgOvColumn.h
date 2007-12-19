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

class FdoSqlServerOvColumn;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL

///<summary>Concrete class defining physical schema overrides for a column.</summary>
public __gc class OvColumn : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvColumn
{
public:
    ///<summary>Constructs a default of an FdoSqlServerOvColumn</summary>
	OvColumn();

    ///<summary>Constructs an instance of an FdoSqlServerOvColumn using the specified arguments</summary>
    /// <param name="name">Input name</param>
	OvColumn(System::String* name);

    ///<summary>Gets the SQL Server formula for this column</summary>
    /// <remarks>An empty string denotes that there is no formula for this column;
    /// otherwise, the column's value is computed based on the formula.</remarks>
    /// <returns>Returns the SQL Server formula</returns>
	__property System::String* get_Formula();

    ///<summary>Sets the SQL Server formula for this column</summary>
    /// <remarks>An empty string denotes that there is no formula for this column;
    /// otherwise, the column's value is computed based on the formula.</remarks>
    /// <returns>Returns nothing</returns>
	__property System::Void set_Formula(System::String* formula);

public private:
	OvColumn(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSqlServerOvColumn* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
