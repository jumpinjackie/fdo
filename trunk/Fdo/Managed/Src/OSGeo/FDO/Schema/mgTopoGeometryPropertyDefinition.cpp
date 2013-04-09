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
#include <Fdo\Schema\TopoGeometryPropertyDefinition.h>

#include "FDO\Schema\mgTopoGeometryPropertyDefinition.h"

FdoTopoGeometryPropertyDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::TopoGeometryPropertyDefinition::GetImpObj()
{
	return static_cast<FdoTopoGeometryPropertyDefinition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::TopoGeometryPropertyDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}
