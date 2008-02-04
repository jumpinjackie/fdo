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
#include <Sm/Ph/Rd/OwnerReader.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhRdOwnerReader::FdoSmPhRdOwnerReader(
    FdoSmPhReaderP reader,
    FdoSmPhDatabaseP database,
    FdoStringP ownerName
) :
	FdoSmPhReader(reader),
    mDatabase(database),
    mOwnerName(ownerName),
    mAllOwnersWithMetaSchemaLoaded(false)
{
}

FdoSmPhRdOwnerReader::FdoSmPhRdOwnerReader(
    FdoSmPhMgrP         mgr,
    FdoSmPhRowsP        rows,
    FdoSmPhDatabaseP    database,
    FdoStringP          ownerName
) :
	FdoSmPhReader(mgr, rows),
    mDatabase(database),
    mOwnerName(ownerName)
{
}

FdoSmPhRdOwnerReader::~FdoSmPhRdOwnerReader(void)
{
}

FdoBoolean FdoSmPhRdOwnerReader::GetHasMetaSchema()
{
    FdoDictionaryElementP elem;
    FdoSmPhReaderP rdr;

    // Create list of FDO-enabled datastores if not already created.
    // This list is used for performance.
    if ( !mOwnersWithMetaSchema ) {
        mOwnersWithMetaSchema = FdoDictionary::Create();

        // Try to load up all the FDO-enabled datastores, returned by this reader,
        // in one select.

        rdr = MakeHasMetaSchemaReader(
            GetDatabase(),
            GetOwnerName()
        );

        // rdr will be NULL if provider does not support bulk-loading FDO-enabled datastores.
        if ( rdr ) {
            while ( rdr->ReadNext() ) {
                elem = FdoDictionaryElement::Create(
                    rdr->GetString( L"", L"name" ),
                    L"yes"                          // "yes" marks it as FDO-enabled.
                );

                mOwnersWithMetaSchema->Add( elem );
            }

            mAllOwnersWithMetaSchemaLoaded = true;
        }
    }

    // Find current datastore in FDO-enabled datastore list.
    elem = mOwnersWithMetaSchema->FindItem( GetString(L"", L"name") );

    if ( (!elem) && (!mAllOwnersWithMetaSchemaLoaded) ) {
        // Datastore not found, and FDO-enabled list was not bulk loaded.
        // Retrieved FDO-enabled status for datastore from RDBMS.
        rdr = MakeHasMetaSchemaReader(
            GetDatabase(),
            GetString(L"", L"name")
        );

        if ( rdr && rdr->ReadNext() ) {
            // Found it, add it to the FDO-enabled datastore list.
            elem = FdoDictionaryElement::Create(
                rdr->GetString( L"", L"name" ),
                L"yes"
            );

            mOwnersWithMetaSchema->Add( elem );
        }
    }

    if ( !elem ) {
        // Datastore is still not found. It must be a non-FDO datastore.
        // Cache it as such.
        elem = FdoDictionaryElement::Create(
            GetString( L"", L"name" ),
            L"no"
        );

        mOwnersWithMetaSchema->Add( elem );
    }

    return ( wcscmp(elem->GetValue(), L"yes") == 0 );
}

FdoSmPhRowsP FdoSmPhRdOwnerReader::MakeRows( FdoSmPhMgrP mgr)
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Single row, no joins
    FdoSmPhRowP row = new FdoSmPhRow(mgr, L"OwnerFields"); 
    rows->Add(row);

    // Each field adds itself to the row.
    FdoSmPhFieldP pField = new FdoSmPhField(
        row, 
        L"name",
        row->CreateColumnDbObject(L"name",false)
    );

    return( rows);
}

FdoSmPhRowsP FdoSmPhRdOwnerReader::MakeHasMetaSchemaRows( FdoSmPhMgrP mgr)
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Single row, no joins
    FdoSmPhRowP row = new FdoSmPhRow(mgr, L"OwnerHasMetaSchema"); 
    rows->Add(row);

    // Each field adds itself to the row.
    FdoSmPhFieldP pField = new FdoSmPhField(
        row, 
        L"name",
        row->CreateColumnDbObject(L"name",false)
    );

    return( rows);
}



