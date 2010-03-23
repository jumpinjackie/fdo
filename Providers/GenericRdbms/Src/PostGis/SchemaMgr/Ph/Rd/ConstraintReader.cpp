//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include <Parse/Parse.h>
#include "ConstraintReader.h"
#include "../Mgr.h"
#include "../Owner.h"
#include <Sm/Ph/Rd/SchemaDbObjectBinds.h>
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdPostGisConstraintReader::FdoSmPhRdPostGisConstraintReader(
    FdoSmPhOwnerP owner,
    FdoStringP constraintName)
    : FdoSmPhRdConstraintReader(),
      mConstraintName(constraintName),
      mOwner(owner)
{
    SetSubReader(MakeReader(owner, constraintName));
}

FdoSmPhRdPostGisConstraintReader::FdoSmPhRdPostGisConstraintReader(
    FdoSmPhOwnerP owner,
	FdoStringP tableName,
    FdoStringP constraintType)
    : FdoSmPhRdConstraintReader(),
      mConstraintName(constraintType),
	  mTableName(tableName),
      mOwner(owner)
{
    SetSubReader(MakeReader(owner, DbObjectName2Objects(tableName), NULL, constraintType));
}

FdoSmPhRdPostGisConstraintReader::FdoSmPhRdPostGisConstraintReader(
    FdoSmPhOwnerP owner,
	FdoStringsP tableNames,
    FdoStringP constraintType)
    : FdoSmPhRdConstraintReader(),
      mConstraintName(constraintType),
      mOwner(owner)
{
    SetSubReader(MakeReader(owner, tableNames, NULL, constraintType));
}

FdoSmPhRdPostGisConstraintReader::FdoSmPhRdPostGisConstraintReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join,
    FdoStringP constraintType)
    : FdoSmPhRdConstraintReader(),
      mConstraintName(constraintType.Upper()),
      mOwner(owner)
{
    SetSubReader(MakeReader(owner, DbObjectName2Objects(L""), join, constraintType));
}

FdoSmPhRdPostGisConstraintReader::~FdoSmPhRdPostGisConstraintReader()
{
    // idle
}

FdoSmPhReaderP FdoSmPhRdPostGisConstraintReader::MakeReader(
    FdoSmPhOwnerP owner,
    FdoStringP constraintName)
{
    FdoStringP ownerName(owner->GetName());

    // TODO
    return (FdoSmPhReader*) NULL;


    // template_postgis=# \d information_schema.table_constraints
    //     View "information_schema.table_constraints"
    //        Column       |       Type        |
    // --------------------+-------------------+
    //  constraint_catalog | character varying |
    //  constraint_schema  | character varying |
    //  constraint_name    | character varying |
    //  table_catalog      | character varying |
    //  table_schema       | character varying |
    //  table_name         | character varying |
    //  constraint_type    | character varying | <- Type: CHECK,FOREIGN KEY,
    //  is_deferrable      | character varying |          PRIMARY KEY,UNIQUE
    //  initially_deferred | character varying |
    //
    //
    // template_postgis=# \d information_schema.key_column_usage
    //    View "information_schema.key_column_usage"
    //             Column             |     Type        |
    // -------------------------------+-----------------+
    //  constraint_catalog            | sql_identifier  |
    //  constraint_schema             | sql_identifier  |
    //  constraint_name               | sql_identifier  |
    //  table_catalog                 | sql_identifier  |
    //  table_schema                  | sql_identifier  |
    //  table_name                    | sql_identifier  |
    //  column_name                   | sql_identifier  |
    //  ordinal_position              | cardinal_number | <- Starts at 1


    // Names of PostgreSQL objects: database, table, field and columns
    // are case-independent, unless you created them with double-quotes
    // around their name, in which case they are case-sensitive.
    // NOTE: In MySQL, table names can be case-sensitive or not,
    //       depending on which operating system you are using.
    //       On Linux, names are case-sensitive.

    // Example of binds:
    // $1 - 'public'
    // $2 - 'geometry_columns_pk'

    FdoStringP sqlString = 
        L"SELECT constraint_name, table_name, column_name "
        L"FROM information_schema.key_column_usage AS kcu "
        //L"WHERE kcu.table_schema = $1"
        L"WHERE kcu.constraint_name = $1 "
        L"ORDER BY kcu.table_schema, kcu.table_name, kcu.ordinal_position";

    // Create a field object for each field in the select list
    FdoSmPhRowsP rows(MakeRows(owner->GetManager()));

    // Create the bind variables
    FdoSmPhRowP binds(new FdoSmPhRow(owner->GetManager(), L"Binds"));

   // FdoSmPhFieldP field(new FdoSmPhField(binds, L"table_schema",
   //                         binds->CreateColumnDbObject(L"table_schema",
   //                             false)));

   // field->SetFieldValue(ownerName);

    FdoSmPhFieldP field = new FdoSmPhField(binds, L"constraint_name",
                binds->CreateColumnDbObject(L"constraint_name", false));

    field->SetFieldValue(constraintName);

    //TODO: Cache this query to make full use of the binds.
    FdoSmPhRdGrdQueryReader* reader = NULL;
    reader = new FdoSmPhRdGrdQueryReader(FdoSmPhRowP(rows->GetItem(0)),
                sqlString, owner->GetManager(), binds);

    return reader;
}

