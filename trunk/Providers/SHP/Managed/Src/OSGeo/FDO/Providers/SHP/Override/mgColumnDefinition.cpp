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
#include "FdoShpColumnDefinition.h"

#include "FDO\Providers\SHP\Override\mgColumnDefinition.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ColumnDefinition::ColumnDefinition() : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoShpOvColumnDefinition::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ColumnDefinition::ColumnDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
{

}

FdoShpOvColumnDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ColumnDefinition::GetImpObj()
{
	return static_cast<FdoShpOvColumnDefinition*>(__super::UnmanagedObject.ToPointer());
}
