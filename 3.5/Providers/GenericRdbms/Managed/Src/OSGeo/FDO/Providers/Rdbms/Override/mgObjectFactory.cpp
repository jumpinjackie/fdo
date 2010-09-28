/*
* 
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

#include "stdafx.h"
#include <RdbmsOv.h>
#include <RdbmsOvPropertyMappingSingle.h>
#include <Utilities/SchemaMgr/Overrides/TableMappingType.h>

#include "mgOvTableMappingType.h"
#include "mgOvColumn.h"
#include "mgOvGeometricColumn.h"
#include "mgOvPropertyDefinition.h"
#include "mgOvClassDefinition.h"
#include "mgOvReadOnlyClassCollection.h"
#include "mgOvTable.h"
#include "mgOvReadOnlyPropertyDefinitionCollection.h"
#include "mgOvSchemaAutoGeneration.h"
#include "mgOvPropertyMappingDefinition.h"
#include "mgOvPropertyMappingSingle.h"
#include "mgOvPropertyMappingRelation.h"
#include "mgOvPropertyMappingConcrete.h"
#include "mgOvPropertyMappingClass.h"
#include "mgObjectFactory.h"

template <class fdoclas, class clas> 
inline NAMESPACE_OSGEO_RUNTIME::Disposable^ CHECK(FdoIDisposable* p, FdoBoolean autoDispose)
{
    return (dynamic_cast<fdoclas*>(p) ?  gcnew clas(IntPtr(p), autoDispose) : nullptr);
}

/*
    Autodesk::Gis::Fdo::Rdbms::Overrides
*/
NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvColumn^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvColumn(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvColumn(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvTable(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumn^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvGeometricColumn(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumn(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvClassDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvReadOnlyClassCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{  	
    if (ptr == IntPtr::Zero)
        return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyPropertyDefinitionCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvReadOnlyPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyPropertyDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvSchemaAutoGeneration(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvPropertyMappingDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoRdbmsOvPropertyMappingDefinition* p = (FdoRdbmsOvPropertyMappingDefinition*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	if (wrap = CHECK<FdoRdbmsOvPropertyMappingSingle, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingSingle>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingDefinition^>(wrap);

	if (wrap = CHECK<FdoRdbmsOvPropertyMappingDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingDefinition^>(wrap);
	
	return nullptr;	
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingSingle^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvPropertyMappingSingle(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingSingle(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingClass^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvPropertyMappingClass(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingClass(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingConcrete^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvPropertyMappingConcrete(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingConcrete(ptr, autoDispose);
}

