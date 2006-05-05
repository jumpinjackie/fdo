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
#include "ColumnReader.h"
#include "../ColTypeMapper.h"
#include "../Mgr.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "Inc/Rdbi/proto.h"

FdoSmPhRdOdbcColumnReader::FdoSmPhRdOdbcColumnReader(
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdColumnReader(mgr, MakeRows(mgr), dbObject)
{
    FdoSmPhOdbcMgr*      pMgr = (FdoSmPhOdbcMgr*)(FdoSmPhMgr*)mgr;

    rdbi_context = pMgr->GetRdbiContext();

    FdoStringP objectName = dbObject->GetName();

    INVOKE_RDBI_FUNC(
        rdbi_col_act(
            rdbi_context,
            NULL,
            (char*)(const char*) objectName,
            NULL
        )
    );
}

FdoSmPhRdOdbcColumnReader::~FdoSmPhRdOdbcColumnReader(void)
{
}

bool FdoSmPhRdOdbcColumnReader::ReadNext()
{
    bool rc = true;
    char columnName[1000];
    char type[1000];
    int  length;
    int  scale;
    int  nullable;
    int is_autoincrement;
    int  position;
    int eof;

    if ( IsEOF() ) 
        return false;

    // Get the next key column name

    INVOKE_RDBI_FUNC(
        rdbi_col_get(
            rdbi_context,
            columnName,
            type,
            &length,
            &scale,
            &nullable,
            &is_autoincrement,
            &position,
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
        FdoStringP nameString = columnName;
        FdoStringP typeString = type;
        mColType = FdoSmPhOdbcColTypeMapper::String2Type( typeString, length, scale );
        SetString( FdoStringP::mEmptyString, L"name", nameString );
        SetInt64( FdoStringP::mEmptyString, "type", mColType);
        SetInt64( FdoStringP::mEmptyString, "size", length);
        SetInt64( FdoStringP::mEmptyString, "scale", scale);
        SetBoolean( FdoStringP::mEmptyString, "nullable", nullable ? true : false );
        SetBoolean( FdoStringP::mEmptyString, "is_autoincremented", is_autoincrement ? true : false );

        SetBOF(false);
    }

    return rc;
}


FdoSmPhColType FdoSmPhRdOdbcColumnReader::GetType()
{
    CheckGet();

    return mColType;
}
