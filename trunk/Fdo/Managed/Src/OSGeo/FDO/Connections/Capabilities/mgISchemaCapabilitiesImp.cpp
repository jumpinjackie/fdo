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

#include "FDO\Connections\Capabilities\mgISchemaCapabilitiesImp.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::ISchemaCapabilitiesImp(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoISchemaCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::GetImpObj()
{
    return static_cast<FdoISchemaCapabilities*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_ClassTypes() []
{
	FdoClassType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetClassTypes(unlength))
	NAMESPACE_OSGEO_FDO_SCHEMA::ClassType mtype [] = new NAMESPACE_OSGEO_FDO_SCHEMA::ClassType[unlength];
	for(FdoInt32 i = 0; i < unlength; i ++)
	{
		mtype[i] = static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::ClassType>(*(unobj + i));
	}
	return mtype;
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_DataTypes() []
{
	FdoDataType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataTypes(unlength))
	NAMESPACE_OSGEO_FDO_SCHEMA::DataType mtype [] = new NAMESPACE_OSGEO_FDO_SCHEMA::DataType[unlength];
	for(FdoInt32 i = 0; i < unlength; i ++)
	{
		mtype[i] = static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(*(unobj + i));
	}
	return mtype;
}

System::Int64 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_MaximumDataValueLength(NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType)
{
	FdoInt64 rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->GetMaximumDataValueLength((FdoDataType)dataType));
    return rv;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_MaximumDecimalPrecision()
{
	FdoInt32 rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->GetMaximumDecimalPrecision());
    return rv;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_MaximumDecimalScale()
{
	FdoInt32 rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->GetMaximumDecimalScale());
    return rv;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_NameSizeLimit(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SchemaElementNameType name)
{
	FdoInt32 rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->GetNameSizeLimit((FdoSchemaElementNameType) name));
    return rv;
}

System::String* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_ReservedCharactersForName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetReservedCharactersForName());

	return result;
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportedAutoGeneratedTypes() []
{
	FdoDataType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSupportedAutoGeneratedTypes(unlength))
	NAMESPACE_OSGEO_FDO_SCHEMA::DataType mtype [] = new NAMESPACE_OSGEO_FDO_SCHEMA::DataType[unlength];
	for(FdoInt32 i = 0; i < unlength; i ++)
	{
		mtype[i] = static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(*(unobj + i));
	}
	return mtype;
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportedIdentityPropertyTypes() []
{
	FdoDataType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSupportedIdentityPropertyTypes(unlength))
	NAMESPACE_OSGEO_FDO_SCHEMA::DataType mtype [] = new NAMESPACE_OSGEO_FDO_SCHEMA::DataType[unlength];
	for(FdoInt32 i = 0; i < unlength; i ++)
	{
		mtype[i] = static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(*(unobj + i));
	}
	return mtype;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsAssociationProperties()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsAssociationProperties())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsAutoIdGeneration()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsAutoIdGeneration())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsCompositeId()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->SupportsCompositeId())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsCompositeUniqueValueConstraints()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->SupportsCompositeUniqueValueConstraints())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsDataStoreScopeUniqueIdGeneration()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsDataStoreScopeUniqueIdGeneration())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsDefaultValue()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsDefaultValue())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsExclusiveValueRangeConstraints()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->SupportsExclusiveValueRangeConstraints())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsInclusiveValueRangeConstraints()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->SupportsInclusiveValueRangeConstraints())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsInheritance()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsInheritance())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsMultipleSchemas()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsMultipleSchemas())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsNetworkModel()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsNetworkModel())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsNullValueConstraints()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->SupportsNullValueConstraints())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsObjectProperties()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsObjectProperties())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsSchemaModification()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSchemaModification())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsSchemaOverrides()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSchemaOverrides())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsUniqueValueConstraints()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->SupportsUniqueValueConstraints())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp::get_SupportsValueConstraintsList()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = GetImpObj()->SupportsValueConstraintsList())
	return rv;
}

