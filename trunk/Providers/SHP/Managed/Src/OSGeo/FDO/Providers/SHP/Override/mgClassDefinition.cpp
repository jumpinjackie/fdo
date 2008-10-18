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
#include "FdoShpClassDefinition.h"

#include "FDO\Providers\SHP\Override\mgClassDefinition.h"
#include "FDO\Providers\SHP\Override\mgObjectFactory.h"
#include "FDO\Providers\SHP\Override\mgPropertyDefinition.h"
#include "FDO\Providers\SHP\Override\mgPropertyDefinitionCollection.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassDefinition::ClassDefinition() : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoShpOvClassDefinition::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassDefinition::ClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping(unmanaged, autoDelete)
{

}

FdoShpOvClassDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassDefinition::GetImpObj()
{
	return static_cast<FdoShpOvClassDefinition*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassDefinition::get_ShapeFile()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetShapeFile())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassDefinition::set_ShapeFile(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetShapeFile(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassDefinition::get_Properties()
{
	FdoShpOvPropertyDefinitionCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetProperties())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ObjectFactory::CreatePropertyDefinitionCollection(result, true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassDefinition::FindByColumnName(System::String* value)
{
	FdoShpOvPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->FindByColumnName(StringToUni(value)))

	return NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ObjectFactory::CreatePropertyDefinition(result, true);
}
