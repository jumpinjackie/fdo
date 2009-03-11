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
#include <Fdo\Schema\NetworkLinkFeatureClass.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Schema\mgNetworkLinkFeatureClass.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgAssociationPropertyDefinition.h"

NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLinkFeatureClass::NetworkLinkFeatureClass() : NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoNetworkLinkFeatureClass::Create()), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLinkFeatureClass::NetworkLinkFeatureClass(System::String^ name, System::String^ description) : NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoNetworkLinkFeatureClass::Create(StringToUni(name), StringToUni(description))), true))
}

FdoNetworkLinkFeatureClass* NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLinkFeatureClass::GetImpObj()
{
	return static_cast<FdoNetworkLinkFeatureClass*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassType NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLinkFeatureClass::ClassType::get()
{
	FdoClassType result;
	
	EXCEPTION_HANDLER(result = GetImpObj()->GetClassType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::ClassType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLinkFeatureClass::StartNodeProperty::get()
{
	FdoAssociationPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetStartNodeProperty())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateAssociationPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLinkFeatureClass::StartNodeProperty::set(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetStartNodeProperty((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLinkFeatureClass::EndNodeProperty::get()
{
	FdoAssociationPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetEndNodeProperty())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateAssociationPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLinkFeatureClass::EndNodeProperty::set(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetEndNodeProperty((value == nullptr ? nullptr : value->GetImpObj())))
}

