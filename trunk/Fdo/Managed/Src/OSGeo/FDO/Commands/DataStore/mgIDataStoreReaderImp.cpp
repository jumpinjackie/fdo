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
#include <Fdo\Commands\DataStore\IDataStorePropertyDictionary.h>
#include <Fdo\Commands\DataStore\IDataStoreReader.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\DataStore\mgIDataStoreReaderImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\DataStore\mgIDataStorePropertyDictionaryImp.h"

NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReaderImp::IDataStoreReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

FdoIDataStoreReader* NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReaderImp::GetImpObj()
{
    return static_cast<FdoIDataStoreReader*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReaderImp::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReaderImp::GetName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return result;
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReaderImp::GetDescription()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReaderImp::GetIsFdoEnabled()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIsFdoEnabled())

	return result;
}

NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionary* NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReaderImp::GetDataStoreProperties()
{
	FdoIDataStorePropertyDictionary* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDataStoreProperties())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDataStorePropertyDictionary(result, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReaderImp::ReadNext()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = GetImpObj()->ReadNext())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReaderImp::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}

