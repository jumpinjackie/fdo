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
#include <Fdo\Schema\PropertyDefinition.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\PropertyType.h>

#include "FDO\Schema\mgPropertyDefinition.h"

FdoPropertyDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition::GetImpObj()
{
	return static_cast<FdoPropertyDefinition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition::PropertyType::get()
{
	FdoPropertyType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(result);
}

System::String^ NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition::QualifiedName::get()
{
	FdoStringP result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetQualifiedName())

	return CHECK_STRING((FdoString*)result);
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition::IsSystem::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetIsSystem())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition::IsSystem::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIsSystem(value))
}
