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
#include <Fdo\Schema\TopoFeaturePropertyDefinition.h>

#include "FDO\Schema\mgTopoFeaturePropertyDefinition.h"

NAMESPACE_OSGEO_FDO_SCHEMA::TopoFeaturePropertyDefinition::TopoFeaturePropertyDefinition(System::String^ name, System::String^ description) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoTopoFeaturePropertyDefinition::Create(StringToUni(name), StringToUni(description))), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::TopoFeaturePropertyDefinition::TopoFeaturePropertyDefinition(System::String^ name, System::String^ description, System::Boolean system) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoTopoFeaturePropertyDefinition::Create(StringToUni(name), StringToUni(description), system)), true))
}

FdoTopoFeaturePropertyDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::TopoFeaturePropertyDefinition::GetImpObj()
{
	return static_cast<FdoTopoFeaturePropertyDefinition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::TopoFeaturePropertyDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}
