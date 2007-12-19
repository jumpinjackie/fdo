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

#include "stdafx.h"
#include "SqlServer\SqlServerOvTextInRowOption.h"
#include "SqlServer\SqlServerOvAssociationPropertyDefinition.h"
#include "SqlServer\SqlServerOvDataPropertyDefinition.h"
#include "SqlServer\SqlServerOvGeometricPropertyDefinition.h"
#include "SqlServer\SqlServerOvObjectPropertyDefinition.h"
#include "SqlServer\SqlServerOvPropertyMappingClass.h"
#include "SqlServer\SqlServerOvPropertyMappingConcrete.h"
#include "SqlServer\SqlServerOvPropertyMappingRelation.h"
#include "SqlServer\SqlServerOvPropertyMappingSingle.h"

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvTextInRowOption.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgObjectFactory.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvPropertyDefinitionCollection.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvPropertyMappingDefinition.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvGeometricColumn.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvColumn.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvTable.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvClassDefinition.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvClassCollection.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvDataPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvAssociationPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvDataPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvGeometricPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvObjectPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvPropertyMappingClass.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvPropertyMappingConcrete.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvPropertyMappingRelation.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvPropertyMappingSingle.h"

template <class fdoclas, class clas> 
inline NAMESPACE_OSGEO_RUNTIME::Disposable* CHECK(FdoIDisposable* p, FdoBoolean autoDispose)
{
    return (dynamic_cast<fdoclas *>(p) ?  new clas(p, autoDispose) : NULL);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

    return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyMappingDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateIOvPropertyMappingDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

	FdoSqlServerOvPropertyMappingDefinition* p = (FdoSqlServerOvPropertyMappingDefinition*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable* wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr"
	if(wrap = CHECK<FdoSqlServerOvPropertyMappingClass, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingClass>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingClass*>(wrap);
	if(wrap = CHECK<FdoSqlServerOvPropertyMappingConcrete, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingConcrete>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingConcrete*>(wrap);
	
	if(wrap = CHECK<FdoSqlServerOvPropertyMappingRelation, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingRelation>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingRelation*>(wrap);
	if(wrap = CHECK<FdoSqlServerOvPropertyMappingSingle, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingSingle>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingSingle*>(wrap);

	if(wrap = CHECK<FdoSqlServerOvPropertyMappingDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingDefinition*>(wrap);

    return NULL;
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricColumn* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvGeometricColumn(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

    return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricColumn(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateIOvPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

	FdoSqlServerOvPropertyDefinition* p = (FdoSqlServerOvPropertyDefinition*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable* wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr"
	if(wrap = CHECK<FdoSqlServerOvAssociationPropertyDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvAssociationPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvAssociationPropertyDefinition*>(wrap);
	if(wrap = CHECK<FdoSqlServerOvDataPropertyDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvDataPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvDataPropertyDefinition*>(wrap);
	if(wrap = CHECK<FdoSqlServerOvGeometricPropertyDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricPropertyDefinition*>(wrap);
	if(wrap = CHECK<FdoSqlServerOvObjectPropertyDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvObjectPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvObjectPropertyDefinition*>(wrap);

	if(wrap = CHECK<FdoSqlServerOvPropertyDefinition, NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyDefinition*>(wrap);

    return NULL;
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvColumn(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

    return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTable* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvTable(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

    return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTable(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvClassDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

    return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvClassCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

    return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvDataPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvDataPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
        return NULL;

    return new NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvDataPropertyDefinition(ptr, autoDispose);
}