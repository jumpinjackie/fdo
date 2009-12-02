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
#include "Fdo.h"
#include "SHP\FdoShpProvider.h"
#include "FdoShpPropertyDefinition.h"

#include "FDO\Providers\SHP\Override\mgPropertyDefinition.h"
#include "FDO\Providers\SHP\Override\mgObjectFactory.h"
#include "FDO\Providers\SHP\Override\mgColumnDefinition.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinition::PropertyDefinition() : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalPropertyMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoShpOvPropertyDefinition::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinition::PropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalPropertyMapping(unmanaged, autoDelete)
{

}

FdoShpOvPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinition::GetImpObj()
{
	return static_cast<FdoShpOvPropertyDefinition*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinition::set_Column(NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ColumnDefinition* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetColumn(value->GetImpObj()))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ColumnDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinition::get_Column()
{
	FdoShpOvColumnDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetColumn())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ObjectFactory::CreateColumnDefinition(result, true);
}
