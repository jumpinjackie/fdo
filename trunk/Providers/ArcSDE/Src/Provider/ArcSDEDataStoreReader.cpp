/*
 * 
* Copyright (C) 2004-2005  Autodesk, Inc.
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
#include "ArcSDEDataStoreReader.h"
#include <FdoCommonConnPropDictionary.h>

ArcSDEDataStoreReader::ArcSDEDataStoreReader(ArcSDEConnection* connection)
{
    mConnection = FDO_SAFE_ADDREF(connection);
    mDataStoreIndex = -1;

    // Grab the datastore list:
    FdoPtr<FdoIConnectionInfo> connInfo = connection->GetConnectionInfo();
    mConnProps = connInfo->GetConnectionProperties();
}

ArcSDEDataStoreReader::~ArcSDEDataStoreReader(void)
{
}

void ArcSDEDataStoreReader::Validate(void)
{
    if (mDataStoreIndex < 0)
        throw FdoException::Create (NlsMsgGet1(ARCSDE_READER_NOT_READY, "Must %1$ls prior to accessing reader.", L"ReadNext"));

    FdoInt32 dataStoreCount;
    mConnProps->EnumeratePropertyValues(CONNECTIONPROPERTY_DATASTORE, dataStoreCount);

    if (mDataStoreIndex >= dataStoreCount)
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_EXHAUSTED, "Reader is exhausted."));
}

FdoString* ArcSDEDataStoreReader::GetName()
{
    Validate();
    FdoInt32 dataStoreCount;
    return mConnProps->EnumeratePropertyValues(CONNECTIONPROPERTY_DATASTORE, dataStoreCount)[mDataStoreIndex];
}

FdoString* ArcSDEDataStoreReader::GetDescription()
{
    Validate();
    return L"";
}

bool ArcSDEDataStoreReader::GetIsFdoEnabled()
{
    Validate();
    return false; // No ArcSDE databases are 'fdo-enabled', they are all 'foreign' databases
}



FdoIDataStorePropertyDictionary* ArcSDEDataStoreReader::GetDataStoreProperties()
{
    FdoPtr<FdoCommonDataStorePropDictionary> dict = new FdoCommonDataStorePropDictionary(mConnection);
    char *mbPropName = NULL;
    FdoInt32 dataStoreCount;

    wide_to_multibyte(mbPropName, CONNECTIONPROPERTY_DATASTORE);
    FdoPtr<ConnectionProperty> prop = new ConnectionProperty(CONNECTIONPROPERTY_DATASTORE,
        NlsMsgGet(ARCSDE_CONNECTION_PROPERTY_DATASTORE, mbPropName),
        mConnProps->EnumeratePropertyValues(CONNECTIONPROPERTY_DATASTORE, dataStoreCount)[mDataStoreIndex],
        true, false, false, false, false, true, false, 0, NULL);
    dict->AddProperty(prop);

    return FDO_SAFE_ADDREF(dict.p);
}

bool ArcSDEDataStoreReader::ReadNext()
{
    mDataStoreIndex++;

    FdoInt32 dataStoreCount;
    mConnProps->EnumeratePropertyValues(CONNECTIONPROPERTY_DATASTORE, dataStoreCount);

    return (mDataStoreIndex < dataStoreCount);
}

void ArcSDEDataStoreReader::Close()
{
    // nothing to do here
}

void ArcSDEDataStoreReader::Dispose()
{
    delete this;
}
