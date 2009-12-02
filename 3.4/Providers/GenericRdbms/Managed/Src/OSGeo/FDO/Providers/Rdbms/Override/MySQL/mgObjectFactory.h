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

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL
public __gc class OvPropertyDefinition;
public __gc class OvClassDefinition;
public __gc class OvPropertyDefinitionCollection;
public __gc class OvTable;
public __gc class OvGeometricColumn;
public __gc class OvColumn;
public __gc class OvDataPropertyDefinition;
public __gc class OvPropertyMappingDefinition;
public __gc class OvClassCollection;
public __gc __interface IOvPropertyDefinition; 
public __gc __interface IOvPropertyMappingDefinition;

public __gc __sealed class ObjectFactory
{
public:
	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition* CreateIOvPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition* CreateOvClassDefinition(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection* CreateOvPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable* CreateOvTable(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricColumn* CreateOvGeometricColumn(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvColumn* CreateOvColumn(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* CreateOvDataPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyMappingDefinition* CreateIOvPropertyMappingDefinition(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassCollection* CreateOvClassCollection(System::IntPtr ptr, System::Boolean autoDispose);

private:
	ObjectFactory(){}
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL
