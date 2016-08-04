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

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE
ref class OvPropertyDefinition;
ref class OvReadOnlyPropertyDefinitionCollection;
ref class OvColumn;
ref class OvGeometricColumn;
ref class OvClassDefinition;
ref class OvTable;
ref class OvReadOnlyClassCollection;
ref class OvSchemaAutoGeneration;
ref class OvPropertyMappingDefinition;
ref class OvPropertyMappingSingle;
ref class OvPropertyMappingClass;
ref class OvPropertyMappingConcrete;

public ref class ObjectFactory sealed
{
public:
	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvColumn^ CreateOvColumn(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable^ CreateOvTable(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumn^ CreateOvGeometricColumn(System::IntPtr ptr, System::Boolean autoDispose);
	
	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^ CreateOvClassDefinition(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection^ CreateOvReadOnlyClassCollection(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition^ CreateOvPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyPropertyDefinitionCollection^ CreateOvReadOnlyPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration^ CreateOvSchemaAutoGeneration(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingDefinition^ CreateOvPropertyMappingDefinition(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingSingle^ CreateOvPropertyMappingSingle(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingClass^ CreateOvPropertyMappingClass(System::IntPtr ptr, System::Boolean autoDispose);

	static NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingConcrete^ CreateOvPropertyMappingConcrete(System::IntPtr ptr, System::Boolean autoDispose);

private:
	ObjectFactory(){}
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE
