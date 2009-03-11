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

array<NAMESPACE_OSGEO_FDO_FILTER::ConditionType>^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::ConditionTypes::get()
{
	FdoConditionType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetConditionTypes(unlength))
	
    array<NAMESPACE_OSGEO_FDO_FILTER::ConditionType>^ result = gcnew array<NAMESPACE_OSGEO_FDO_FILTER::ConditionType>(unlength);
	for(FdoInt32 i = 0; i < unlength; i++)
		result[i] = static_cast<NAMESPACE_OSGEO_FDO_FILTER::ConditionType>(*(unobj + i));
	
    return result;
}

array<NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations>^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::SpatialOperations::get()
{
	FdoSpatialOperations* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpatialOperations(unlength))

	array<NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations>^ result = gcnew array<NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations>(unlength);
	for(FdoInt32 i = 0; i < unlength; i++)
		result[i] = static_cast<NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations>(*(unobj + i));
	
    return result;
}

array<NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations>^NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::DistanceOperations::get()
{
	FdoDistanceOperations* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDistanceOperations(unlength))

    array<NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations>^ result = gcnew array<NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations>(unlength);
	for(FdoInt32 i = 0; i < unlength; i++)
		result[i] = static_cast<NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations>(*(unobj + i));
	
    return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::SupportsNonLiteralGeometricOperations()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsNonLiteralGeometricOperations())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp::SupportsGeodesicDistance()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsGeodesicDistance())
	return rv;
}
