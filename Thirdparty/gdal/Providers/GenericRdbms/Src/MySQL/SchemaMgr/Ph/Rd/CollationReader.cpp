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
#include "CollationReader.h"
#include "../Mgr.h"
#include "../Database.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdMySqlCollationReader::FdoSmPhRdMySqlCollationReader (
    FdoSmPhDatabaseP database, 
    FdoStringP collationName
) :
    FdoSmPhRdCollationReader(MakeQueryReader(database, collationName), collationName)
{
}

FdoSmPhRdMySqlCollationReader::~FdoSmPhRdMySqlCollationReader(void)
{
}

FdoSmPhReaderP FdoSmPhRdMySqlCollationReader::MakeQueryReader (
    FdoSmPhDatabaseP database, 
    FdoStringP collationName
)
{
    FdoStringP sql;
    FdoSmPhRowsP rows;
    FdoSmPhRowP row;
    FdoSmPhRowP binds;
    FdoSmPhMgrP mgr = database->GetManager();

    if ( wcslen(database->GetName()) != 0 )
        throw FdoSchemaException::Create( NlsMsgGet(FDORDBMS_4, "Collations can only be retrieved from current MySQL server"));

    FdoSmPhReaderP reader;
    // todo: cache the reader
    if (!reader)
    {
        // Generate sql statement if not already done
        FdoStringP qualification;

        if ( collationName != L"" ) {
            // Selecting single object, qualify by this object.
            qualification = L"  where collation_name collate utf8_bin = ?\n";
        } 

        sql = FdoStringP::Format (
              L"select collation_name, character_set_name\n"
              L" from information_schema.collations\n"
              L" %ls"
              L" order by collation_name collate utf8_bin",
              (FdoString*) qualification
        );

        rows = MakeRows (mgr);
        row = rows->GetItem (0);

        reader = new FdoSmPhRdGrdQueryReader (row, sql, mgr, MakeBinds (mgr, collationName));

    }
    else {
        FdoSmPhRdGrdQueryReader* pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
        if ( collationName != L"" ) {
            // Re-executing so update bind variables first.
            binds = pReader->GetBinds ();
		    FdoSmPhFieldsP	fields = binds->GetFields();

            FdoSmPhFieldP(fields->GetItem (L"collation_name"))->SetFieldValue (collationName);
        }

        pReader->Execute();
    }

    return reader;
}

FdoSmPhRowP FdoSmPhRdMySqlCollationReader::MakeBinds (FdoSmPhMgrP mgr, FdoStringP collationName)
{
    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"Binds" );
    FdoSmPhDbObjectP rowObj = row->GetDbObject();

    if ( collationName != L"" ) {
        FdoSmPhFieldP field = new FdoSmPhField(
            row,
            L"collation_name",
            rowObj->CreateColumnDbObject(L"collation_name",false)
        );

        field->SetFieldValue(collationName);
    }

    return( row );
}
