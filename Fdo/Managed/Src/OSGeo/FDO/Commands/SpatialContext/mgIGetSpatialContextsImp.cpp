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
#include <Fdo\Commands\SpatialContext\IGetSpatialContexts.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\SpatialContext\mgIGetSpatialContextsImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\SpatialContext\mgISpatialContextReaderImp.h"

FdoIGetSpatialContexts* NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IGetSpatialContextsImp::GetImpObj()
{
    return static_cast<FdoIGetSpatialContexts*>(__super::UnmanagedObject.ToPointer());
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IGetSpatialContextsImp::get_ActiveOnly()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetActiveOnly())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IGetSpatialContextsImp::set_ActiveOnly(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetActiveOnly(value))
}

NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReader* NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IGetSpatialContextsImp::Execute()
{
	FdoISpatialContextReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->Execute())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateISpatialContextReader(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IGetSpatialContextsImp::Dispose()
{
	NAMESPACE_OSGEO_RUNTIME::Disposable::Dispose();
}
