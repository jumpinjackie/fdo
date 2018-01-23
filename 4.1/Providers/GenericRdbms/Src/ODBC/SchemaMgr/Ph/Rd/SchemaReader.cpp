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
#include "SchemaReader.h"
#include "../Mgr.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "Inc/Rdbi/proto.h"

FdoSmPhRdOdbcSchemaReader::FdoSmPhRdOdbcSchemaReader(FdoSmPhRowsP rows, FdoSmPhOwnerP owner, bool dsInfo) :
	FdoSmPhRdSchemaReader(rows, owner, dsInfo),
    mRows(rows),
    mOwner(owner)
{
    FdoStringP           ownerName = owner->GetName();
    FdoSmPhMgrP          mgr = owner->GetManager();
    FdoSmPhOdbcMgr*       pMgr = (FdoSmPhOdbcMgr*)(FdoSmPhMgr*)mgr;

    rdbi_context = pMgr->GetRdbiContext();

    if (rdbi_context->dispatch.capabilities.supports_unicode == 1)
        INVOKE_RDBI_FUNC(rdbi_users_actW (rdbi_context, ownerName))
    else
        INVOKE_RDBI_FUNC(rdbi_users_act (rdbi_context, ownerName))
}


FdoSmPhRdOdbcSchemaReader::~FdoSmPhRdOdbcSchemaReader(void)
{
}

bool FdoSmPhRdOdbcSchemaReader::ReadNext()
{
    bool rc = true;
    FdoStringP pName;
    int eof;

    if ( IsEOF() ) 
        return false;

    wchar_t nameBuf[1000];
    rdbi_string_def name;
    name.wString = nameBuf;
    *name.wString = L'\0';

    // Get the next owner
    if (rdbi_context->dispatch.capabilities.supports_unicode == 1)
    {
        INVOKE_RDBI_FUNC(rdbi_users_getW (rdbi_context, name.wString, &eof))
        pName = name.cwString;
    }
    else
    {
        INVOKE_RDBI_FUNC(rdbi_users_get( rdbi_context, name.cString, &eof))
        pName = name.ccString;
    }
    // Check if we're done
    if ( eof )
    {
        SetEOF(true);
        rc = false;
    }
    else
    {
        // Set the field values according to the current schema.
        if (pName.GetLength() == 0)
        {
            // This ODBC driver doesn't support schemas, so assign 
            // a default value.
            SetString( L"", L"schemaname", mOwner->GetBestSchemaName() );
        }
        else
        {
            SetString( L"", L"schemaname", pName );
            SetString( L"", L"tableowner", pName );
        }

        SetBOF(false);
    }

    return rc;
}
