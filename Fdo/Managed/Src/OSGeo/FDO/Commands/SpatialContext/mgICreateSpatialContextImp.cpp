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
#include <Fdo\Commands\SpatialContext\ICreateSpatialContext.h>
#include "FDO\Commands\SpatialContext\mgICreateSpatialContextImp.h"

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::Name::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return CHECK_STRING(result);
}

FdoICreateSpatialContext* NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::GetImpObj()
{
    return static_cast<FdoICreateSpatialContext*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::Name::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::Description::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::Description::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDescription(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::CoordinateSystem::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCoordinateSystem())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::CoordinateSystem::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCoordinateSystem(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::CoordinateSystemWkt::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCoordinateSystemWkt())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::CoordinateSystemWkt::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCoordinateSystemWkt(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::ExtentType::get()
{
	FdoSpatialContextExtentType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetExtentType())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::ExtentType::set(NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetExtentType(static_cast<FdoSpatialContextExtentType>(value)))
}

array<System::Byte>^ NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::Extent::get()
{
	FdoByteArray* arr;
    array<System::Byte>^ result;
    try
    {
    	EXCEPTION_HANDLER(arr = GetImpObj()->GetExtent())
    	result = FdoByteArrayToByteArray(arr->GetData(), arr->GetCount());
    }
    finally
    {
        if (arr != nullptr)
	        arr->Release();
    }
	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::Extent::set(array<System::Byte>^ buffer)
{
	FdoByteArray* byteArray = ByteArrayToFdoByteArray(buffer);
    try
    {
	    EXCEPTION_HANDLER(GetImpObj()->SetExtent(byteArray))
    }
    finally
    {
        if (byteArray != nullptr)
	        byteArray->Release();
    }
}

System::Double NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::XYTolerance::get()
{
	System::Double result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXYTolerance())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::XYTolerance::set(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetXYTolerance(value))
}

System::Double NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::ZTolerance::get()
{
	System::Double result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetZTolerance())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::ZTolerance::set(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetZTolerance(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::UpdateExisting::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetUpdateExisting())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::UpdateExisting::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetUpdateExisting(value))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}
