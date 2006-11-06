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
#include "PkeyReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"
#include "Inc/Rdbi/proto.h"

FdoSmPhRdOdbcPkeyReader::FdoSmPhRdOdbcPkeyReader(
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdPkeyReader(mgr, MakeRows(mgr)),
    mDbObject(dbObject)
{
    FdoSmPhOdbcMgr*      pMgr = (FdoSmPhOdbcMgr*)(FdoSmPhMgr*)mgr;

    rdbi_context = pMgr->GetRdbiContext();

    FdoStringP objectName = dbObject->GetName();
    FdoStringP ownerName = dbObject->GetOwner();

    if (rdbi_context->dispatch.capabilities.supports_unicode == 1)
        INVOKE_RDBI_FUNC(rdbi_pkeys_actW (rdbi_context, ownerName, objectName))
    else
        INVOKE_RDBI_FUNC(rdbi_pkeys_act (rdbi_context, ownerName, objectName))
}

FdoSmPhRdOdbcPkeyReader::~FdoSmPhRdOdbcPkeyReader(void)
{
}

bool FdoSmPhRdOdbcPkeyReader::ReadNext()
{
    bool rc = true;
    int eof;
    FdoStringP pColumnName;
    if ( IsEOF() ) 
        return false;

    wchar_t columnNameBuf[1000];
    rdbi_string_def columnName;
    columnName.wString = columnNameBuf;
    *columnName.wString = L'\0';

    // Get the next key column name
    if (rdbi_context->dispatch.capabilities.supports_unicode == 1)
    {
        INVOKE_RDBI_FUNC(rdbi_pkeys_getW (rdbi_context, columnName.wString, &eof))
        pColumnName = columnName.cwString;
    }
    else
    {
        INVOKE_RDBI_FUNC(rdbi_pkeys_get (rdbi_context, columnName.cString, &eof))
        pColumnName = columnName.ccString;
    }

    // Check if we're done
    if ( eof )
    {
        SetEOF(true);
        rc = false;
    }
    else
    {
        FdoStringP tableName = mDbObject->GetName();
        // Just invent a constraint name. ODBC doesn't really have them.
        FdoStringP constraintName = L"Pk";
        constraintName += tableName;

        // Set the field values according to the current key column.
        SetString( L"", L"constraint_name", FdoStringP(constraintName) );
        SetString( L"", L"table_name", FdoStringP(tableName) );
        SetString( L"", L"column_name", pColumnName );

        SetBOF(false);
    }

    return rc;
}
