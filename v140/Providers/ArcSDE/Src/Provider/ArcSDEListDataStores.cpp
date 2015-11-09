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
#include "ArcSDEListDataStores.h"
#include "ArcSDEDataStoreReader.h"


ArcSDEListDataStores::ArcSDEListDataStores() :
    ArcSDECommand<FdoIListDataStores>(NULL),
    m_bIncludeNonFdoDatastores(true)
{
}

ArcSDEListDataStores::ArcSDEListDataStores(FdoIConnection *connection) :
    ArcSDECommand<FdoIListDataStores>(connection),
    m_bIncludeNonFdoDatastores(true)
{
}

ArcSDEListDataStores::~ArcSDEListDataStores(void)
{
}

bool ArcSDEListDataStores::GetIncludeNonFdoEnabledDatastores()
{
    return m_bIncludeNonFdoDatastores;
}

void ArcSDEListDataStores::SetIncludeNonFdoEnabledDatastores(bool include)
{
    m_bIncludeNonFdoDatastores = include;
}

FdoIDataStoreReader* ArcSDEListDataStores::Execute()
{
    // For now, ignore m_bIncludeNonFdoDatastores since all datastores are of
    // the same kind (no FDO-specific system tables).

    // Create & return the DataStore reader:
    return new ArcSDEDataStoreReader(mConnection);
}

