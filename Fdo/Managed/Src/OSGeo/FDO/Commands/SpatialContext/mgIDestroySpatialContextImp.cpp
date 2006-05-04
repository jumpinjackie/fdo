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
#include <Fdo\Commands\SpatialContext\IDestroySpatialContext.h>

#include "FDO\Commands\SpatialContext\mgIDestroySpatialContextImp.h"

FdoIDestroySpatialContext* NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IDestroySpatialContextImp::GetImpObj()
{
    return static_cast<FdoIDestroySpatialContext*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IDestroySpatialContextImp::get_Name()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

		return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IDestroySpatialContextImp::set_Name(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IDestroySpatialContextImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IDestroySpatialContextImp::Dispose()
{
	NAMESPACE_OSGEO_RUNTIME::Disposable::Dispose();
}
