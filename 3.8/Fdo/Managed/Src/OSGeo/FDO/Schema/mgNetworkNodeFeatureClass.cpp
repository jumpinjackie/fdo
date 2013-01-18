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
#include <Fdo\Schema\NetworkNodeFeatureClass.h>

#include "FDO\Schema\mgNetworkNodeFeatureClass.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgAssociationPropertyDefinition.h"


NAMESPACE_OSGEO_FDO_SCHEMA::NetworkNodeFeatureClass::NetworkNodeFeatureClass() : NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoNetworkNodeFeatureClass::Create()), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::NetworkNodeFeatureClass::NetworkNodeFeatureClass(System::String^ name, System::String^ description) : NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoNetworkNodeFeatureClass::Create(StringToUni(name), StringToUni(description))), true))
}

FdoNetworkNodeFeatureClass* NAMESPACE_OSGEO_FDO_SCHEMA::NetworkNodeFeatureClass::GetImpObj()
{
	return static_cast<FdoNetworkNodeFeatureClass*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::NetworkNodeFeatureClass::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassType NAMESPACE_OSGEO_FDO_SCHEMA::NetworkNodeFeatureClass::ClassType::get()
{
	FdoClassType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::ClassType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::NetworkNodeFeatureClass::LayerProperty::get()
{
	FdoAssociationPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLayerProperty())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateAssociationPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::NetworkNodeFeatureClass::LayerProperty::set(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLayerProperty((value == nullptr ? nullptr : value->GetImpObj())))
}
