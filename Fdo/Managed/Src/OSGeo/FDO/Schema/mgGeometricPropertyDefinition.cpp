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
#include <Fdo\Schema\GeometricPropertyDefinition.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\PropertyType.h>

#include "FDO\Schema\mgGeometricPropertyDefinition.h"

NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::GeometricPropertyDefinition() : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoGeometricPropertyDefinition::Create(), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::GeometricPropertyDefinition(System::String* name, System::String* description) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoGeometricPropertyDefinition::Create(StringToUni(name), StringToUni(description)), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::GeometricPropertyDefinition(System::String* name, System::String* description, System::Boolean system) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoGeometricPropertyDefinition::Create(StringToUni(name), StringToUni(description), system), true))
}

FdoGeometricPropertyDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::GetImpObj()
{
	return static_cast<FdoGeometricPropertyDefinition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::get_PropertyType()
{
	FdoPropertyType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(result);
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::get_GeometryTypes()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGeometryTypes())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::set_GeometryTypes(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometryTypes(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::get_ReadOnly()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetReadOnly())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::set_ReadOnly(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetReadOnly(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::get_HasElevation()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetHasElevation())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::set_HasElevation(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetHasElevation(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::get_HasMeasure()
{	
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetHasMeasure())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::set_HasMeasure(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetHasMeasure(value))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::set_SpatialContextAssociation(System::String *value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSpatialContextAssociation(StringToUni(value)))
}

System::String * NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::get_SpatialContextAssociation()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSpatialContextAssociation())

	return result;
}
NAMESPACE_OSGEO_COMMON::GeometryType NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::get_SpecificGeometryTypes(System::Int32 &length)[]
{
	FdoGeometryType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpecificGeometryTypes(unlength));
    NAMESPACE_OSGEO_COMMON::GeometryType mtype [] = new NAMESPACE_OSGEO_COMMON::GeometryType[unlength];
	for(FdoInt32 i = 0; i < unlength; i ++)
	{
		mtype[i] = static_cast<NAMESPACE_OSGEO_COMMON::GeometryType>(*(unobj + i));
	}
	return mtype;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::set_SpecificGeometryTypes(NAMESPACE_OSGEO_COMMON::GeometryType types[], System::Int32 length)
{
	FdoGeometryType *unobj = new FdoGeometryType[length+1];
	for(FdoInt32 i = 0; i < length; i ++)
	{
        unobj[i] = static_cast<FdoGeometryType>(types[i]);
	}
    EXCEPTION_HANDLER(GetImpObj()->SetSpecificGeometryTypes(unobj, length));
}
