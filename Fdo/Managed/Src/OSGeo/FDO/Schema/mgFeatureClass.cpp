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
#include <Fdo\Schema\FeatureClass.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Schema\mgFeatureClass.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgGeometricPropertyDefinition.h"

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass::FeatureClass() : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFeatureClass::Create()), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass::FeatureClass(System::String^ name, System::String^ description) : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFeatureClass::Create(StringToUni(name), StringToUni(description))), true))
}

FdoFeatureClass* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass::GetImpObj()
{
	return static_cast<FdoFeatureClass*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassType NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass::ClassType::get()
{
	FdoClassType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::ClassType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass::GeometryProperty::get()
{
	FdoGeometricPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGeometryProperty())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateGeometricPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass::GeometryProperty::set(NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometryProperty((value == nullptr ? nullptr : value->GetImpObj())))
}

