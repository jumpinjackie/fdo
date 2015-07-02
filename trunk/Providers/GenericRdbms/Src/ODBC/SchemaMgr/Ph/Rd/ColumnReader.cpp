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
    FdoSmPhRdColumnReader(mgr, (FdoSmPhRowCollection*) NULL, dbObject)
{
    SetRows(MakeRows(mgr));
    FdoSmPhOdbcMgr*      pMgr = (FdoSmPhOdbcMgr*)(FdoSmPhMgr*)mgr;

    rdbi_context = pMgr->GetRdbiContext();

    FdoStringP objectName = dbObject->GetName();
    FdoStringP ownerName = dbObject->GetOwner();

    SetString( FdoStringP::mEmptyString, L"table_name", objectName );
    if (rdbi_context->dispatch.capabilities.supports_unicode == 1){
        INVOKE_RDBI_FUNC(rdbi_col_actW(rdbi_context, ownerName, objectName, NULL));
    }else{
        INVOKE_RDBI_FUNC(rdbi_col_act(rdbi_context, ownerName, objectName, NULL));
    }
}

FdoSmPhRdOdbcColumnReader::~FdoSmPhRdOdbcColumnReader(void)
{
}

bool FdoSmPhRdOdbcColumnReader::ReadNext()
{
    bool rc = true;
    int  length;
    int  scale;
    int  nullable;
    int is_autoincrement;
    int  position;
    int eof;
    FdoStringP nameString;
    FdoStringP typeString;

    if ( IsEOF() ) 
        return false;
    
    wchar_t columnNameBuf[1000];
    wchar_t typeBuf[1000];
    rdbi_string_def columnName;
    rdbi_string_def type;
    columnName.wString = columnNameBuf;
    type.wString = typeBuf;
    *columnName.wString = *type.wString = L'\0';

    // Get the next key column name
    if (rdbi_context->dispatch.capabilities.supports_unicode == 1)
    {
        INVOKE_RDBI_FUNC(rdbi_col_getW(
            rdbi_context,
            columnName.wString,
            type.wString,
            &length,
            &scale,
            &nullable,
            &is_autoincrement,
            &position,
            &eof));

        nameString = columnName.cwString;
        typeString = type.cwString;
    }
    else
    {
        INVOKE_RDBI_FUNC(rdbi_col_get(
            rdbi_context,
            columnName.cString,
            type.cString,
            &length,
            &scale,
            &nullable,
            &is_autoincrement,
            &position,
            &eof));
        
        nameString = columnName.ccString;
        typeString = type.ccString;
    }
    // Check if we're done
    if ( eof )
    {
        SetEOF(true);
        rc = false;
    }
    else
    {
        mColType = FdoSmPhOdbcColTypeMapper::String2Type( typeString, length, scale );
        SetString( FdoStringP::mEmptyString, L"name", nameString );
        SetInt64( FdoStringP::mEmptyString, "type", mColType);
        SetInt64( FdoStringP::mEmptyString, "size", length);
        SetInt64( FdoStringP::mEmptyString, "scale", scale);
        SetBoolean( FdoStringP::mEmptyString, "nullable", nullable ? true : false );
        SetBoolean( FdoStringP::mEmptyString, "is_autoincremented", is_autoincrement ? true : false );
		SetString( FdoStringP::mEmptyString, L"default_value", L"");
        SetBOF(false);
    }

    return rc;
}


FdoSmPhColType FdoSmPhRdOdbcColumnReader::GetType()
{
    CheckGet();

    return mColType;
}
