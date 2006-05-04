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

#include "FDO\Commands\DataStore\mgIDataStorePropertyDictionaryImp.h"

FdoIDataStorePropertyDictionary* NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionaryImp::GetImpObj()
{
    return static_cast<FdoIDataStorePropertyDictionary*>(__super::UnmanagedObject.ToPointer());
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionaryImp::IsPropertyFileName(System::String* name)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->IsPropertyFileName(StringToUni(name)))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionaryImp::IsPropertyFilePath(System::String* name)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->IsPropertyFilePath(StringToUni(name)))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionaryImp::Dispose()
{
	NAMESPACE_OSGEO_RUNTIME::Disposable::Dispose();
}
