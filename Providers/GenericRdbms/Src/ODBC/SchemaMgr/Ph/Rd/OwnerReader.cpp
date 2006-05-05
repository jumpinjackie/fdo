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

    INVOKE_RDBI_FUNC(
        rdbi_users_act(
            rdbi_context,
            (char*)(const char*) ownerName
        )
    );
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
    char ownerName[1000];
    int eof;

    if ( IsEOF() ) 
        return false;

    // Get the next owner

    INVOKE_RDBI_FUNC(
        rdbi_users_get(
            rdbi_context,
            ownerName,
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
        // Set the field values according to the current owner.
        SetString( L"", L"name", FdoStringP(ownerName) );

        SetBOF(false);
    }

    return rc;
}
