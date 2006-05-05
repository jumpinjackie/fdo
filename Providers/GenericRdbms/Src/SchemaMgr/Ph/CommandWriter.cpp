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
#include "CommandWriter.h"

FdoSmPhGrdCommandWriter::FdoSmPhGrdCommandWriter(FdoSmPhRowP row , FdoSmPhMgrP mgr) :
	FdoSmPhCommandWriter(row, mgr),
    mInsertStmt(NULL)
{
}

FdoSmPhGrdCommandWriter::~FdoSmPhGrdCommandWriter(void)
{
    if ( mInsertStmt ) {
        mInsertStmt->Free();
        delete mInsertStmt;
    }
}

void FdoSmPhGrdCommandWriter::Add()
{
    FdoSmPhGrdMgrP mgr = GetManager().p->SmartCast<FdoSmPhGrdMgr>();
    GdbiConnection* connection = mgr->GetGdbiConnection();
    GdbiCommands* cmds = connection->GetCommands();

    FdoInt32                    cidx;
    FdoStringP                  statement;
    FdoStringsP                 cols = FdoStringCollection::Create();
    FdoStringsP                 vals = FdoStringCollection::Create();
    FdoSmPhFieldsP              binds = new FdoSmPhFieldCollection;
    FdoSmPhFieldsP              pFields = GetRow()->GetFields();
    bool                        rebind = false;

    if ( !mInsertStmt ) {
        // Initialize the insert command.

        if ( !(GetRow()->GetDbObject()->GetExists()) ) 
            throw FdoSchemaException::Create (
	            NlsMsgGet1 (
		            FDORDBMS_364,
		            "Table %1$ls is not in the current datastore; datastore schema needs to be upgraded.",
		            (FdoString*) GetRow()->GetDbObject()->GetQName() 
	            )
            );

        // find each field to add to insert statement
        for ( cidx = 0; cidx < pFields->GetCount(); cidx++ ) {
            FdoSmPhFieldP pField = pFields->GetItem(cidx);

            // Get the column.
            FdoSmPhColumnP pColumn = pField->GetColumn();

            if ( pColumn->GetExists() ) {
                // Column exists or is creatable to add it to the insert list

                // get field name
                FdoStringP insCol = pField->GetUpdCol();
            
                // IDENTITY/AUTO_INCREMENT columns are not writeable, e.g. 'F_CLASSDEFINITION.classid'.
                if ( !cmds->IsFieldWriteable( GetRow()->GetName(), pColumn->GetName() ) )
                    continue;

                if ( insCol.GetLength() > 0 ) {
                    // get bind name for field value
                    FdoStringP insVal;
                    if ( pField->GetCanBind() ) {
                        insVal = mgr->FormatBindField( binds->GetCount() );
                        binds->Add(pField);
                    }
                    else {                        
                        insVal = pColumn->GetValueSql( pField->GetFieldValue() );
                    }

                    cols->Add( insCol );
                    vals->Add( insVal );
                }
                else {
		            throw FdoSchemaException::Create (
			            NlsMsgGet1 (
				            FDORDBMS_308,
				            "Cannot generate column name for %1$ls",
				            (FdoString*) pField->GetQName() 
			            )
		            );
                }
            }
        }

        statement = FdoStringP::Format( L"insert into %ls ( %ls ) values ( %ls )",
                        GetRow()->GetName(),
                        (FdoString*) cols->ToString(),
                        (FdoString*) vals->ToString()
                    );

        mInsertStmt = connection->Prepare( (const wchar_t*) statement );
    }
    else
    {
        // It still needs updating the null indicators, which are provider specific.
        binds = pFields;
        rebind = true;
    }

    // find each field to add to insert statement
    for ( cidx = 0; cidx < pFields->GetCount(); cidx++ ) {
        FdoSmPhFieldP pField = pFields->GetItem(cidx);

        // Get the column.
        FdoSmPhColumnP pColumn = pField->GetColumn();

        if ( !pColumn ) {
            if ( pField->GetFieldValue() != pField->GetDefaultValue() )
	            throw FdoSchemaException::Create (
		            NlsMsgGet1 (
			            FDORDBMS_312,
			            "Column %1$ls is not in the current datastore; datastore schema needs to be upgraded.",
			            (FdoString*) pField->GetQName() 
		            )
	            );
        }
    }

	// Bind the fields to the insert statement
    Bind( mInsertStmt, binds, rebind );
	
	// Do the insert
    mInsertStmt->ExecuteNonQuery();
}

