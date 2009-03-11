/*
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
*
*/

#include "stdafx.h"
#include "Fdo.h"
#include "FdoWmsOvLayerDefinition.h"

#include "FDO\Providers\WMS\Override\mgOvLayerDefinition.h"
#include "FDO\Providers\WMS\Override\mgObjectFactory.h"
#include "FDO\Providers\WMS\Override\mgOvStyleDefinition.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition::OvLayerDefinition() : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoWmsOvLayerDefinition::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition::OvLayerDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
{

}

FdoWmsOvLayerDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition::GetImpObj()
{
	return static_cast<FdoWmsOvLayerDefinition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvStyleDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition::Style::get()
{
	FdoWmsOvStyleDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetStyle())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvStyleDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition::Style::set(NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvStyleDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetStyle(value->GetImpObj()))
}