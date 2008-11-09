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
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\NetworkClass.h>

#include "FDO\Schema\mgNetworkClass.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgNetworkLayerClass.h"

NAMESPACE_OSGEO_FDO_SCHEMA::NetworkClass::NetworkClass() : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoNetworkClass::Create(), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::NetworkClass::NetworkClass(System::String* name, System::String* description) : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoNetworkClass::Create(StringToUni(name), StringToUni(description)), true))
}

FdoNetworkClass* NAMESPACE_OSGEO_FDO_SCHEMA::NetworkClass::GetImpObj()
{
	return static_cast<FdoNetworkClass*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassType NAMESPACE_OSGEO_FDO_SCHEMA::NetworkClass::get_ClassType()
{
	FdoClassType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::ClassType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass* NAMESPACE_OSGEO_FDO_SCHEMA::NetworkClass::get_LayerClass()
{
	FdoNetworkLayerClass* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLayerClass())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateNetworkLayerClass(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::NetworkClass::set_LayerClass(NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLayerClass((value == NULL ? NULL : value->GetImpObj())))
}

