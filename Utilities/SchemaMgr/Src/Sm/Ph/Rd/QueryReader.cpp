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
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Error.h>

FdoSmPhRdQueryReader::FdoSmPhRdQueryReader(
    FdoSmPhRowP fields, 
    FdoStringP sStatement, 
    FdoSmPhMgrP mgr,
    FdoSmPhRowP binds
) :
	FdoSmPhReader(mgr, MakeRows(fields,mgr)),
    mStatement(sStatement),
    mBindFields(binds)
{
}

FdoSmPhRdQueryReader::FdoSmPhRdQueryReader(
    FdoSmPhRowsP rows, 
    FdoStringP sClauses, 
    FdoSmPhMgrP mgr,
    FdoSmPhRowP binds
) :
	FdoSmPhReader(mgr, rows),
	mStatement(MakeStatement(rows, sClauses)),
    mBindFields(binds)
{
}


FdoSmPhRdQueryReader::~FdoSmPhRdQueryReader(void)
{
}


FdoStringP FdoSmPhRdQueryReader::MakeStatement( FdoSmPhRowsP rows, FdoStringP sClauses )
{
    FdoInt32 tidx;
    FdoInt32 cidx;
    FdoStringsP cols = FdoStringCollection::Create();
    FdoStringsP tables = FdoStringCollection::Create();
    FdoStringP statement;

    // Process each row
    for ( tidx = 0; tidx < rows->GetCount(); tidx++ ) {
        FdoSmPhRowP row = rows->GetItem(tidx);

        // Skip statement generation if any of the row database objects do not exist.
        // In this case the query will return no rows.
        if ( !row->GetDbObject()->GetExists() ) {
            tables = FdoStringCollection::Create();
            break;
        }

        // Add the table to the "from" list + the table name as a alias
		tables->Add( row->GetDbObject()->GetDbQName() + L" " + row->GetName() );

        FdoSmPhFieldsP pFields = row->GetFields();

        // Process each table's field
        for ( cidx = 0; cidx < pFields->GetCount(); cidx++ ) {
            FdoSmPhFieldP pField = pFields->GetItem(cidx);

            FdoStringP selectCol = pField->GetSelect();

            if ( selectCol.GetLength() > 0 ) {
                // Add the field to the select list.
                cols->Add( selectCol );
            }
            else {
		        throw FdoSchemaException::Create (
                    FdoSmError::NLSGetMessage(
                        FDO_NLSID(FDOSM_308), 
    			        (FdoString*) pField->GetQName() 
                    )
		        );
            }
        }
    }

    // Create SQL style query statement.
    // TODO: think about moving this up to generic RDBMS level.
    if ( tables->GetCount() > 0 ) {
        statement = FdoStringP::Format( L"select %ls from %ls %ls",
                        (FdoString*) cols->ToString( L", "),
                        (FdoString*) tables->ToString( L", "),
                        (FdoString*) sClauses
                    );
    }
    else {
        statement = L"";
    }

    return( statement );
}

FdoSmPhRowsP FdoSmPhRdQueryReader::MakeRows(FdoSmPhRowP row, FdoSmPhMgrP mgr)
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();
    rows->Add(row);
    
    return( rows );
}
