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
#include "Owner.h"
#include "Mgr.h"
#include "Rd/ConstraintReader.h"

FdoSmPhMySqlTable::FdoSmPhMySqlTable(
    FdoStringP name,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState,
    FdoStringP pkeyName,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdTable(name, pOwner, elementState, pkeyName),
    FdoSmPhMySqlDbObject(name, pOwner, reader),
    FdoSmPhDbObject(name, pOwner, elementState)
{
}

FdoSmPhMySqlTable::~FdoSmPhMySqlTable(void)
{
}

const FdoLockType* FdoSmPhMySqlTable::GetLockTypes(FdoInt32& size) const
{
    // Most types of MySql tables do not support transactions and thus
    // do not support any type of locking.
    size = 0;
    return (FdoLockType*) NULL;
    // TODO call base GetLockTypes when storage engine is InnoDB or BDB 
    // (these engines do support transactions).
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhMySqlTable::CreateConstraintReader( FdoString* type ) const
{
	FdoSmPhMySqlOwner* pMySqlOwner = static_cast<FdoSmPhMySqlOwner*>((FdoSmPhDbElement*) GetParent());

    return new FdoSmPhRdMySqlConstraintReader( pMySqlOwner, GetName(), type );
}

void FdoSmPhMySqlTable::CommitChildren( bool isBeforeParent )
{
    if ( isBeforeParent ) 
    {
        // At this point all string columns with length 255 or less have type varchar.
        // Columns with length 256-65535 have type text. In MySQL 5.0.3 the maximum for varchar
        // columns was increased to 65535 bytes, in which case, we might be able to change
        // some of the text columns to varchar. 

        FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
        FdoInt32 maxVarcharLen = mgr->GetVarcharMaxLen();

        if ( maxVarcharLen > 255 ) 
        {
            // If we get here, the MySQL server is 5.0.3 or later. 
            int stringWDefaultTotal = 0;
            int stringTotal = 0;
            int otherTotal = 0;
            int maxTotal = 65534;
            int stringCount = 0;

            // The total row size cannot exceed maxTotal so we need to determine the current
            // row size to know how many text columns can be switched to varchar. For switching,
            // priority is given to columns with default values since a default cannot be set
            // for a text column.
            //
            // Check each column and total up the following (in bytes)
            //
            //    - size of new string columns with default value
            //    - size of new string columns without default value
            //    - size of other columns

            FdoSmPhColumnsP columns = GetColumns();
            FdoSmPhColumnsP strColumns = new FdoSmPhColumnCollection();

            for ( int ix = 0; ix < columns->GetCount(); ix++ ) 
            {
                FdoSmPhColumnP column = columns->GetItem(ix);

                int binarySize = column->GetDbBinarySize();

                if ( column->GetType() == FdoSmPhColType_String )
                    // For storage, MySQL seems to need an extra 2 bytes in addition to 
                    // what is needed to store the string values.
                    binarySize += 2;

                if ( (column->GetElementState() != FdoSchemaElementState_Added) ||
                     (column->GetType() != FdoSmPhColType_String) ||
                     (binarySize <= 10) ||
                     ((column->GetTypeName().ICompare(L"VARCHAR") != 0) && (column->GetTypeName().ICompare(L"TEXT") != 0))
                )
                {
                    // The other column total consists of all existing columns plus any new
                    // columns that are neither varchar nor text. Also count varchar columns
                    // of size under 10 bytes. Nothing is gained by switching these to text columns.
                    
                    FdoStringP upperType = column->GetTypeName().Upper();
                    
                    if ( upperType.Contains(L"TEXT") || upperType.Contains(L"BLOB") )
                        // existing blob type columns only take 10 bytes from the row itself
                        otherTotal += 10;
                    else
                        otherTotal += binarySize;
                }
                else if ( column->GetDefaultValue() )
                {
                    // new string column with default
                    stringWDefaultTotal += binarySize;
                    // Also add it to an ordered list
                    AddToStrCols( strColumns, column );
                }
                else
                {
                    // new string column without default
                    stringTotal += binarySize;
                    stringCount++;
                    // Also add it to an ordered list
                    AddToStrCols( strColumns, column );
                }
            }

            // Determine the target total, the size we will allow the row to grow. 
            // Must include otherTotal since these included column sizes that can't change.
            // Also, try to accomodate all new string columns with defaults as varchar columns.
            // Allocate 3/4 of the remainder to string columns without defaults. The other 
            // 1/4 can accomodate columns that are later added to this table.
            int targetTotal = maxTotal - ((maxTotal - otherTotal - stringWDefaultTotal - (10 * stringCount)) / 4);

            if ( targetTotal > maxTotal ) 
                // Target total too big so calculate it as the fixed total plus 3/4 of the 
                // remainder for new string columns. This table will fail to commit since
                // some string columns with default will get the text type. However, continue
                // on and let MySQL generate the error message.
                targetTotal = maxTotal - ((maxTotal - otherTotal) / 4);

            // Determine the current row size
            int currTotal = otherTotal + stringTotal + stringWDefaultTotal;

            if ( targetTotal <= maxTotal ) 
            {
                // Go through the sorted list for new string columns. 
                // The list has columns without defaults first, followed by columns
                // with defaults. Within each of these 2 categories columns are ordered
                // from biggest to smallest. 
                //
                // Go through the list and assign the first columns to the text type,
                // until the row size drops below the target. Columns after that become
                // varchar. This minimizes the number of string columns that become text
                // and maximizes the number of string columns with default, that become varchar.
                for ( int jx = 0; jx < strColumns->GetCount(); jx++ ) 
                {
                    FdoSmPhColumnP column = strColumns->GetItem(jx);

                    if ( currTotal > targetTotal ) 
                    {
                        // Column currently counted as size+2.
                        // Text column takes up 10 bytes in the row, so setting column
                        // type to text reduces row size by column size + 2 - 10
                        currTotal -= (column->GetDbBinarySize() - 8);
                        column->SetTypeName(L"TEXT");
                    }
                    else
                    {
                        // We're under the target size so make this column varchar.
                        column->SetTypeName(L"VARCHAR");
                    }
                }
            }
        }
    }

    FdoSmPhTable::CommitChildren( isBeforeParent );
}

void FdoSmPhMySqlTable::AddToStrCols( FdoSmPhColumnsP cols, FdoSmPhColumnP col )
{
    int colSize = col->GetDbBinarySize();
    int dflt = col->GetDefaultValue() ? 1 : 0;
    int ix = 0;

    for ( ix = 0; ix < cols->GetCount(); ix++ ) 
    {
        FdoSmPhColumnP curCol = cols->GetItem(ix);
        int curSize = curCol->GetDbBinarySize();
        int curDflt = curCol->GetDefaultValue() ? 1: 0;

        if ( curDflt > dflt ) 
        {
            cols->Insert(ix, col);
            break;
        }
        else if ( curDflt == dflt ) 
        {
            if ( colSize >= curSize ) 
            {
                cols->Insert(ix, col);
                break;
            }
        }
    }

    if ( ix >= cols->GetCount() ) 
        cols->Add(col);
}

bool FdoSmPhMySqlTable::Add()
{
    FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"%ls %ls",
        (FdoString*) GetAddSql(),
        (FdoString*) GetAddStorageSql()
    );


    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt, true );

    return true;
}

