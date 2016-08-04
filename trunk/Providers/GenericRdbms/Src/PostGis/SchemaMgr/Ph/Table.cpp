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
#include "Table.h"
#include "Owner.h"
#include "Column.h"
#include "ColumnGeom.h"
#include "Mgr.h"
#include "Rd/PkeyReader.h"
#include "Rd/FkeyReader.h"
#include "Rd/ConstraintReader.h"

FdoSmPhPostGisTable::FdoSmPhPostGisTable()
{
}

FdoSmPhPostGisTable::FdoSmPhPostGisTable(FdoStringP name,
    const FdoSmPhOwner* owner,
    FdoSchemaElementState state,
    FdoStringP pkeyName,
    FdoSmPhRdDbObjectReader* reader)
    : FdoSmPhGrdTable(name, owner, state, pkeyName),
      FdoSmPhPostGisDbObject(name, owner, reader),
      FdoSmPhDbObject(name, owner, state)
{
}

FdoSmPhPostGisTable::~FdoSmPhPostGisTable()
{
}

const FdoLockType* FdoSmPhPostGisTable::GetLockTypes(FdoInt32& size) const
{
    // TODO: mloskot - Check this notes for PostgreSQL

    // Most types of MySQL tables do not support transactions and thus
    // do not support any type of locking.
    size = 0;
    return NULL;
}

FdoSmPhDbObjectsP FdoSmPhPostGisTable::GetSubDeletedTables()
{
    if ( !mSubDeletedTables )
        mSubDeletedTables = new FdoSmPhDbObjectCollection( NULL );

    return mSubDeletedTables;
}

void FdoSmPhPostGisTable::SetElementState(FdoSchemaElementState elementState)
{
    FdoSmPhGrdTable::SetElementState( elementState );

    if ( elementState == FdoSchemaElementState_Deleted ) {
        FdoSmPhBaseObjectsP baseObjects = this->GetBaseObjects();
        FdoInt32 idx;

        for ( idx = 0; idx < baseObjects->GetCount(); idx ++ ) {
            FdoSmPhBaseObjectP baseObject = baseObjects->GetItem( idx );
            FdoSmPhDbObjectP baseDbObject = baseObject->GetDbObject();
            FdoSmPhPostGisTableP baseTable = baseDbObject.p->SmartCast<FdoSmPhPostGisTable>();
            if ( baseTable )
                baseTable->GetSubDeletedTables()->Add( this );
        }
    }
}


FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhPostGisTable::CreatePkeyReader() const
{
    // Remove const qualifiers cleanly
    FdoSmPhPostGisTable* thisTable = NULL;
    thisTable = const_cast<FdoSmPhPostGisTable*>(this);
    FDO_SAFE_ADDREF(thisTable);

    // Create primary key reader
    FdoSmPhRdPostGisPkeyReader* reader = NULL;
    FdoSmPhOwnerP owner = FDO_SAFE_ADDREF(static_cast<FdoSmPhOwner*>(const_cast<FdoSmSchemaElement*>(GetParent())));
    reader = new FdoSmPhRdPostGisPkeyReader(
        owner, thisTable);
    
    return reader;
}

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhPostGisTable::CreateFkeyReader() const
{
    // Remove const qualifiers cleanly
    FdoSmPhPostGisTable* thisTable = NULL;
    thisTable = const_cast<FdoSmPhPostGisTable*>(this);
    FDO_SAFE_ADDREF(thisTable);

    // Create foreign key reader
    FdoSmPhRdPostGisFkeyReader* reader = NULL;
    FdoSmPhOwnerP owner = FDO_SAFE_ADDREF(static_cast<FdoSmPhOwner*>(const_cast<FdoSmSchemaElement*>(GetParent())));
    reader = new FdoSmPhRdPostGisFkeyReader(
        owner, thisTable);

    return reader;
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhPostGisTable::CreateConstraintReader(
    FdoString* type) const
{
    // Remove const qualifier
    FdoSmSchemaElement* smElement = NULL;
    smElement = const_cast<FdoSmSchemaElement*>(GetParent());

    FdoSmPhDbElement* dbElement = NULL;
    dbElement = static_cast<FdoSmPhDbElement*>(smElement);

	FdoSmPhPostGisOwner* pgOwner = NULL;
    pgOwner = static_cast<FdoSmPhPostGisOwner*>(dbElement);

    // Create constraint reader
    FdoSmPhRdPostGisConstraintReader* reader = NULL;
    reader = new FdoSmPhRdPostGisConstraintReader(pgOwner, GetName(), type);

    return reader;
}

void FdoSmPhPostGisTable::CommitChildren( bool isBeforeParent )
{
    if ( isBeforeParent && (this->GetCommitState() == FdoSchemaElementState_Deleted) ) {
        FdoInt32 idx;
        FdoSmPhDbObjectsP subDeletedTables = GetSubDeletedTables();

        for ( idx = 0; idx < subDeletedTables->GetCount(); idx++ ) {
            FdoSmPhDbObjectP subDeletedTable = subDeletedTables->GetItem( idx );
            subDeletedTable->Commit(true, isBeforeParent );
        }
    }

    FdoSmPhGrdTable::CommitChildren( isBeforeParent );
}

void FdoSmPhPostGisTable::Commit( bool fromParent, bool isBeforeParent )
{
    if ( GetCommitState() != FdoSchemaElementState_Deleted && GetElementState() != FdoSchemaElementState_Deleted ) {
        FdoSmPhDbObjectP rootObject = this->GetRootObject();

        if ( rootObject ) 
            rootObject->Commit(fromParent, isBeforeParent);
    }

    FdoSmPhDbElement::Commit( fromParent, isBeforeParent );
}


bool FdoSmPhPostGisTable::Add()
{
    FdoSmPhPostGisMgrP mgr(GetManager()->SmartCast<FdoSmPhPostGisMgr>());

    GdbiConnection* gdbiConn = NULL;
    gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = FdoStringP::Format(L"%ls ",
        static_cast<FdoString*>(GetAddSql()));

    FdoSmPhBaseObjectsP baseObjects = GetBaseObjects();
    if ( baseObjects->GetCount() > 0 ) {
        FdoSmPhDbObjectP baseObject = FdoSmPhBaseObjectP(baseObjects->GetItem(0))->GetDbObject();
        sqlStmt = sqlStmt + FdoStringP::Format( L" inherits(%ls)", (FdoString*)baseObject->GetDbQName() );
    }

    gdbiConn->ExecuteNonQuery(
        static_cast<const char*>(sqlStmt), true);

    return true;
}

bool FdoSmPhPostGisTable::Modify()
{
    // Nothing to modify yet.
    return true;
}

bool FdoSmPhPostGisTable::Delete()
{
    FdoSmPhPostGisMgrP mgr(GetManager()->SmartCast<FdoSmPhPostGisMgr>());
    
    // Must be sure gdbiConn is dereferencable
    GdbiConnection* gdbiConn = NULL;
    gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt(GetDeleteSql());

    gdbiConn->ExecuteNonQuery(
        static_cast<const char*>(sqlStmt), true);

    return true;
}

bool FdoSmPhPostGisTable::AddColumn( FdoSmPhColumnP column )
{
    bool ret = true;

    FdoSmPhColumnP baseColumn = dynamic_cast<FdoSmPhPostGisColumn*>(column.p)->GetBaseColumn();

    if ( (!baseColumn) || (baseColumn->GetElementState() == FdoSchemaElementState_Deleted) ) {
        FdoSmPhPostGisColumnGeomP columnGeom = column->SmartCast<FdoSmPhPostGisColumnGeom>();
        if ( columnGeom ) {
            if ( GetElementState() == FdoSchemaElementState_Unchanged )
                columnGeom->Commit( true, false );
            else
                ret = false;
        }
        else {
            ret = FdoSmPhGrdTable::AddColumn( column );
        }
    }

    return ret;
}

bool FdoSmPhPostGisTable::DeleteColumn( FdoSmPhColumnP column )
{
    bool ret = true;

    // Skip dropping inherited geometry columns. They are automatically
    // dropped when the base column is dropped.
    // However, other types of inherited columns do not go away unless 
    // explicitly dropped.

    if ( !dynamic_cast<FdoSmPhPostGisColumn*>(column.p)->GetBaseColumn() || !dynamic_cast<FdoSmPhPostGisColumnGeom*>(column.p) ) 
        ret = FdoSmPhGrdTable::DeleteColumn( column );

    return ret;
}

FdoSmPhFkeyP FdoSmPhPostGisTable::NewFkey(FdoStringP name,
    FdoStringP pkeyTableName,
    FdoStringP pkeyTableOwner,
    FdoSchemaElementState state)
{
    FdoSmPhPostGisFkey* fkey = NULL;
    fkey = new FdoSmPhPostGisFkey(name, this, pkeyTableName,
        pkeyTableOwner, state);

    return fkey;
}

FdoStringP FdoSmPhPostGisTable::GetAddHdgSql()
{
    return FdoStringP::Format( 
        L"create table %ls", 
        (FdoString*) GetDbQName() 
    );
}

FdoStringP FdoSmPhPostGisTable::GetConstraintDDLName( FdoStringP constraintName ) const
{
    return FdoStringP(L"\"")+constraintName+L"\"";
}

FdoStringP FdoSmPhPostGisTable::ConvertCkey( FdoDataValue *val )
{
	FdoStringP	ckey = GetManager()->FormatSQLVal(val);

    return ckey;
}

FdoStringP FdoSmPhPostGisTable::GetCkeyClause( FdoStringP columnName, FdoDataPropertyP fdoProp )
{
	FdoStringP	ckey = L"";

	FdoPtr<FdoPropertyValueConstraint>  valConstr = fdoProp->GetValueConstraint();

	if ( valConstr && (valConstr->GetConstraintType() == FdoPropertyValueConstraintType_Range) ) 
	{
		FdoPropertyValueConstraintRange *c = static_cast<FdoPropertyValueConstraintRange*>( valConstr.p );
		
		if ( FdoPtr<FdoDataValue>(c->GetMinValue()) != NULL )
		{
    		ckey += FdoStringP(L"\"") + columnName + L"\"";
			ckey += c->GetMinInclusive() ? L" >= " : L" > ";
			FdoPtr<FdoDataValue>  val = c->GetMinValue();
	
			ckey += ConvertCkey( val );
			ckey += L" ";
		}

		if ( FdoPtr<FdoDataValue>(c->GetMaxValue()) != NULL )
		{
			if ( ckey != L"" )
				ckey += L" and ";
    		ckey += FdoStringP(L"\"") + columnName + L"\"";
			ckey += c->GetMaxInclusive() ? L" <= " : L" < ";
			FdoPtr<FdoDataValue>  val = c->GetMaxValue();

			ckey += ConvertCkey( val );
			ckey += L" ";
		}
	} 
	else if ( valConstr && (valConstr->GetConstraintType() == FdoPropertyValueConstraintType_List) ) 
	{	
		FdoPropertyValueConstraintList *c = static_cast<FdoPropertyValueConstraintList*>( valConstr.p );
		FdoPtr<FdoDataValueCollection>  lst = c->GetConstraintList();
		
		if ( lst->GetCount() == 0 )
			return L"";

   		ckey += FdoStringP(L"\"") + columnName + L"\"";
		ckey += L" in (";

		for ( int i = 0; i < lst->GetCount(); i++ ) 
		{
			FdoPtr<FdoDataValue>  val = lst->GetItem(i);
			FdoDataType	type = val->GetDataType();

			// This shouldn't happen - check needed when creating the list
			if ( type == FdoDataType_BLOB || type == FdoDataType_CLOB )
				continue; 
	
			ckey += ConvertCkey( val );

			if ( i != (lst->GetCount() - 1))
				ckey += L", ";
		}
		ckey += L")";
	}
	else
	{
		// Cannot happen since one cannot set the constraint type;  
	}

	return ckey;
}

void FdoSmPhPostGisTable::LoadCkeys( FdoSmPhReaderP ckeyRdr, bool isSkipAdd )
{
    FdoSmPhCheckConstraintP  ckeyCurr;

    // read each check constraint column.
    while (ckeyRdr && ckeyRdr->ReadNext() ) {

        FdoStringP ckeyName			= ckeyRdr->GetString(L"", L"constraint_name");
        FdoStringP columnName		= ckeyRdr->GetString(L"", L"column_name");
    	FdoStringP clause			= ckeyRdr->GetString(L"", L"check_clause");

		if ( clause == L"" || clause.Contains(L"NOT NULL"))
			continue;

        if ( isSkipAdd ) 
            continue;

        // PostGis constraint reader gets column_name as a 1-based column position.
        FdoInt32 columnPosition = columnName.ToLong();
        // Get the column based on this position
        FdoSmPhColumnP ckeyColumn = Position2Column(columnPosition);

        // Check column must be in this table.
    	if ( ckeyColumn == NULL ) {
		    if ( GetElementState() != FdoSchemaElementState_Deleted )
		        AddCkeyColumnError( columnName );
		}
        else {
            ckeyCurr = new FdoSmPhCheckConstraint( ckeyName, ckeyColumn->GetName(), clause );
            this->AddCkeyCol( ckeyCurr );
        }
	}
}

bool FdoSmPhPostGisTable::LoadUkeyColumn( FdoSmPhReaderP ukeyRdr, FdoSmPhColumnsP ukey  )
{
    // PostGis unique key reader sets column_name to a list of 1-based positions (1 per
    // unique key column,
    FdoStringP columnName		= ukeyRdr->GetString(L"", L"column_name");
    // Parse out enclosing {}
    columnName                  = columnName.Mid( 1, columnName.GetLength() - 2 );
    // Positions are comma separated
    FdoStringsP columnPositions = FdoStringCollection::Create( columnName, L"," );

    for ( FdoInt32 ix = 0; ix < columnPositions->GetCount(); ix++ ) {
        FdoStringP columnPositionStr = columnPositions->GetString( ix );
        FdoInt32 columnPosition = columnPositionStr.ToLong();

        FdoSmPhColumnP ukeyColumn = Position2Column(columnPosition);;
        
        // Unique Key column must be in this table.
        if ( ukeyColumn == NULL ) {
	        if ( GetElementState() != FdoSchemaElementState_Deleted )
	            AddUkeyColumnError( columnName );

            return false;
        }

        ukey->Add( ukeyColumn );
    }

    return true;
}

