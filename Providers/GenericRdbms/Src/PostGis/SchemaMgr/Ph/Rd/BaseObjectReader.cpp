/*
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 *
 */

#include "stdafx.h"
#include "BaseObjectReader.h"
#include <Sm/Ph/Rd/SchemaDbObjectBinds.h>
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"

FdoSmPhRdPostGisBaseObjectReader::FdoSmPhRdPostGisBaseObjectReader(
    FdoSmPhOwnerP owner
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, owner)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdPostGisBaseObjectReader::FdoSmPhRdPostGisBaseObjectReader(
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, dbObject)
{
    FdoSmPhOwnerP owner = FDO_SAFE_ADDREF((FdoSmPhOwner*)(FdoSmPhDbElement*)dbObject->GetParent());
    FdoStringsP objectNames = FdoStringCollection::Create();
    objectNames->Add(dbObject->GetName());
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdPostGisBaseObjectReader::FdoSmPhRdPostGisBaseObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, owner)
{
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdPostGisBaseObjectReader::FdoSmPhRdPostGisBaseObjectReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, owner)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    SetSubReader(MakeQueryReader(owner,objectNames,join));
}

FdoSmPhRdPostGisBaseObjectReader::~FdoSmPhRdPostGisBaseObjectReader(void)
{
}

FdoSmPhReaderP FdoSmPhRdPostGisBaseObjectReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join
)
{
    FdoStringP           ownerName = owner->GetName();
    FdoSmPhMgrP          mgr = owner->GetManager();
    FdoSmPhPostGisMgrP   pgMgr = mgr->SmartCast<FdoSmPhPostGisMgr>();

    FdoStringP sqlString = FdoStringP::Format(
        L"select %ls (NS.nspname || '.' || S.relname) as name, B.relname as base_name,\n"
        L" NB.nspname as base_schema, cast(null as varchar) as base_database, \n"
        L" cast('%ls' as varchar) as base_owner, "
        L" %ls as collate_schema_name, "
        L" %ls as collate_name "
        L" from pg_namespace NS, pg_namespace NB, pg_class S, pg_class B, pg_inherits I $(JOIN_FROM)\n"
        L" where \n"
        L" I.inhrelid = S.oid and I.inhparent = B.oid\n"
        L" and S.relnamespace = NS.oid and B.relnamespace = NB.oid\n"
        L" $(AND) $(QUALIFICATION) \n"
        L" order by collate_schema_name, collate_name, I.inhseqno asc",
        join ? L"distinct" : L"",
        (FdoString*) owner->GetName(),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"NS.nspname"),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"S.relname")
    );

    FdoSmPhReaderP reader = FdoSmPhRdBaseObjectReader::MakeQueryReader(
        L"",
        owner,
        sqlString,
        L"NS.nspname",
        L"S.relname",
        objectNames,
        join
    );

    return reader;
}
