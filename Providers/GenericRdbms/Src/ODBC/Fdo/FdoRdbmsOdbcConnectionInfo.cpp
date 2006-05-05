/*
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
#include "FdoRdbmsOdbcConnectionInfo.h"
#include "FdoRdbmsOdbcConnectionPropertyDictionary.h"
#include "FdoRdbmsOdbcProviderInfo.h"

FdoRdbmsOdbcConnectionInfo::FdoRdbmsOdbcConnectionInfo(FdoRdbmsConnection *connection) :
mPropertyDictionary(NULL),
mConnection(connection)
{

}

FdoRdbmsOdbcConnectionInfo::~FdoRdbmsOdbcConnectionInfo()
{
    FDO_SAFE_RELEASE(mPropertyDictionary);
}


const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderName()
{
    return RDBMS_ODBC_PROVIDER_NAME;
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderDisplayName()
{
    return NlsMsgGet(FDORDBMS_476, RDBMS_ODBC_PROVIDER_DEFAULT_DISPLAY_NAME);
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderDescription()
{
    return NlsMsgGet(FDORDBMS_477, RDBMS_ODBC_PROVIDER_DEFAULT_DESCRIPTION);
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderVersion()
{
    return RDBMS_ODBC_PROVIDER_VERSION;
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetFeatureDataObjectsVersion()
{
    return RDBMS_ODBC_FDO_VERSION;
}

FdoIConnectionPropertyDictionary* FdoRdbmsOdbcConnectionInfo::GetConnectionProperties()
{
    if (mPropertyDictionary == NULL)
        mPropertyDictionary = (FdoIConnectionPropertyDictionary *)new FdoRdbmsOdbcConnectionPropertyDictionary(mConnection);
    FDO_SAFE_ADDREF(mPropertyDictionary);
    return (FdoIConnectionPropertyDictionary *)mPropertyDictionary;
}

void FdoRdbmsOdbcConnectionInfo::Dispose()
{
    delete this;
}


