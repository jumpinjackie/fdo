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
#include <Rdbms.h>
#include <Fdo\Schema\GeometricPropertyDefinition.h>
#include <IDestroySpatialIndex.h>

#include "mgIDestroySpatialIndexImp.h"

FdoIDestroySpatialIndex* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IDestroySpatialIndexImp::GetImpObj()
{
	return static_cast<FdoIDestroySpatialIndex*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IDestroySpatialIndexImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IDestroySpatialIndexImp::IDestroySpatialIndexImp(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
{

}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IDestroySpatialIndexImp::Name::get()
{
	FdoString* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetName())

	return CHECK_STRING(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IDestroySpatialIndexImp::Name::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IDestroySpatialIndexImp::GeometricProperty::set(NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometricProperty(static_cast<FdoGeometricPropertyDefinition*>(value->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IDestroySpatialIndexImp::GeometricProperty::get()
{
	FdoGeometricPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGeometricProperty())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateGeometricPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IDestroySpatialIndexImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}

