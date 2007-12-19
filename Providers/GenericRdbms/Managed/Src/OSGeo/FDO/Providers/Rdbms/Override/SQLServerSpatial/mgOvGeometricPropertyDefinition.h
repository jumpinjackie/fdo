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

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgIOvPropertyDefinition.h"

class FdoSqlServerOvGeometricPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
public __gc class OvGeometricColumn;

///<summary>Concrete class defining physical schema overrides for a data property definition.</summary>
public __gc class OvGeometricPropertyDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyDefinition
{
public:
    ///<summary>Constructs a default of an FdoSqlServerOvGeometricPropertyDefinition</summary>
	OvGeometricPropertyDefinition();

    ///<summary>Constructs an instance of an FdoSqlServerOvGeometricPropertyDefinition using the specified arguments</summary>
    /// <param name="name">Input name</param>
	OvGeometricPropertyDefinition(System::String* name);

    ///<summary>Gets the SqlServer column</summary>
    /// <returns>Returns the SqlServer column</returns>
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricColumn* get_Column();

    ///<summary>Sets the SqlServer column</summary>
    /// <returns>Returns nothing</returns>
	__property System::Void set_Column(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricColumn* geometricColumn);

public private:
	OvGeometricPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSqlServerOvGeometricPropertyDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
