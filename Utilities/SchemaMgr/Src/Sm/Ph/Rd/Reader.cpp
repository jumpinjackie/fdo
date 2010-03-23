/*
 * Copyright (C) 2010  Autodesk, Inc.
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
#include <Sm/Ph/Rd/Reader.h>
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Ph/Rd/SchemaDbObjectBinds.h>
#include <Sm/Ph/DbObject.h>

FdoSmPhRdReader::FdoSmPhRdReader(
    FdoSmPhReaderP reader
) :
	FdoSmPhReader(reader)
{
}


FdoSmPhRdReader::FdoSmPhRdReader(
    FdoSmPhMgrP         mgr,
    FdoSmPhRowsP        rows
) :
	FdoSmPhReader(mgr, rows)
{
}


FdoSmPhRdReader::~FdoSmPhRdReader(void)
{
}

FdoSmPhReaderP FdoSmPhRdReader::MakeQueryReader(
    FdoStringP readerName,
    FdoSmPhOwnerP owner,
    FdoStringP sqlString,
    FdoStringP schemaColumn,
    FdoStringP dbObjectColumn,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join
)
{
    FdoStringP           finalSqlString;
    FdoSmPhMgrP          mgr = owner->GetManager();

    FdoSmPhReaderP reader;

    // If static cursors enabled, resuse existing query reader if availabe.
    if ( readerName.GetLength() > 0 ) 
        reader = mgr->GetStaticReader ( readerName );

    if ( !reader ) {
        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);

        // Generate join and qualification clauses

        // Create binds for object names
        FdoSmPhRdSchemaDbObjectBindsP binds = new FdoSmPhRdSchemaDbObjectBinds(
            mgr,
            schemaColumn,
            L"schema_name",
            dbObjectColumn,
            L"table_name",
            objectNames
        );

        // Determine syntax caller wants for the join:
        //      useClause=true - use a JOIN clause.
        //      useClause=false - add join table to from clause and join to where clause.
        bool useClause = sqlString.Contains( L"$(JOIN_CLAUSE)" );

        // If joining to another table, generated from sub-clause for table.
        FdoStringP joinFrom;
        FdoStringP joinClause;
        if ( join != NULL ) {
            if ( useClause ) 
                joinClause = join->GetClause( mgr, schemaColumn,dbObjectColumn );
            else
                joinFrom = FdoStringP::Format( L"  , %ls\n", (FdoString*) join->GetFrom() );
        }

        // Get where clause for owner and object name binds.
        FdoStringP qualification = binds->GetSQL();

        if ( (!useClause) && (join != NULL) ) {
            // If joining to another table, but not generating join clause, add join to qualification.
            qualification += FdoStringP::Format( 
                L"  %ls (%ls)\n", 
                (qualification == L"") ? L"" : L"and",
                (FdoString*) join->GetWhere(mgr, schemaColumn, dbObjectColumn) 
            );
        }

        // Substitute join and qualification clauses into given SQL
        finalSqlString = sqlString.Replace( L"$(JOIN_CLAUSE)", joinClause );
        finalSqlString = finalSqlString.Replace( L"$(JOIN_FROM)", joinFrom );
        finalSqlString = finalSqlString.Replace( L"$(QUALIFICATION)", qualification );

        // Handle qualification clause connectors.
        if ( qualification != L"" ) {
            finalSqlString = finalSqlString.Replace( L"$(WHERE)", L"where" );
            finalSqlString = finalSqlString.Replace( L"$(AND)", L"and" );
        }
        else {
            finalSqlString = finalSqlString.Replace( L"$(WHERE)", L"" );
            finalSqlString = finalSqlString.Replace( L"$(AND)", L"" );
        }

        reader = mgr->CreateQueryReader(row, finalSqlString, binds->GetBinds() );

        if (reader && (!join) && (readerName.GetLength() > 0) )
            // Save away as static reader.
			mgr->SetStaticReader ( readerName, reader );
    }
    else {
        // Re-execute this existing reader.

        FdoSmPhRdQueryReaderP queryReader = reader->SmartCast<FdoSmPhRdQueryReader>();
            
        // queryReader is NULL when it is an empty reader. No need to re-execute in this case.
        if ( queryReader ) {
            FdoSmPhRowP bindRow;
            
            // Update bind values
            bindRow = queryReader->GetBinds();

            FdoSmPhRdSchemaDbObjectBindsP binds = new FdoSmPhRdSchemaDbObjectBinds(
                mgr,
                schemaColumn,
                L"schema_name",
                dbObjectColumn,
                L"table_name",
                objectNames,
                bindRow,
                true
            );

            // re-execute
            queryReader->Execute();
        }
    }

    return reader;
}

FdoSmPhRowsP FdoSmPhRdReader::MakeRows( FdoSmPhMgrP mgr )
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    return rows;
}

FdoStringsP FdoSmPhRdReader::DbObject2Objects( FdoSmPhDbObjectP dbObject )
{
    FdoStringsP tableNames = FdoStringCollection::Create();
    if ( dbObject ) 
        tableNames->Add( dbObject->GetName() );

    return tableNames;
}

FdoStringsP FdoSmPhRdReader::DbObjectName2Objects( FdoStringP dbObjectName )
{
    FdoStringsP dbObjectNames = FdoStringCollection::Create();
    if ( dbObjectName != L"" ) 
        dbObjectNames->Add( dbObjectName );

    return dbObjectNames;
}
