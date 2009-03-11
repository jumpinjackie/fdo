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
#include <Fdo\Commands\DataStore\IListDataStores.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\DataStore\mgIListDataStoresImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IListDataStoresImp::IListDataStoresImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
{

}

FdoIListDataStores* NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IListDataStoresImp::GetImpObj()
{
    return static_cast<FdoIListDataStores*>(__super::UnmanagedObject.ToPointer());
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IListDataStoresImp::IncludeNonFdoEnabledDatastores::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIncludeNonFdoEnabledDatastores())

    return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IListDataStoresImp::IncludeNonFdoEnabledDatastores::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIncludeNonFdoEnabledDatastores(value))
}

NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReader^ NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IListDataStoresImp::Execute()
{
	FdoIDataStoreReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->Execute())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDataStoreReader(IntPtr(result), true);
}
