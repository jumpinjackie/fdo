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
#include <MySQLOv.h>

#include <MySQLOvStorageEngineType.h>
#include <MySQLOvDataPropertyDefinition.h>
#include <MySQLOvAssociationPropertyDefinition.h>
#include <MySQLOvGeometricPropertyDefinition.h>
#include <MySQLOvObjectPropertyDefinition.h>
#include <MySQLOvPropertyMappingSingle.h>
#include <MySQLOvPropertyMappingDefinition.h>

#include "FDO\Providers\Rdbms\Override\MySQL\mgOvStorageEngineType.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgIOvPropertyMappingDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgIOvPropertyMappingRelation.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvPropertyMappingSingle.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvDataPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvAssociationPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvGeometricPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvObjectPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvClassDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvPropertyDefinitionCollection.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvTable.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvGeometricColumn.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvColumn.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvDataPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvPropertyMappingDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvClassCollection.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgObjectFactory.h"

template <class fdoclas, class clas> 
inline NAMESPACE_OSGEO_RUNTIME::Disposable* CHECK(FdoIDisposable* p, FdoBoolean autoDispose)
{
    return (dynamic_cast<fdoclas*>(p) ?  new clas(p, autoDispose) : NULL);
}

/*
    Autodesk::Gis::Fdo::Rdbms::Override::MySQL
*/
NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::ObjectFactory::CreateIOvPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

	FdoMySQLOvPropertyDefinition* p = (FdoMySQLOvPropertyDefinition*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable* wrap;

	if (wrap = CHECK<FdoMySQLOvDataPropertyDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition*>(wrap);
	if (wrap = CHECK<FdoMySQLOvAssociationPropertyDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvAssociationPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvAssociationPropertyDefinition*>(wrap);
	if (wrap = CHECK<FdoMySQLOvGeometricPropertyDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricPropertyDefinition*>(wrap);
	if (wrap = CHECK<FdoMySQLOvObjectPropertyDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvObjectPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvObjectPropertyDefinition*>(wrap);
	
	if (wrap = CHECK<FdoMySQLOvPropertyDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinition*>(wrap);
	
	return NULL;
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::ObjectFactory::CreateOvClassDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

	return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::ObjectFactory::CreateOvPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

	return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::ObjectFactory::CreateOvTable(System::IntPtr ptr, System::Boolean autoDispose)
{ 
    if (ptr == IntPtr::Zero)
        return NULL;

	return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricColumn* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::ObjectFactory::CreateOvGeometricColumn(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

	return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricColumn(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvColumn* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::ObjectFactory::CreateOvColumn(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

	return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvColumn(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::ObjectFactory::CreateOvDataPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

	return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyMappingDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::ObjectFactory::CreateIOvPropertyMappingDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

	FdoMySQLOvPropertyMappingDefinition* p = (FdoMySQLOvPropertyMappingDefinition*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable* wrap;

	if (wrap = CHECK<FdoMySQLOvPropertyMappingSingle, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyMappingSingle>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyMappingSingle*>(wrap);
	
	if (wrap = CHECK<FdoMySQLOvPropertyMappingDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyMappingDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyMappingDefinition*>(wrap);
	
	return NULL;
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::ObjectFactory::CreateOvClassCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

	return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassCollection(ptr, autoDispose);
}

