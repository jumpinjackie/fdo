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
#include "QueryReader.h"
#include "../Mgr.h"

#define FDO_FETCH_SIZE 50


FdoSmPhRdGrdQueryReader::FdoSmPhRdGrdQueryReader(
    FdoSmPhRowP fields,
    FdoStringP sStatement,
    FdoSmPhMgrP mgr,
    FdoSmPhRowP binds
) :
    FdoSmPhRdQueryReader(fields, sStatement, mgr, binds),
    mQuery(NULL),
    mResults(NULL)
{
    Execute();
}

FdoSmPhRdGrdQueryReader::FdoSmPhRdGrdQueryReader(
    FdoSmPhRowsP rows,
    FdoStringP sClauses,
    FdoSmPhMgrP mgr,
    FdoSmPhRowP binds
) :
    FdoSmPhRdQueryReader(rows, sClauses, mgr, binds),
    mQuery(NULL),
    mResults(NULL)
{
    Execute();
}


FdoSmPhRdGrdQueryReader::~FdoSmPhRdGrdQueryReader(void)
{
    if ( mResults )
        delete mResults;

    if ( mQuery )
        delete mQuery;
}

bool FdoSmPhRdGrdQueryReader::ReadNext()
{
    // Nothing to do if no query statement.
    if ( mStatement.GetLength() == 0 )
        SetEOF(true);

    // Nothing to do if no more rows.
    if ( this->IsEOF() )
        return false;

    if ( !mResults->ReadNext() ) {
        SetEOF(true);
        return false;
    }

    SetBOF(false);
    return(!IsEOF());
}

FdoStringP FdoSmPhRdGrdQueryReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    // Make sure there is a current row.
    CheckGet();

    FdoSmPhRdGrdFieldArrayP fetch = GetFieldArray(tableName, fieldName);
    return fetch->GetString();
}

int FdoSmPhRdGrdQueryReader::GetInteger( FdoStringP tableName, FdoStringP fieldName )
{
    // Make sure there is a current row.
    CheckGet();

    FdoSmPhRdGrdFieldArrayP fetch = GetFieldArray(tableName, fieldName);
    return fetch->GetInteger();
}

long FdoSmPhRdGrdQueryReader::GetLong( FdoStringP tableName, FdoStringP fieldName )
{
    // Make sure there is a current row.
    CheckGet();

    FdoSmPhRdGrdFieldArrayP fetch = GetFieldArray(tableName, fieldName);
    return fetch->GetLong();
}

double FdoSmPhRdGrdQueryReader::GetDouble( FdoStringP tableName, FdoStringP fieldName )
{
    // Make sure there is a current row.
    CheckGet();

    FdoSmPhRdGrdFieldArrayP fetch = GetFieldArray(tableName, fieldName);
    return fetch->GetDouble();
}

bool FdoSmPhRdGrdQueryReader::GetBoolean( FdoStringP tableName, FdoStringP fieldName )
{
    // Make sure there is a current row.
    CheckGet();

    FdoSmPhRdGrdFieldArrayP fetch = GetFieldArray(tableName, fieldName);
    return fetch->GetBoolean();
}

void FdoSmPhRdGrdQueryReader::Execute()
{
    int i,j;
    int col = 0;

    SetBOF(true);
    SetEOF(false);

    GdbiConnection* connection = ((FdoSmPhGrdMgr*)(FdoSmPhMgr*)GetManager())->GetGdbiConnection();
    GdbiCommands* commands = connection->GetCommands();

    // Execute only if select statement was generated. Select statement generation
    // is skipped when the from tables do not exist.
    if ( mStatement.GetLength() > 0 ) {
        if ( mQuery == NULL ) {
            // Create a collection to hold field values for all rows.
            mFetches = new FdoSmPhRdGrdRowArrayCollection();

            mQuery = connection->Prepare( (wchar_t*)(FdoString*) mStatement );

            // Bind each bind field to the query.
            if ( mBindFields ) {
                for ( j = 0; j < mBindFields->GetFields()->GetCount(); j++ ) {
                    FdoSmPhFieldP pField = mBindFields->GetFields()->GetItem(j);
                    col++;
                    if ( GetManager()->IsRdbUnicode() ) 
                        mQuery->Bind( col, pField->GetBindSize(), (FdoString*) pField->GetBindString(), pField->GetNullInd() );
                    else
                        mQuery->Bind( col, pField->GetBindSize(), (char*) pField->GetBindString(), pField->GetNullInd() );
                    GDBI_NI_TYPE  *null_ind = pField->GetNullInd();
                    if ( *null_ind )
                        commands->set_null( null_ind, 0, 0);
                    else
                        commands->set_nnull( null_ind, 0, 0);
                }
            }

            // get the query results set
            mResults = mQuery->ExecuteQuery();

            col = 0;
            FdoSmPhRowsP rows = GetRows();

            for ( i = 0; i < rows->GetCount(); i++ ) {
                // Create a row array for each row
                FdoSmPhRowP row = rows->GetItem(i);

                FdoSmPhRdGrdRowArrayP rowArray = new FdoSmPhRdGrdRowArray(row);
                mFetches->Add(rowArray);

                for ( j = 0; j < row->GetFields()->GetCount(); j++ ) {
                    // Create a field array for each field.
                    // field array holds the field value when the next row is retreived
                    FdoSmPhFieldP field = row->GetFields()->GetItem(j);
                    col++;
                    FdoSmPhRdGrdFieldArrayP fieldArray = new FdoSmPhRdGrdFieldArray( field, mResults, col );
                    rowArray->GetFields()->Add(fieldArray);
                }
            }
        }
        else {
            if ( mResults )
                delete mResults;

            mResults = mQuery->ExecuteQuery();

			FdoSmPhRdGrdRowArrayP rowArray;
			FdoSmPhRdGrdFieldArrayP fieldArray;
			int i, j;
			for ( i = 0; i < mFetches->GetCount(); i++ ) {
				rowArray = mFetches->GetItem(i);
				for ( j = 0; j < rowArray->GetFields()->GetCount(); j++ )	{
					fieldArray = rowArray->GetFields()->GetItem(j);
					fieldArray->UpdateResults(mResults);
				}
			}
		}
    }
}

FdoSmPhRdGrdFieldArrayP FdoSmPhRdGrdQueryReader::GetFieldArray( FdoStringP tableName, FdoStringP fieldName)
{
    FdoSmPhRdGrdRowArrayP rowArray;
    FdoSmPhRdGrdFieldArrayP fieldArray;
    int cidx;

    if ( tableName.GetLength() > 0 ) {
        rowArray = mFetches->GetItem( tableName );
        fieldArray = rowArray ? rowArray->GetFields()->GetItem( fieldName ) : NULL;
    }
    else {
        for ( cidx = 0; cidx < mFetches->GetCount(); cidx++ ) {
            rowArray = mFetches->GetItem(cidx);
            fieldArray = rowArray->GetFields()->FindItem( fieldName );
            if ( fieldArray )
                break;
        }
    }

    if ( fieldArray == NULL )
            throw FdoSchemaException::Create( 
			    NlsMsgGet2(
				    FDORDBMS_125,
                    "Field '%1$ls' is not retrieved by query: '%2$ls'.", 
                    (tableName == L"") ?
                       (FdoString*) fieldName :
                       (FdoString*) (tableName + L"." + fieldName),
                    (FdoString*) mStatement
                )
    		);

    return fieldArray;
}

void FdoSmPhRdGrdQueryReader::EndSelect()
{
	if ( mResults )
		mResults->End();
}

