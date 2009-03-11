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
#include <Fdo\Commands\Feature\IBaseSelect.h>
#include <Fdo\Commands\OrderingOption.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Feature\mgIBaseSelectImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\mgIdentifierCollection.h"
#include "FDO\Commands\mgOrderingOption.h"

FdoIBaseSelect* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IBaseSelectImp::GetImpObj()
{
    return static_cast<FdoIBaseSelect*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_COMMANDS::IdentifierCollection^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IBaseSelectImp::PropertyNames::get()
{
	FdoIdentifierCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyNames())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIdentifierCollection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_COMMANDS::IdentifierCollection^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IBaseSelectImp::Ordering::get()
{
	FdoIdentifierCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetOrdering())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIdentifierCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IBaseSelectImp::OrderingOption::set(NAMESPACE_OSGEO_FDO_COMMANDS::OrderingOption value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOrderingOption(static_cast<FdoOrderingOption>(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS::OrderingOption NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IBaseSelectImp::OrderingOption::get()
{
	FdoOrderingOption result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetOrderingOption())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS::OrderingOption>(result);
}
