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
#include <Fdo\Schema\NetworkFeatureClass.h>
#include <Fdo\Schema\FeatureClass.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Schema\mgNetworkFeatureClass.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgDataPropertyDefinition.h"
#include "FDO\Schema\mgAssociationPropertyDefinition.h"

FdoNetworkFeatureClass* NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass::GetImpObj()
{
	return static_cast<FdoNetworkFeatureClass*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass::CostProperty::get()
{
	FdoDataPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCostProperty())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass::CostProperty::set(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCostProperty((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass::NetworkProperty::get()
{
	FdoAssociationPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetNetworkProperty())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateAssociationPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass::NetworkProperty::set(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetNetworkProperty((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass::ReferencedFeatureProperty::get()
{
	FdoAssociationPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetReferencedFeatureProperty())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateAssociationPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass::ReferencedFeatureProperty::set(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetReferencedFeatureProperty((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass::ParentNetworkFeatureProperty::get()
{
	FdoAssociationPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetParentNetworkFeatureProperty())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateAssociationPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass::ParentNetworkFeatureProperty::set(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetParentNetworkFeatureProperty((value == nullptr ? nullptr : value->GetImpObj())))
}

