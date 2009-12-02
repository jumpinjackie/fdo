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

#include "FDO\Connections\Capabilities\mgIFilterCapabilitiesImp.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::IFilterCapabilitiesImp(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoIFilterCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::GetImpObj()
{
    return static_cast<FdoIFilterCapabilities*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

NAMESPACE_OSGEO_FDO_FILTER::ConditionType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::get_ConditionTypes() []
{
	FdoConditionType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetConditionTypes(unlength))
	NAMESPACE_OSGEO_FDO_FILTER::ConditionType rv [] = new NAMESPACE_OSGEO_FDO_FILTER::ConditionType[unlength];
	for(FdoInt32 i = 0; i < unlength; i++)
	{
		rv[i] = static_cast<NAMESPACE_OSGEO_FDO_FILTER::ConditionType>(*(unobj + i));
	}
	return rv;
}

NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::get_SpatialOperations() []
{
	FdoSpatialOperations* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpatialOperations(unlength))
	NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations rv [] = new NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations[unlength];
	for(FdoInt32 i = 0; i < unlength; i++)
	{
		rv[i] = static_cast<NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations>(*(unobj + i));
	}
	return rv;
}

NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::get_DistanceOperations() []
{
	FdoDistanceOperations* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDistanceOperations(unlength))
	NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations rv [] = new NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations[unlength];
	for(FdoInt32 i = 0; i < unlength; i++)
	{
		rv[i] = static_cast<NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations>(*(unobj + i));
	}
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::SupportsNonLiteralGeometricOperations()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsNonLiteralGeometricOperations())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::SupportsGeodesicDistance()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsGeodesicDistance())
	return rv;
}
