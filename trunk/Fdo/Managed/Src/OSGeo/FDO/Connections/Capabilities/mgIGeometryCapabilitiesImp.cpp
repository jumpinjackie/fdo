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
    return static_cast<FdoIGeometryCapabilities*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilitiesImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

array<NAMESPACE_OSGEO_COMMON::GeometryType>^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilitiesImp::GeometryTypes::get()
{
	FdoGeometryType* unobj;
	FdoInt32 length;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGeometryTypes(length))

	array<NAMESPACE_OSGEO_COMMON::GeometryType>^ result = gcnew array<NAMESPACE_OSGEO_COMMON::GeometryType>(length);
	for(FdoInt32 i = 0; i < length; i ++)
		result[i] = static_cast<NAMESPACE_OSGEO_COMMON::GeometryType>(*(unobj + i));

    return result;
}

array<NAMESPACE_OSGEO_COMMON::GeometryComponentType>^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilitiesImp::GeometryComponentTypes::get()
{
	FdoGeometryComponentType* unobj;
	FdoInt32 length;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGeometryComponentTypes(length))

	array<NAMESPACE_OSGEO_COMMON::GeometryComponentType>^ result = gcnew array<NAMESPACE_OSGEO_COMMON::GeometryComponentType>(length);
	for(FdoInt32 i = 0; i < length; i ++)
		result[i] = static_cast<NAMESPACE_OSGEO_COMMON::GeometryComponentType>(*(unobj + i));

    return result;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilitiesImp::Dimensionalities::get()
{
	FdoInt32 rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->GetDimensionalities())
	return rv;
}
