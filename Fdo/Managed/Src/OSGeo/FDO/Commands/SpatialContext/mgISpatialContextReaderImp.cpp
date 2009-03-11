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
// The following header files sequence is very important
#include <Fdo\Commands\SpatialContext\SpatialContextExtentType.h>
#include <Fdo\Commands\SpatialContext\ISpatialContextReader.h>

#include "FDO\Commands\SpatialContext\mgISpatialContextReaderImp.h"

FdoISpatialContextReader* NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp::GetImpObj()
{
    return static_cast<FdoISpatialContextReader*>(__super::UnmanagedObject.ToPointer());
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp::GetName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp::GetDescription()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())

	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp::GetCoordinateSystem()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCoordinateSystem())

	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp::GetCoordinateSystemWkt()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCoordinateSystemWkt())

	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp::GetExtentType()
{
	FdoSpatialContextExtentType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetExtentType())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>(result);
}

array<System::Byte>^ NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp::GetExtent()
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
        if(arr != nullptr)
	        arr->Release();
    }
	return result;
}

System::Double NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp::GetXYTolerance()
{
	System::Double result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXYTolerance())

	return result;
}

System::Double NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp::GetZTolerance()
{
	System::Double result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetZTolerance())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp::IsActive()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->IsActive())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp::ReadNext()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = GetImpObj()->ReadNext())

	return result;
}
