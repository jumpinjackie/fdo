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

    INVOKE_RDBI_FUNC(
        rdbi_users_act(
            rdbi_context,
            (char*)(const char*) ownerName
        )
    );
}


FdoSmPhRdOdbcSchemaReader::~FdoSmPhRdOdbcSchemaReader(void)
{
}

bool FdoSmPhRdOdbcSchemaReader::ReadNext()
{
    bool rc = true;
    char name[1000];
    int eof;

    if ( IsEOF() ) 
        return false;

    // Get the next owner

    INVOKE_RDBI_FUNC(
        rdbi_users_get(
            rdbi_context,
            name,
            &eof
        )
    );

    // Check if we're done
    if ( eof )
    {
        SetEOF(true);
        rc = false;
    }
    else
    {
        // Set the field values according to the current schema.
        if (name[0] == '\0')
        {
            // This ODBC driver doesn't support schemas, so assign 
            // a default value.
            SetString( L"", L"schemaname", mOwner->GetBestSchemaName() );
        }
        else
        {
            SetString( L"", L"schemaname", FdoStringP(name) );
            SetString( L"", L"tableowner", FdoStringP(name) );
        }

        SetBOF(false);
    }

    return rc;
}
