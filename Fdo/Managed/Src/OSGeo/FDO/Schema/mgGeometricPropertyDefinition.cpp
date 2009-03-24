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
	EXCEPTION_HANDLER(Attach(IntPtr(FdoGeometricPropertyDefinition::Create()), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::GeometricPropertyDefinition(System::String^ name, System::String^ description) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoGeometricPropertyDefinition::Create(StringToUni(name), StringToUni(description))), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::GeometricPropertyDefinition(System::String^ name, System::String^ description, System::Boolean system) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoGeometricPropertyDefinition::Create(StringToUni(name), StringToUni(description), system)), true))
}

FdoGeometricPropertyDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::GetImpObj()
{
	return static_cast<FdoGeometricPropertyDefinition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::PropertyType::get()
{
	FdoPropertyType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(result);
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::GeometryTypes::get()
{
	System::Int32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGeometryTypes())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::GeometryTypes::set(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometryTypes(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::ReadOnly::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetReadOnly())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::ReadOnly::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetReadOnly(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::HasElevation::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetHasElevation())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::HasElevation::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetHasElevation(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::HasMeasure::get()
{	
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetHasMeasure())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::HasMeasure::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetHasMeasure(value))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::SpatialContextAssociation::set(System::String ^value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSpatialContextAssociation(StringToUni(value)))
}

System::String ^ NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::SpatialContextAssociation::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSpatialContextAssociation())

	return CHECK_STRING(result);
}

array<NAMESPACE_OSGEO_COMMON::GeometryType>^ NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::SpecificGeometryTypes::get(System::Int32% length)
{
	FdoGeometryType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpecificGeometryTypes(unlength));
    array<NAMESPACE_OSGEO_COMMON::GeometryType>^ mtype = gcnew array<NAMESPACE_OSGEO_COMMON::GeometryType>(unlength);
	for(FdoInt32 i = 0; i < unlength; i ++)
		mtype[i] = static_cast<NAMESPACE_OSGEO_COMMON::GeometryType>(*(unobj + i));
	
    return mtype;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition::SpecificGeometryTypes::set(System::Int32% length, array<NAMESPACE_OSGEO_COMMON::GeometryType>^ types)
{
	FdoGeometryType *unobj = new FdoGeometryType[length+1];
	for(FdoInt32 i = 0; i < length; i++)
	{
        unobj[i] = static_cast<FdoGeometryType>(types[i]);
	}
    EXCEPTION_HANDLER(GetImpObj()->SetSpecificGeometryTypes(unobj, length));
}
