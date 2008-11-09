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
#include <Fdo\Commands\Feature\IExtendedSelect.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Feature\mgIExtendedSelectImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\Feature\mgIScrollableFeatureReaderImp.h"

FdoIExtendedSelect* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IExtendedSelectImp::GetImpObj()
{
    return static_cast<FdoIExtendedSelect*>(__super::UnmanagedObject.ToPointer());
}

void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IExtendedSelectImp::SetOrderingOption( System::String* propertyName, NAMESPACE_OSGEO_FDO_COMMANDS::OrderingOption  option )
{
	EXCEPTION_HANDLER(GetImpObj()->SetOrderingOption(StringToUni(propertyName), static_cast<FdoOrderingOption>(option)))
}

NAMESPACE_OSGEO_FDO_COMMANDS::OrderingOption NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IExtendedSelectImp::GetOrderingOption( System::String* propertyName )
{
	FdoOrderingOption result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetOrderingOption(StringToUni(propertyName)));

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS::OrderingOption>(result);
}

void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IExtendedSelectImp::ClearOrderingOptions()
{
	EXCEPTION_HANDLER(GetImpObj()->ClearOrderingOptions());
}

NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReader* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IExtendedSelectImp::ExecuteScrollable()
{
	FdoIScrollableFeatureReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->ExecuteScrollable())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIScrollableFeatureReader(result, true);
}

