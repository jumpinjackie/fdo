/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
*
*/

#include "stdafx.h"
#include "Fdo.h"
#include "FdoWmsOvClassDefinition.h"

#include "FDO\Providers\WMS\Override\mgOvClassDefinition.h"
#include "FDO\Providers\WMS\Override\mgObjectFactory.h"
#include "FDO\Providers\WMS\Override\mgOvRasterDefinition.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassDefinition::OvClassDefinition() : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoWmsOvClassDefinition::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassDefinition::OvClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping(unmanaged, autoDelete)
{

}

FdoWmsOvClassDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassDefinition::GetImpObj()
{
	return static_cast<FdoWmsOvClassDefinition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassDefinition::RasterDefinition::get()
{
	FdoWmsOvRasterDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetRasterDefinition())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvRasterDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassDefinition::RasterDefinition::set(NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetRasterDefinition(value->GetImpObj()))
}
