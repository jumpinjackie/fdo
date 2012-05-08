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
#include "Table.h"
#include "Mgr.h"

FdoSmPhGrdTable::FdoSmPhGrdTable(
    FdoStringP name,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState,
    FdoStringP pkeyName
) :
    FdoSmPhTable(name, pOwner, elementState, pkeyName),
    FdoSmPhDbObject( name, pOwner, elementState )
{
}

FdoSmPhGrdTable::~FdoSmPhGrdTable(void)
{
}

void FdoSmPhGrdTable::ClearRows()
{
    FdoSmPhGrdMgrP      mgr = GetManager().p->SmartCast<FdoSmPhGrdMgr>();
    GdbiConnection*     gdbiConn = mgr->GetGdbiConnection();
    FdoStringP          sqlStmt;

    sqlStmt = FdoStringP::Format(
        L"delete from %ls",
        (FdoString*) GetDbQName()
        );

    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );
}

void FdoSmPhGrdTable::InitFdoLocking( FdoStringP colName )
{
    if ( RefColumns()->RefItem(colName) ) 
        FdoSmPhDbObject::SetLockingMode( FdoMode );
}

void FdoSmPhGrdTable::InitFdoLt( FdoStringP colName )
{
    if ( RefColumns()->RefItem(colName) ) 
        FdoSmPhDbObject::SetLtMode( FdoMode );
}



bool FdoSmPhGrdTable::AddColumn( FdoSmPhColumnP column )
{
    if ( GetElementState() != FdoSchemaElementState_Unchanged ) {
        // Wait until table modified before adding the column.
        return false;
    }

    // Skip adding column if table was just added. Table add took care
    // of column add.
    if ( GetCommitState() != FdoSchemaElementState_Added ) {
        // Sofar "add column" clause seems to be RDBMS-neutral
        FdoStringP sqlStmt = FdoStringP::Format(
            L"%ls %ls",
            (FdoString*) GetAddColSql(),
            (FdoString*) column->GetAddSql()
        );

        ExecuteDDL( sqlStmt );
    }

    column->Commit( true );

    return true;
}

bool FdoSmPhGrdTable::ModifyColumn( FdoSmPhColumnP column )
{
    return true;
}

bool FdoSmPhGrdTable::DeleteColumn( FdoSmPhColumnP column )
{

    FdoSchemaElementState elementState = GetElementState();

    // Skip deleting column if table was deleted or added.
    if ( (elementState == FdoSchemaElementState_Modified) ||
         (elementState == FdoSchemaElementState_Unchanged) 
    ) {
        FdoStringP sqlStmt = FdoStringP::Format(
            L"%ls %ls",
            (FdoString*) GetDeleteColSql(),
            (FdoString*) column->GetDbName()
        );

        ExecuteDDL( sqlStmt );
    }

    column->Commit( true, true );

    return true;
}

void FdoSmPhGrdTable::DropConstraint( FdoStringP constraintName )
{
	FdoStringP sqlStmt = GetDropConstraintSql(constraintName); 

	// Ignore the error
	try	{
		ExecuteDDL( sqlStmt );
	} catch (FdoException *ex) {
		ex->Release();
	}
}

bool FdoSmPhGrdTable::AddConstraint( FdoStringP constraint)
{
	bool	status = true;

	FdoStringP sqlStmt = GetAddConstraintSql(constraint);

	try	{
		ExecuteDDL( sqlStmt );
	} catch (FdoException *ex) {
		ex->Release();
		status = false;
	}
	return status;
}

