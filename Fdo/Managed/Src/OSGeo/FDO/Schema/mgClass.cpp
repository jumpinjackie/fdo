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
#include <Fdo\Schema\Class.h>

#include "FDO\Schema\mgClass.h"

NAMESPACE_OSGEO_FDO_SCHEMA::Class::Class() : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoClass::Create()), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::Class::Class(System::String^ name, System::String^ description) : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoClass::Create(StringToUni(name), StringToUni(description))), true))
}

FdoClass* NAMESPACE_OSGEO_FDO_SCHEMA::Class::GetImpObj()
{
	return static_cast<FdoClass*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassType NAMESPACE_OSGEO_FDO_SCHEMA::Class::ClassType::get()
{
	FdoClassType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::ClassType>(result);
}
