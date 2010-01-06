/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include "CoordSysReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"
#include "../Database.h"
#include <FdoCommonStringUtil.h>

FdoSmPhRdSqsCoordSysReader::FdoSmPhRdSqsCoordSysReader(
    FdoSmPhOwnerP owner,
    FdoStringP csysName
) :
    FdoSmPhRdCoordSysReader((FdoSmPhReader*) NULL, owner, csysName)
{
    if ( csysName.IsNumber() ) {
        SetSubReader(MakeQueryReader(owner,L"",FdoCommonStringUtil::StringToInt64((FdoString*)csysName)));
    }
    else {
        SetSubReader(MakeQueryReader(owner,csysName,-1));
    }
}

FdoSmPhRdSqsCoordSysReader::FdoSmPhRdSqsCoordSysReader(
    FdoSmPhOwnerP owner,
    FdoInt64 srid
) :
    FdoSmPhRdCoordSysReader((FdoSmPhReader*) NULL, owner, L"")
{
    SetSubReader(MakeQueryReader(owner,L"",srid));
}

FdoSmPhRdSqsCoordSysReader::~FdoSmPhRdSqsCoordSysReader(void)
{
}

FdoSmPhReaderP FdoSmPhRdSqsCoordSysReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringP csysName,
    FdoInt64 srid
)
{
    FdoStringP           sqlString;
    FdoSmPhMgrP          mgr = owner->GetManager();

    FdoSmPhReaderP reader;
//TODO: cache the queries for performance
/*
    if ( object_set )
        reader = pMgr->GetCoordSysReader(dblink_set);
    else
        reader = pMgr->GetCoordSysReader(dblink_set);
*/
    if ( !reader ) {
        // Generate sql statement if not already done
        // Coordinate System name is not kept in its own column, so it is extracted from 
        // the WKT instead.
        sqlString = FdoStringP::Format(
            L"select SUBSTRING( well_known_text, 9, CHARINDEX('\"',well_known_text,9) - 9 ) as name, \n"
            L"well_known_text as wktext, spatial_reference_id as srid \n"
            L"from %ls.sys.spatial_reference_systems \n"
            L"%ls %ls",
            (FdoString *) owner->GetDbName(),
            ((csysName != L"") || (srid > -1)) ? L"where" : L"",
            (csysName != L"") ? 
                L"SUBSTRING( well_known_text, 9, CHARINDEX('\"',well_known_text,9) - 9 ) = ?" :
                ((srid > -1) ? L"spatial_reference_id = ?" : L"")
        );

        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);

        reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, MakeBinds(mgr,csysName,srid) );
/*
        if ( object_set )
            pMgr->SetCoordSysReader(reader, dblink_set);
        else
            pMgr->SetCoordSyssReader(reader, dblink_set);
*/
    }
    else {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
        FdoSmPhRowP binds = pReader->GetBinds();
		FdoSmPhFieldsP pFields = binds->GetFields();

        FdoSmPhFieldP bind = pFields->FindItem(L"name");        
        if ( bind )
            bind->SetFieldValue(csysName);

        bind = pFields->FindItem(L"srid");        
        if ( bind )
            bind->SetFieldValue(
                FdoStringP::Format(
#ifdef _WIN32
                    L"%I64d",
#else
                    L"%lld",
#endif
                    srid
                )
            );

        pReader->Execute();
    }

    return reader;
}


FdoSmPhRowP FdoSmPhRdSqsCoordSysReader::MakeBinds( 
    FdoSmPhMgrP mgr, 
    FdoStringP csysName, 
    FdoInt64 srid 
)
{
    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"Binds" );
    FdoSmPhDbObjectP rowObj = row->GetDbObject();
    FdoSmPhFieldP field;

    if ( csysName != L"" ) {
        field = new FdoSmPhField(
            row,
            L"name",
            rowObj->CreateColumnChar(L"name",false, 256)
        );
    
        field->SetFieldValue(csysName);
    }

    if ( srid > -1 ) {
        field = new FdoSmPhField(
            row,
            L"srid",
            rowObj->CreateColumnInt64(L"spatial_reference_id",false)
        );
    
        field->SetFieldValue(
            FdoStringP::Format(
#ifdef _WIN32
                L"%I64d",
#else
                L"%lld",
#endif
                srid
            )
        );
    }

    return( row );
}

