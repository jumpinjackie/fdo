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

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
public __gc class OvPropertyDefinitionCollection;
public __gc class OvGeometricColumn;
public __gc class OvColumn;
public __gc class OvTable;
public __gc class OvClassDefinition;
public __gc class OvClassCollection;
public __gc class OvDataPropertyDefinition;

public __gc __interface IOvPropertyMappingDefinition;
public __gc __interface IOvPropertyDefinition;

public __gc __sealed class ObjectFactory
{
public:
    static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyDefinitionCollection* CreateOvPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyMappingDefinition* CreateIOvPropertyMappingDefinition(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricColumn* CreateOvGeometricColumn(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyDefinition* CreateIOvPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn* CreateOvColumn(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTable* CreateOvTable(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition* CreateOvClassDefinition(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassCollection* CreateOvClassCollection(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvDataPropertyDefinition* CreateOvDataPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);

private:
	ObjectFactory(){}
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
