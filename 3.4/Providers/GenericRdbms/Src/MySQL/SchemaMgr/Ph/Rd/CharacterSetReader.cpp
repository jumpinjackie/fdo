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
#include "CharacterSetReader.h"
#include "../Mgr.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdMySqlCharacterSetReader::FdoSmPhRdMySqlCharacterSetReader (
    FdoSmPhDatabaseP database, 
    FdoStringP characterSetName
) :
    FdoSmPhRdCharacterSetReader(MakeQueryReader(database, characterSetName), characterSetName)
{
}

FdoSmPhRdMySqlCharacterSetReader::~FdoSmPhRdMySqlCharacterSetReader(void)
{
}

FdoSmPhReaderP FdoSmPhRdMySqlCharacterSetReader::MakeQueryReader (
    FdoSmPhDatabaseP database, 
    FdoStringP characterSetName
)
{
    FdoStringP sql;
    FdoSmPhRowsP rows;
    FdoSmPhRowP row;
    FdoSmPhRowP binds;
    FdoSmPhMgrP mgr = database->GetManager();

    if ( wcslen(database->GetName()) != 0 )
        throw FdoSchemaException::Create( NlsMsgGet(FDORDBMS_8, "Character Sets can only be retrieved from current MySQL server"));

    FdoSmPhReaderP reader;
    // todo: cache the reader
    if (!reader)
    {
        // Generate sql statement if not already done
        FdoStringP qualification;

        if ( characterSetName != L"" ) {
            // Selecting single object, qualify by this object.
            qualification = L"  where character_set_name collate utf8_bin = ?\n";
        } 

        sql = FdoStringP::Format (
              L"select character_set_name, maxlen\n"
              L" from information_schema.character_sets\n"
              L" %ls"
              L" order by character_set_name collate utf8_bin",
              (FdoString*) qualification
        );

        rows = MakeRows (mgr);
        row = rows->GetItem (0);

        FdoSmPhFieldP field = new FdoSmPhField(
            row, 
            L"maxlen",
            row->CreateColumnInt64(L"maxlen",false)
        );

        reader = new FdoSmPhRdGrdQueryReader (row, sql, mgr, MakeBinds (mgr, characterSetName));

    }
    else {
        FdoSmPhRdGrdQueryReader* pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
        if ( characterSetName != L"" ) {
            // Re-executing so update bind variables first.
            binds = pReader->GetBinds ();
		    FdoSmPhFieldsP	fields = binds->GetFields();

            FdoSmPhFieldP(fields->GetItem (L"character_set_name"))->SetFieldValue (characterSetName);
        }

        pReader->Execute();
    }

    return reader;
}

FdoSmPhRowP FdoSmPhRdMySqlCharacterSetReader::MakeBinds (FdoSmPhMgrP mgr, FdoStringP characterSetName)
{
    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"Binds" );
    FdoSmPhDbObjectP rowObj = row->GetDbObject();

    if ( characterSetName != L"" ) {
        FdoSmPhFieldP field = new FdoSmPhField(
            row,
            L"character_set_name",
            rowObj->CreateColumnDbObject(L"character_set_name",false)
        );

        field->SetFieldValue(characterSetName);
    }

    return( row );
}