bool FdoSmPhMySqlTable::Modify()
{
    // Nothing to modify yet.
    return true;
}

bool FdoSmPhMySqlTable::Delete()
{
    FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = GetDeleteSql();

    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt, true );

    return true;
}

FdoSmPhFkeyP FdoSmPhMySqlTable::NewFkey(
    FdoStringP name,
    FdoStringP pkeyTableName,
    FdoStringP pkeyTableOwner,
    FdoSchemaElementState elementState
)
{
    return new FdoSmPhMySqlFkey( name, this, pkeyTableName, pkeyTableOwner, elementState );
}

FdoStringP FdoSmPhMySqlTable::GetAddStorageSql()
{
    FdoStringP storageSql;

    // Validate that the storage engine is acceptable for table creation:
    if (   (GetStorageEngine() == MySQLOvStorageEngineType_Memory)
        || (GetStorageEngine() == MySQLOvStorageEngineType_Archive)
        || (GetStorageEngine() == MySQLOvStorageEngineType_Example)
        )
        throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_436, "The MySQL storage engine '%1$ls' is not supported for creation of new tables", StorageEngineEnumToString(GetStorageEngine())));

    // Always specify the storage engine, since our default (MyISAM) differs from MySQL's default (InnoDB):
    storageSql += FdoStringP::Format(L" ENGINE=%ls", StorageEngineEnumToString(GetStorageEngine()));

    if (GetAutoIncrementColumnSeed() != 1)
        storageSql += FdoStringP::Format(L" AUTO_INCREMENT=%I64d", GetAutoIncrementColumnSeed());

    if (wcslen(GetDataDirectory()) > 0)
        storageSql += FdoStringP::Format(L" DATA DIRECTORY='%ls'", GetDataDirectory());

    if (wcslen(GetIndexDirectory()) > 0)
        storageSql += FdoStringP::Format(L" INDEX DIRECTORY='%ls'", GetIndexDirectory());

    return storageSql;
}

FdoStringP FdoSmPhMySqlTable::GetDropConstraintSql(FdoStringP constraintName )
{
    return FdoStringP::Format( 
        L"alter table %ls drop index %ls", 
        (FdoString*) GetDDLQName(),
		(FdoString *)GetConstraintDDLName(constraintName)
    );
}
