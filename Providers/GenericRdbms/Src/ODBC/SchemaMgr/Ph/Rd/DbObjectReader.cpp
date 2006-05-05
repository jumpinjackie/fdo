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
#include "DbObjectReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"
#include "Inc/Rdbi/proto.h"

FdoSmPhRdOdbcDbObjectReader::FdoSmPhRdOdbcDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringP objectName
) :
    FdoSmPhRdDbObjectReader(owner->GetManager(), MakeRows(owner->GetManager()), owner, objectName),
    m_objectType(FdoSmPhDbObjType_Unknown)
{
    FdoStringP           ownerName = owner->GetName();
    FdoSmPhMgrP          mgr = owner->GetManager();
    FdoSmPhOdbcMgr*      pMgr = (FdoSmPhOdbcMgr*)(FdoSmPhMgr*)mgr;

    rdbi_context = pMgr->GetRdbiContext();

    INVOKE_RDBI_FUNC(
        rdbi_objects_act(
            rdbi_context,
            (char*)(const char*) ownerName,
            (char*)(const char*) objectName
        )
    );
}

FdoSmPhRdOdbcDbObjectReader::~FdoSmPhRdOdbcDbObjectReader(void)
{
}

FdoSmPhDbObjType FdoSmPhRdOdbcDbObjectReader::GetType()
{
    return m_objectType;
}



bool FdoSmPhRdOdbcDbObjectReader::ReadNext()
{
    bool rc = true;
    char objectName[1000];
    char type[2];
    int eof;

    if ( IsEOF() ) 
        return false;

    // Get the next object

    INVOKE_RDBI_FUNC(
        rdbi_objects_get(
            rdbi_context,
            objectName,
            type,
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
        // Set the field values according to the current object.
        SetString( L"", L"name", FdoStringP(objectName) );

        if (type[0] == 'T')
            m_objectType = FdoSmPhDbObjType_Table;
        else if (type[0] == 'V')
            m_objectType = FdoSmPhDbObjType_View;
        else
            m_objectType = FdoSmPhDbObjType_Unknown;

        SetBOF(false);
    }

    return rc;
}