FdoStringP FdoSmPhRdPostGisConstraintReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    FdoStringP fieldValue;

    if ( fieldName == L"check_clause" )  {
        // postgres adds the data type to each element in the constraint. The constraint parser does not expect the type much less the
        // postgres types. We have to strip off any type qualifier from the list such as: ("PropList"::text = ANY (ARRAY['Str10'::character varying, 'Str50'::character varying]::text[]))
        FdoStringP defValue = FdoSmPhRdConstraintReader::GetString( tableName, fieldName ); 
        if( defValue != NULL  )
        {
            fieldValue = defValue.Replace(L"= ANY ((ARRAY[",L"IN (");
            fieldValue = fieldValue.Replace(L"= ANY (ARRAY[",L"IN (");
            fieldValue = fieldValue.Replace(L"::text[]",L"");
            fieldValue = fieldValue.Replace(L"::text",L"");
            fieldValue = fieldValue.Replace(L"::bigint",L"");
            fieldValue = fieldValue.Replace(L"::numeric",L"");
            fieldValue = fieldValue.Replace(L"::real",L"");
            fieldValue = fieldValue.Replace(L"::date",L"");
            fieldValue = fieldValue.Replace(L"::timestamp without time zone",L"");
            fieldValue = fieldValue.Replace(L"]",L")"); 
            fieldValue = fieldValue.Replace(L"double precision",L"");
            fieldValue = fieldValue.Replace(L"character varying",L"");           
            fieldValue = fieldValue.Replace(L"::",L" ");
        }
        else
            fieldValue = FdoSmPhRdConstraintReader::GetString( tableName, fieldName );

        FdoCommonParse      			   *parser  = new FdoCommonParse();;
        FdoPtr<FdoPropertyValueConstraint> pConstr;
        try 
        {
			if (parser)
				pConstr = parser->ParseConstraint( fieldValue );
        }
        catch ( FdoExpressionException* ex ) {
            // Silently skip the unsupported constraints.
            ex->Release();
            pConstr = NULL;
            fieldValue = "";
        }
        if (parser)
            delete parser;
    }
    else {
        fieldValue = FdoSmPhRdConstraintReader::GetString( tableName, fieldName );        
    }

    return fieldValue;
}

FdoSmPhReaderP FdoSmPhRdPostGisConstraintReader::MakeReader(
    FdoSmPhOwnerP owner,
    FdoStringsP tableNames,
    FdoSmPhRdTableJoinP join,
    FdoStringP constraintType)
{
    FdoSmPhPostGisMgrP   pgMgr = owner->GetManager()->SmartCast<FdoSmPhPostGisMgr>();
    FdoSmPhPostGisOwnerP pgOwner(owner->SmartCast<FdoSmPhPostGisOwner>());

    // PostgreSQL does support CHECK constraints:
    // http://www.postgresql.org/docs/8.2/interactive/infoschema-table-constraints.html

    FdoStringP sqlString;

    if ( constraintType == L"C" ) 
    {
        sqlString = FdoStringP::Format(
            L" SELECT %ls tc.conname AS constraint_name,"
            L" ns.nspname ||'.'|| c.relname AS table_name,"
            L" tc.conkey[1] AS column_name, "
            L" ns.nspname AS table_schema,"
            L" substring(pg_get_constraintdef(tc.oid),7) as check_clause, "
            L" %ls as collate_schema_name, "
            L" %ls as collate_table_name, "
            L" %ls as collate_constraint_name "
            L" FROM pg_constraint tc, pg_class c, pg_namespace ns $(JOIN_FROM) "
            L" WHERE tc.contype = 'c' "
            L" and array_upper(tc.conkey,1) = 1 "
            L" and c.oid = tc.conrelid and ns.oid = tc.connamespace "
            L" $(AND) $(QUALIFICATION)\n"
            L" ORDER BY collate_schema_name, collate_table_name, collate_constraint_name",
            (join ? L"distinct" : L""),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"ns.nspname"),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"c.relname"),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"tc.conname")
        );
    }
    else if ( constraintType == L"U" )
	{
        sqlString = FdoStringP::Format(
            L" SELECT %ls tc.conname AS constraint_name,"
            L" ns.nspname ||'.'|| c.relname AS table_name,"
            L" cast(tc.conkey as text) AS column_name, "
            L" ns.nspname AS table_schema,"
            L" %ls as collate_schema_name, "
            L" %ls as collate_table_name, "
            L" %ls as collate_constraint_name "
            L" FROM pg_constraint tc, pg_class c, pg_namespace ns $(JOIN_FROM) "
            L" WHERE tc.contype = 'u' "
            L" and c.oid = tc.conrelid and ns.oid = tc.connamespace "
            L" $(AND) $(QUALIFICATION)\n"
            L" ORDER BY collate_schema_name, collate_table_name, collate_constraint_name",
            (join ? L"distinct" : L""),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"ns.nspname"),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"c.relname"),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"tc.conname")
        );
    }
      
    FdoSmPhReaderP reader = FdoSmPhRdConstraintReader::MakeQueryReader(
        L"",
        owner,
        sqlString,
        L"ns.nspname",
        L"c.relname",
        tableNames,
        join
    );

    return reader;
}

FdoSmPhRowsP FdoSmPhRdPostGisConstraintReader::MakeRows( FdoSmPhMgrP mgr )
{
    // Call superclass to populate generic row/columns:
    FdoSmPhRowsP rows = FdoSmPhRdConstraintReader::MakeRows(mgr);
    FdoSmPhRowP row = rows->GetItem(0);

    FdoSmPhFieldP field = new FdoSmPhField(
        row, 
        L"table_schema",
        row->CreateColumnDbObject(L"table_schema",false)
    );

    field = new FdoSmPhField(
	    row, 
        L"check_clause",
	    row->CreateColumnDbObject(L"check_clause",false)
    );

    return( rows);
}

