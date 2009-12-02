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
    if ( this->IsEOF() || mResults == NULL)
        return false;

    // Derived read may have modified some field values after they have been read.
    // Clear these modifications so they don't override the next values that are read.
    Clear();

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

    // Derived reader may have modified some field values after they have been read.
    // Get value directly from modified field, instead of value fetched from RDBMS.
    if ( FieldIsModified(tableName, fieldName) ) 
        return FdoSmPhReader::GetString( tableName, fieldName );

    FdoSmPhRdGrdFieldArrayP fetch = GetFieldArray(tableName, fieldName);
    return fetch->GetString();
}

int FdoSmPhRdGrdQueryReader::GetInteger( FdoStringP tableName, FdoStringP fieldName )
{
    // Make sure there is a current row.
    CheckGet();

    // Derived reader may have modified some field values after they have been read.
    // Get value directly from modified field, instead of value fetched from RDBMS.
    if ( FieldIsModified(tableName, fieldName) ) 
        return FdoSmPhReader::GetInteger( tableName, fieldName );

    FdoSmPhRdGrdFieldArrayP fetch = GetFieldArray(tableName, fieldName);
    return fetch->GetInteger();
}

long FdoSmPhRdGrdQueryReader::GetLong( FdoStringP tableName, FdoStringP fieldName )
{
    // Make sure there is a current row.
    CheckGet();

    // Derived reader may have modified some field values after they have been read.
    // Get value directly from modified field, instead of value fetched from RDBMS.
    if ( FieldIsModified(tableName, fieldName) ) 
        return FdoSmPhReader::GetLong( tableName, fieldName );

    FdoSmPhRdGrdFieldArrayP fetch = GetFieldArray(tableName, fieldName);
    return fetch->GetLong();
}

double FdoSmPhRdGrdQueryReader::GetDouble( FdoStringP tableName, FdoStringP fieldName )
{
    // Make sure there is a current row.
    CheckGet();

    // Derived reader may have modified some field values after they have been read.
    // Get value directly from modified field, instead of value fetched from RDBMS.
    if ( FieldIsModified(tableName, fieldName) ) 
        return FdoSmPhReader::GetDouble( tableName, fieldName );

    FdoSmPhRdGrdFieldArrayP fetch = GetFieldArray(tableName, fieldName);
    return fetch->GetDouble();
}

bool FdoSmPhRdGrdQueryReader::GetBoolean( FdoStringP tableName, FdoStringP fieldName )
{
    // Make sure there is a current row.
    CheckGet();

    // Derived reader may have modified some field values after they have been read.
    // Get value directly from modified field, instead of value fetched from RDBMS.
    if ( FieldIsModified(tableName, fieldName) ) 
        return FdoSmPhReader::GetBoolean( tableName, fieldName );

    FdoSmPhRdGrdFieldArrayP fetch = GetFieldArray(tableName, fieldName);
    return fetch->GetBoolean();
}

void FdoSmPhRdGrdQueryReader::Execute()
{
    int i,j;
    int col = 0;
    if ( mResults )
        delete mResults;
    mResults = NULL;

    SetBOF(true);
    SetEOF(false);

    GdbiConnection* connection = ((FdoSmPhGrdMgr*)(FdoSmPhMgr*)GetManager())->GetGdbiConnection();
    GdbiCommands* commands = connection->GetCommands();

    // Execute only if select statement was generated. Select statement generation
    // is skipped when the from tables do not exist.
    if ( ((const wchar_t*)mStatement)[0] != '\0' )  {
        if ( mQuery == NULL ) {
            // Create a collection to hold field values for all rows.
            mFetches = new FdoSmPhRdGrdRowArrayCollection();

            mQuery = connection->Prepare( (wchar_t*)(FdoString*) mStatement );

            // Bind each bind field to the query.
            if ( mBindFields ) {
				FdoSmPhFieldsP	pFields = mBindFields->GetFields();

                for ( j = 0; j < pFields->GetCount(); j++ ) {
                    FdoSmPhFieldP pField = pFields->GetItem(j);
                    col++;
                    if ( GetManager()->IsRdbUnicode() ) 
                        mQuery->Bind( col, pField->GetBindSize(), (FdoString*) pField->GetBindString(), pField->GetNullInd()->GetDbIndicator() );
                    else
                        mQuery->Bind( col, pField->GetBindSize(), (char*) pField->GetBindString(), pField->GetNullInd()->GetDbIndicator() );
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
				
				FdoSmPhFieldsP	pFields = row->GetFields();
                for ( j = 0; j < pFields->GetCount(); j++ ) {
                    // Create a field array for each field.
                    // field array holds the field value when the next row is retreived
                    FdoSmPhFieldP field = pFields->GetItem(j);
                    col++;
                    FdoSmPhRdGrdFieldArrayP fieldArray = new FdoSmPhRdGrdFieldArray( field, mResults, col );
					FdoSmPhRdGrdFieldArraysP fields = rowArray->GetFields();
                    fields->Add(fieldArray);
                }
            }
        }
        else {
            mResults = mQuery->ExecuteQuery();

			FdoSmPhRdGrdRowArrayP rowArray;
			FdoSmPhRdGrdFieldArrayP fieldArray;
			int i, j;
			for ( i = 0; i < mFetches->GetCount(); i++ ) {
				rowArray = mFetches->GetItem(i);
				FdoSmPhRdGrdFieldArraysP pFields = rowArray->GetFields();
				for ( j = 0; j < pFields->GetCount(); j++ )	{
					fieldArray = pFields->GetItem(j);
					fieldArray->UpdateResults(mResults);
				}
			}
		}
    }
}

bool FdoSmPhRdGrdQueryReader::FieldIsModified( FdoStringP tableName, FdoStringP fieldName )
{
    bool isModified = false;
    FdoSmPhFieldP field = FdoSmPhRdQueryReader::GetField( tableName, fieldName );

    if ( field ) 
        isModified = field->GetIsModified();

    return isModified;
}


FdoSmPhRdGrdFieldArrayP FdoSmPhRdGrdQueryReader::GetFieldArray( FdoStringP tableName, FdoStringP fieldName)
{
    FdoSmPhRdGrdRowArrayP rowArray;
    FdoSmPhRdGrdFieldArrayP fieldArray;
    int cidx;

    if ( tableName.GetLength() > 0 ) {
        rowArray = mFetches->GetItem( tableName );
		if ( rowArray )
		{
			FdoSmPhRdGrdFieldArraysP pFields = rowArray->GetFields();
			fieldArray = pFields->GetItem( fieldName );
		}
    }
    else {
        for ( cidx = 0; cidx < mFetches->GetCount(); cidx++ ) {
            rowArray = mFetches->GetItem(cidx);
			FdoSmPhRdGrdFieldArraysP pFields = rowArray->GetFields();
            fieldArray = pFields->FindItem( fieldName );
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