void FdoSmPhGrdCommandWriter::Modify( FdoStringP sClauses )
{
    GdbiConnection* connection = ((FdoSmPhGrdMgr*)(FdoSmPhMgr*)GetManager())->GetGdbiConnection();
    GdbiStatement* updStmt = NULL;
    FdoInt32 cidx;
    FdoInt32 bindIdx = 0;
    FdoStringP statement;
    FdoStringsP exprs = FdoStringCollection::Create();

    FdoSmPhFieldsP pFields = GetRow()->GetFields();
    FdoSmPhFieldsP binds = new FdoSmPhFieldCollection();

    // Where clause not yet parameterized so generate statement each time.

    for ( cidx = 0; cidx < pFields->GetCount(); cidx++ ) {
        FdoSmPhFieldP pField = pFields->GetItem(cidx);

        // Update only fields that were set.
        if ( pField->GetIsModified() ) {
            FdoStringP updCol = pField->GetUpdCol();

            if ( updCol.GetLength() > 0 ) {
                exprs->Add( FdoStringP::Format(L"%ls = %ls", 
                                (FdoString*) updCol,
                                (FdoString*) GetManager()->FormatBindField( bindIdx++ )
                        )
                );
                binds->Add(pField);
            }
            else {
	            throw FdoSchemaException::Create (
		            NlsMsgGet1 (
			            FDORDBMS_312,
			            "Column %1$ls is not in the current datastore; datastore schema needs to be upgraded.",
			            (FdoString*) pField->GetQName() 
		            )
	            );
            }
        }
    }

    statement = FdoStringP::Format( L"update %ls set %ls  %ls",
                    GetRow()->GetName(),
                    (FdoString*) exprs->ToString(),
                    (FdoString*) sClauses
                );


    try {
	    // Create the update statement
        updStmt = connection->Prepare( (const wchar_t*) statement );

	    // Bind updated fields to the statement
        Bind( updStmt, binds );

	    // Do the update
        updStmt->ExecuteNonQuery();
    }
    catch (...) {
        if ( updStmt ) {
            try {
                updStmt->Free();
                delete updStmt;
            }
            catch (...) {
            }
        }
        throw;
    }

    updStmt->Free();
    delete updStmt;
}

void FdoSmPhGrdCommandWriter::Delete( FdoStringP sClauses )
{
    GdbiConnection* connection = ((FdoSmPhGrdMgr*)(FdoSmPhMgr*)GetManager())->GetGdbiConnection();
    GdbiStatement* delStmt = NULL;
    FdoStringP statement;

    statement = FdoStringP::Format( L"delete from %ls %ls",
                    GetRow()->GetName(),
                    (FdoString*) sClauses
                );

    try {
        delStmt = connection->Prepare( (const wchar_t*) statement );
        delStmt->ExecuteNonQuery();
    }
    catch (...) {
        if ( delStmt ) {
            try {
                delStmt->Free();
                delete delStmt;
            }
            catch (...) {
            }
        }
        throw;
    }

    delStmt->Free();
    delete delStmt;
}


void FdoSmPhGrdCommandWriter::Bind( GdbiStatement* stmt, FdoSmPhFieldsP binds, bool rebind )
{
    FdoInt32 cidx;

    if ( !rebind )
    {
        for ( cidx = 0; cidx < binds->GetCount(); cidx++ ) {
            FdoSmPhFieldP pField = binds->GetItem(cidx);
            if ( GetManager()->IsRdbUnicode() )
                stmt->Bind( cidx + 1, pField->GetBindSize(), (wchar_t*) pField->GetBindString(), pField->GetNullInd() );                
            else
                stmt->Bind( cidx + 1, pField->GetBindSize(), (char*) pField->GetBindString(), pField->GetNullInd() );                
        }
    } 

    // Set the value for the NI. It is provider specific.
    for ( cidx = 0; cidx < binds->GetCount(); cidx++ ) {
        FdoSmPhGrdMgrP mgr = GetManager().p->SmartCast<FdoSmPhGrdMgr>();
        GdbiConnection* connection = mgr->GetGdbiConnection();
        GdbiCommands* cmds = connection->GetCommands();
        FdoSmPhFieldP pField = binds->GetItem(cidx);

        if ( pField->GetCanBind() ) {
            if ( pField->GetFieldValue().GetLength() != 0)
                cmds->set_nnull(pField->GetNullInd(), 0, 0);
            else
                cmds->set_null(pField->GetNullInd(), 0, 0);
        }
    }
}

