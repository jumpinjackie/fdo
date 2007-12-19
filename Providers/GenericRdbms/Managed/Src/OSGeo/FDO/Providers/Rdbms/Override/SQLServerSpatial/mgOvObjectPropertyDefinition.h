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

class FdoSqlServerOvObjectPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
public __gc __interface IOvPropertyMappingDefinition;

///<summary>Concrete class defining physical schema overrides for an object property definition.</summary>
public __gc class OvObjectPropertyDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyDefinition
{
public:
    ///<summary>Constructs a default of an FdoSqlServerOvObjectPropertyDefinition</summary>
	OvObjectPropertyDefinition();

    ///<summary>Constructs an instance of an FdoSqlServerOvObjectPropertyDefinition using the specified arguments</summary>
    /// <param name="name">Input name</param>
	OvObjectPropertyDefinition(System::String* name);

	///<summary>Gets the mapping definition</summary>
    /// <returns>Returns the mapping definition</returns>
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyMappingDefinition* get_MappingDefinition();

    ///<summary>Sets the mapping definition</summary>
    /// <returns>Returns nothing</returns>
	__property System::Void set_MappingDefinition(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyMappingDefinition* propertyMappingDefinition);

public private:
	OvObjectPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSqlServerOvObjectPropertyDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
