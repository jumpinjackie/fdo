/*
 * (C) Copyright 2011 by Autodesk, Inc. All Rights Reserved.
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
#include "SynonymReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"
#include "../Database.h"
#include <Sm/Ph/Rd/DbObjectBinds.h>

FdoSmPhRdSqsSynonymReader::FdoSmPhRdSqsSynonymReader(
    FdoSmPhOwnerP owner,
    FdoStringP objectName
) :
    FdoSmPhRdSynonymReader((FdoSmPhReader*) NULL, owner),
    mOwner(owner)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    if ( objectName != L"" ) 
        objectNames->Add(objectName);
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdSqsSynonymReader::FdoSmPhRdSqsSynonymReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
) :
    FdoSmPhRdSynonymReader((FdoSmPhReader*) NULL, owner),
    mOwner(owner)
{
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdSqsSynonymReader::~FdoSmPhRdSqsSynonymReader(void)
{
}

FdoSmPhReaderP FdoSmPhRdSqsSynonymReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
)
{
    FdoSmPhMgrP mgr(owner->GetManager());
    
    // Generate SQL statement for selecting the synonyms and their base objects

    FdoStringP sqlString = FdoStringP::Format(
        L"select a.name collate latin1_general_bin as name, b.name collate latin1_general_bin as table_schema, \n"
        L"  a.base_object_name as base_name \n"
        L"   from %ls.sys.synonyms a \n"
        L"   INNER JOIN %ls.sys.schemas b on (a.schema_id = b.schema_id)\n"
        L" $(WHERE) $(QUALIFICATION)\n"
        L" order by b.name collate latin1_general_bin asc, a.name collate latin1_general_bin asc",
        (FdoString*)(owner->GetDbName()),
        (FdoString*)(owner->GetDbName())
    );

    FdoSmPhReaderP reader = FdoSmPhRdSynonymReader::MakeQueryReader(
        L"",
        owner,
        sqlString,
        L"b.name",
        L"a.name",
        objectNames,
        NULL
    );

    return reader;
}

bool FdoSmPhRdSqsSynonymReader::ReadNext()
{
    mBaseName = L"";
    mBaseOwner = L"";

    if ( !FdoSmPhRdSynonymReader::ReadNext() )
        return false;

    // SQL Server stores name of base object as:
    //
    //    [schema].[table_name] - when base object and synonym in same database
    //    [database].[schema].[table_name] - when base object and synonym in different databases
    //
    // with names delimited by [].
    //
    // Names can potentially contain embedded '[' or ']' characters. '[' is treated as
    // literal if between starting '[' delimiter and its corresponding ending ']'
    // Literal ']' is encoded as ']]'. 

    FdoStringP fullBaseName = FdoSmPhRdSynonymReader::GetString(L"",L"base_name");

    // Parse out database, schema and table name parts from full name.

    int dot1 = -1;
    int dot2 = -1;
    int i = 0;
    bool inBracket = false;
    FdoString* p = NULL;

    for ( p = fullBaseName; (*p) != 0; p++ ) 
    {
        if ( (*p) == '[' )
        {
            // inside bracketed name until hit closing ']'
            inBracket = true;
        }
        else if ( ((*p) == ']') && ((*(p+1)) != ']')  )
        {
            // At ']' but not ']]' so at end of bracketed name.
            inBracket = false;
        }
        else if ( (!inBracket) && (*p) == '.' )
        {
            // Found a dot separator outside bracketed name. Track position of 1st and
            // 2nd dots encountered. There will 2 dots maximum.
            if ( dot1 == -1 )
                dot1 = i;
            else
                dot2 = i;
        }

        i++;
    }

    if ( dot1 != -1 ) 
    {
        // At least 1 dot encountered, we can parse this full name.

        // Extract the first name.
        FdoStringP string1 = fullBaseName.Mid(1, dot1 - 2);

        FdoStringP string2;

        FdoStringP string3;

        if ( dot2 != -1 ) 
        {
            // There were 2 dots. Extract 2nd and 3rd names.
            string2 = fullBaseName.Mid( dot1 + 2, dot2 - dot1 - 3 );
            string3 = fullBaseName.Mid( dot2 + 2, fullBaseName.GetLength() - dot2 - 3 );

            // Set owner and base object name. Decode any embedded ']'.

            // Datastore is 2st string.
            mBaseOwner = string1.Replace(L"]]",L"]");

            // Base name is schema.table or concatenation of 2nd and 3rd names
            mBaseName = string2 + L"." + string3;
            mBaseName = mBaseName.Replace(L"]]",L"]");
        }
        else
        {
            // Only one dot. Extract 2nd name
            string2 = fullBaseName.Mid( dot1 + 2, fullBaseName.GetLength() - dot1 - 3 );

            // Base object and synonym in same datastore
            mBaseOwner = mOwner->GetName();
            mBaseName = string1 + L"." + string2;

            // Base name is schema.table or concatenation of 1st and 2nd names
            mBaseName = mBaseName.Replace(L"]]",L"]");
        }
    }

    return true;
}

FdoStringP FdoSmPhRdSqsSynonymReader::GetString(FdoStringP tableName, FdoStringP fieldName)
{
    if ( wcscmp(fieldName, L"name") == 0 )
        return FdoSmPhRdSynonymReader::GetString(tableName,L"table_schema") + L"." +
               FdoSmPhRdSynonymReader::GetString(tableName,L"name");
    else if ( wcscmp(fieldName, L"base_name") == 0 )
        return mBaseName;
    else if ( wcscmp(fieldName, L"base_owner") == 0 )
        return mBaseOwner;
    else if ( wcscmp(fieldName, L"base_database") == 0 )
        return L"";

    return FdoSmPhRdSynonymReader::GetString(tableName, fieldName);
}

FdoSmPhRowsP FdoSmPhRdSqsSynonymReader::MakeRows( FdoSmPhMgrP mgr)
{
    FdoSmPhRowsP rows = FdoSmPhRdSynonymReader::MakeRows(mgr);

    // Single row, no joins
    FdoSmPhRowP row = rows->GetItem(0); 

    // Each field adds itself to the row.
    FdoSmPhFieldP pField = new FdoSmPhField(
        row, 
        L"table_schema",
        row->CreateColumnDbObject(L"table_schema",false)
    );

    return rows;
}