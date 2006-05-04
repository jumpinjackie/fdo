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
#include <Fdo.h>

#include "mgIGeometryCapabilitiesImp.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilitiesImp::IGeometryCapabilitiesImp(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoIGeometryCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilitiesImp::GetImpObj()
{
    return static_cast<FdoIGeometryCapabilities*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilitiesImp::Dispose(Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}

NAMESPACE_OSGEO_COMMON::GeometryType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilitiesImp::get_GeometryTypes() []
{
	FdoGeometryType* unobj;
	FdoInt32 length;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGeometryTypes(length))
	NAMESPACE_OSGEO_COMMON::GeometryType obj [] = new NAMESPACE_OSGEO_COMMON::GeometryType[length];
	for(FdoInt32 i = 0; i < length; i ++)
	{
		obj[i] = static_cast<NAMESPACE_OSGEO_COMMON::GeometryType>(*(unobj + i));
	}
	return obj;
}

NAMESPACE_OSGEO_COMMON::GeometryComponentType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilitiesImp::get_GeometryComponentTypes() []
{
	FdoGeometryComponentType* unobj;
	FdoInt32 length;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGeometryComponentTypes(length))
    NAMESPACE_OSGEO_COMMON::GeometryComponentType obj [] = new NAMESPACE_OSGEO_COMMON::GeometryComponentType[length];
	for(FdoInt32 i = 0; i < length; i ++)
	{
		obj[i] = static_cast<NAMESPACE_OSGEO_COMMON::GeometryComponentType>(*(unobj + i));
	}
	return obj;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilitiesImp::get_Dimensionalities()
{
	FdoInt32 rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->GetDimensionalities())
	return rv;
}
