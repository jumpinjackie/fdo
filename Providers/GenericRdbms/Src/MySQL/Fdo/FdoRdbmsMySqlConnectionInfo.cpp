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
#include "FdoRdbmsMySqlConnectionInfo.h"
#include "FdoRdbmsMySqlConnectionPropertyDictionary.h"
#include "FdoRdbmsMySqlProviderInfo.h"

FdoRdbmsMySqlConnectionInfo::FdoRdbmsMySqlConnectionInfo(FdoRdbmsConnection *connection) :
mPropertyDictionary(NULL),
mConnection(connection)
{

}

FdoRdbmsMySqlConnectionInfo::~FdoRdbmsMySqlConnectionInfo()
{
    FDO_SAFE_RELEASE(mPropertyDictionary);
}


const wchar_t* FdoRdbmsMySqlConnectionInfo::GetProviderName()
{
    return RDBMS_MYSQL_PROVIDER_NAME;
}

const wchar_t* FdoRdbmsMySqlConnectionInfo::GetProviderDisplayName()
{
    return NlsMsgGet(FDORDBMS_472, RDBMS_MYSQL_PROVIDER_DEFAULT_DISPLAY_NAME);
}

const wchar_t* FdoRdbmsMySqlConnectionInfo::GetProviderDescription()
{
    return NlsMsgGet(FDORDBMS_473, RDBMS_MYSQL_PROVIDER_DEFAULT_DESCRIPTION);
}

const wchar_t* FdoRdbmsMySqlConnectionInfo::GetProviderVersion()
{
    return RDBMS_MYSQL_PROVIDER_VERSION;
}


const wchar_t* FdoRdbmsMySqlConnectionInfo::GetFeatureDataObjectsVersion()
{
    return RDBMS_MYSQL_FDO_VERSION;
}

FdoIConnectionPropertyDictionary* FdoRdbmsMySqlConnectionInfo::GetConnectionProperties()
{
    if (mPropertyDictionary == NULL)
        mPropertyDictionary = (FdoIConnectionPropertyDictionary *)new FdoRdbmsMySqlConnectionPropertyDictionary(mConnection);
    FDO_SAFE_ADDREF(mPropertyDictionary);
    return mPropertyDictionary;
}

void FdoRdbmsMySqlConnectionInfo::Dispose()
{
    delete this;
}


