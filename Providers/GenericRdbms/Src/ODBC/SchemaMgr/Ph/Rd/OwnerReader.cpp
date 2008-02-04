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
#include "OwnerReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"
#include "Inc/Rdbi/proto.h"

FdoSmPhRdOdbcOwnerReader::FdoSmPhRdOdbcOwnerReader(
    FdoSmPhDatabaseP database,
    FdoStringP       ownerName
) :
	FdoSmPhRdOwnerReader(database->GetManager(), MakeRows(database->GetManager()), database, ownerName)
{
    FdoSmPhMgrP          mgr = database->GetManager();
    FdoSmPhOdbcMgr*      pMgr = (FdoSmPhOdbcMgr*)(FdoSmPhMgr*)mgr;

    rdbi_context = pMgr->GetRdbiContext();
    if (rdbi_context->dispatch.capabilities.supports_unicode == 1)
        INVOKE_RDBI_FUNC(rdbi_users_actW (rdbi_context, ownerName))
    else
        INVOKE_RDBI_FUNC(rdbi_users_act (rdbi_context, ownerName))
}

FdoSmPhRdOdbcOwnerReader::~FdoSmPhRdOdbcOwnerReader(void)
{
}


FdoBoolean FdoSmPhRdOdbcOwnerReader::GetHasMetaSchema()
{
    // ODBC Provider does not support metaschema.
    return false;
}

FdoStringP FdoSmPhRdOdbcOwnerReader::GetDescription()
{
    // ODBC does not support user descriptions.
    return FdoStringP::mEmptyString;
}


bool FdoSmPhRdOdbcOwnerReader::ReadNext()
{
    bool rc = true;
    FdoStringP pOwnerName;
    int eof;

    if ( IsEOF() ) 
        return false;

    wchar_t ownerNameBuf[1000];
    rdbi_string_def ownerName;
    ownerName.wString = ownerNameBuf;
    *ownerName.wString = L'\0';
    
    // Get the next owner
    if (rdbi_context->dispatch.capabilities.supports_unicode == 1)
    {
        INVOKE_RDBI_FUNC(rdbi_users_getW (rdbi_context, ownerName.wString, &eof))
        pOwnerName = ownerName.cwString;
    }
    else
    {
        INVOKE_RDBI_FUNC(rdbi_users_get (rdbi_context, ownerName.cString, &eof))
        pOwnerName = ownerName.ccString;
    }

    // Check if we're done
    if ( eof )
    {
        SetEOF(true);
        rc = false;
    }
    else
    {
        // Set the field values according to the current owner.
        SetString( L"", L"name", pOwnerName );

        SetBOF(false);
    }

    return rc;
}

FdoSmPhReaderP FdoSmPhRdOdbcOwnerReader::MakeHasMetaSchemaReader(
    FdoSmPhDatabaseP database,
    FdoStringP ownerName 
)
{
    // ODBC datastores never have MetaSchema
    return (FdoSmPhReader*) NULL;
}

