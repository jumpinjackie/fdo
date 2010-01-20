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
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdPostGisConstraintReader::FdoSmPhRdPostGisConstraintReader(
    FdoSmPhOwnerP owner,
    FdoStringP constraintName)
    : FdoSmPhRdConstraintReader(MakeReader(owner, constraintName)),
      mConstraintName(constraintName),
      mOwner(owner)
{
    // idle
}

FdoSmPhRdPostGisConstraintReader::FdoSmPhRdPostGisConstraintReader(
    FdoSmPhOwnerP owner,
	FdoStringP tableName,
    FdoStringP constraintType)
    : FdoSmPhRdConstraintReader(
        MakeReader(owner, tableName, NULL, constraintType)),
      mConstraintName(constraintType),
	  mTableName(tableName),
      mOwner(owner)
{
    // idle
}

FdoSmPhRdPostGisConstraintReader::FdoSmPhRdPostGisConstraintReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join,
    FdoStringP constraintType)
    : FdoSmPhRdConstraintReader(MakeReader(owner, L"", join, constraintType)),
      mConstraintName(constraintType.Upper()),
      mOwner(owner)
{
    // idle
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
        // postgres changes our check from "prop" IN (...) to "prop" = ANY (ARRAY[...]). The caller expect the IN clause.
        FdoStringP defValue = FdoSmPhRdConstraintReader::GetString( tableName, fieldName ); 
        if( defValue != NULL && defValue.Contains(L"= ANY (ARRAY[") )
        {
            fieldValue = defValue.Replace(L"= ANY (ARRAY[",L"IN (");
            fieldValue = fieldValue.Replace(L"]",L")");
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
	FdoStringP	tableName,
    FdoSmPhRdTableJoinP join,
    FdoStringP constraintType)
{
    FdoSmPhPostGisMgrP   pgMgr = owner->GetManager()->SmartCast<FdoSmPhPostGisMgr>();
    FdoSmPhPostGisOwnerP pgOwner(owner->SmartCast<FdoSmPhPostGisOwner>());

    // PostgreSQL does support CHECK constraints:
    // http://www.postgresql.org/docs/8.2/interactive/infoschema-table-constraints.html

    FdoStringP ownerName(owner->GetName());
 
    // If joining to another table, generated from sub-clause for table.
    FdoStringP joinFrom;
    if ((NULL != join) && (L"" == tableName))
    {
        joinFrom = FdoStringP::Format(L"  , %ls\n",
            static_cast<FdoString*>(join->GetFrom()));
    }

    FdoStringP qualification;

    if (tableName != L"")
    {
        // Selecting single object, qualify by this object.
        qualification = L"  and tc.table_name = $1\n";
    } 
    else
    {
        if (NULL != join)
        {
            // Otherwise, if joining to another table, generated join clause.
            qualification = FdoStringP::Format(L"  and (%ls)\n",
                static_cast<FdoString*>(join->GetWhere(L"tc.table_name")));
        }
    }

    // Create a field object for each field in the select list.
    FdoSmPhRowsP rows(new FdoSmPhRowCollection());

    // Single row, no joins
    FdoSmPhRowP row(new FdoSmPhRow(owner->GetManager(),
                        L"ConstraintColumns", NULL));
    rows->Add(row);
    
    // Each field adds itself to the row.
    FdoSmPhFieldP field(new FdoSmPhField(row, L"constraint_name",
                            row->CreateColumnDbObject(L"constraint_name",
                            false)));

    field = new FdoSmPhField(row, L"table_name",
                row->CreateColumnDbObject(L"table_name", false));

    field = new FdoSmPhField(row, L"column_name",
                row->CreateColumnDbObject(L"column_name", false));

    field = new FdoSmPhField(
        row, 
        L"table_schema",
        row->CreateColumnDbObject(L"table_schema",false)
    );

    FdoStringP sqlString;

    if ( constraintType == L"C" ) 
    {
        sqlString = FdoStringP::Format(
            L" SELECT %ls tc.constraint_name AS constraint_name,"
            L" tc.table_schema ||'.'|| tc.table_name AS table_name,"
            L" tc.column_name AS column_name, "
            L" tc.table_schema AS table_schema,"
            L" kcu.check_clause as check_clause, "
            L" %ls as collate_schema_name, "
            L" %ls as collate_table_name, "
            L" %ls as collate_constraint_name "
            L" FROM %ls AS tc, %ls AS kcu %ls"
            L" WHERE (tc.table_schema = kcu.constraint_schema"
            L" %ls\n"
            L" AND tc.constraint_name = kcu.constraint_name)"
            L" ORDER BY collate_schema_name, collate_table_name, collate_constraint_name",
            (join ? L"distinct" : L""),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"tc.table_schema"),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"tc.table_name"),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"tc.constraint_name"),
            L"information_schema.constraint_column_usage",
            L"information_schema.check_constraints",
            static_cast<FdoString*>(joinFrom),
            static_cast<FdoString*>(qualification));

        field = new FdoSmPhField(
			    row, 
                L"check_clause",
			    row->CreateColumnDbObject(L"check_clause",false)
		    );
    }
    else if ( constraintType == L"U" )
	{
        sqlString = FdoStringP::Format(
            L" SELECT %ls tc.constraint_name AS constraint_name,"
            L" tc.table_schema ||'.'|| tc.table_name AS table_name,"
            L" tc.column_name AS column_name, "
            L" tc.table_schema AS table_schema,"
            L" %ls as collate_schema_name, "
            L" %ls as collate_table_name, "
            L" %ls as collate_constraint_name "
            L" FROM %ls AS tc, %ls AS kcu %ls"
            L" WHERE (tc.table_schema = kcu.constraint_schema"
            L" AND kcu.constraint_type ='UNIQUE'"
            L" %ls\n"
            L" AND tc.constraint_name = kcu.constraint_name)"
            L" ORDER BY collate_schema_name, collate_table_name, collate_constraint_name",
            (join ? L"distinct" : L""),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"tc.table_schema"),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"tc.table_name"),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"tc.constraint_name"),
            L"information_schema.constraint_column_usage",
            L"information_schema.table_constraints",
            static_cast<FdoString*>(joinFrom),
            static_cast<FdoString*>(qualification));
    }
      
    // Create the bind variables
    FdoSmPhRowP binds(new FdoSmPhRow(owner->GetManager(), L"Binds")); 
    if (tableName != L"")
    {
        field = new FdoSmPhField(binds, "table_name",
                    binds->CreateColumnDbObject(L"table_name", false));

        field->SetFieldValue(tableName);
    }

    // TODO: cache this query to make full use of the binds.

    FdoSmPhRdGrdQueryReader* reader = NULL;
    reader = new FdoSmPhRdGrdQueryReader(FdoSmPhRowP(rows->GetItem(0)),
                sqlString, owner->GetManager(), binds );

    return reader;
}
