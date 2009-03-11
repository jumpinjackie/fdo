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
#include <Fdo\Schema\NetworkLayerClass.h>

#include "FDO\Schema\mgNetworkLayerClass.h"

NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass::NetworkLayerClass() : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(System::IntPtr::Zero, false)
{
    EXCEPTION_HANDLER(Attach(IntPtr(FdoNetworkLayerClass::Create()), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass::NetworkLayerClass(System::String^ name, System::String^ description) : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoNetworkLayerClass::Create(StringToUni(name), StringToUni(description))), true))
}

FdoNetworkLayerClass* NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass::GetImpObj()
{
	return static_cast<FdoNetworkLayerClass*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassType NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass::ClassType::get()
{
	FdoClassType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::ClassType>(result);
}
