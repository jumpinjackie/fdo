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
#include "DbObject.h"
#include "Owner.h"
#include "Rd/ColumnReader.h"
#include "ColumnChar.h"
#include "ColumnBool.h"
#include "ColumnByte.h"
#include "ColumnInt16.h"
#include "ColumnInt32.h"
#include "ColumnInt64.h"
#include "ColumnDecimal.h"
#include "ColumnDate.h"
#include "ColumnSingle.h"
#include "ColumnDouble.h"
#include "ColumnBLOB.h"
#include "ColumnGeom.h"
#include "ColumnUnknown.h"
#include <FdoCommonStringUtil.h>

FdoSmPhMySqlDbObject::FdoSmPhMySqlDbObject(
    FdoStringP name,
    const FdoSmPhOwner* pOwner,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhDbObject(name, pOwner ),
    mAutoIncrementColumnSeed(1),
    mStorageEngine(MySQLOvStorageEngineType_MyISAM)
{
    // Read override info from database:
    if (reader != NULL)
    {
        //TODO: get autoincrement column name from column reader instead of table reader:
		// mAutoIncrementColumnName = reader->GetString(L"", L"autoincrement_column_name");
		mAutoIncrementColumnName= L"";

        //TODO: GetInteger() returns int which can't handle 64 bit values, which is what seed is
        mAutoIncrementColumnSeed = reader->GetInteger(L"", L"autoincrement_column_seed");
        mStorageEngine = StorageEngineStringToEnum(reader->GetString(L"", L"storage_engine"));
        mDataDirectory = reader->GetString(L"", L"data_directory");
        mIndexDirectory = reader->GetString(L"", L"index_directory");

        // TEMPORARY WORKAROUNDS - BEGIN:
        // Need to add a FdoSmPhRdDbObjectReader::IsNull() method to detect NULL values for column seed:
        if (mAutoIncrementColumnSeed==0)
            mAutoIncrementColumnSeed = 1;
        // Need to find a way to actually fetch the data/index directories, currently always get " ":
        if (mDataDirectory == L" ")
            mDataDirectory = L"";
        if (mIndexDirectory == L" ")
            mIndexDirectory = L"";
        // TEMPORARY WORKAROUNDS - END.
    }

}

FdoSmPhMySqlDbObject::~FdoSmPhMySqlDbObject(void)
{
}

FdoStringP FdoSmPhMySqlDbObject::GetDbQName() const
{
    return ( ((const FdoSmPhDbElement*)GetParent())->GetDbName() + L"." + GetDbName() );
}

bool FdoSmPhMySqlDbObject::GetSupportsWrite() const
{
    // It has been decided to return "true" as the default value because it has been deemed
    // to expensive to determine whether or not the class is writable. 
    return true;
}

FdoStringsP FdoSmPhMySqlDbObject::GetKeyColsSql( FdoSmPhColumnCollection* columns )
{
    FdoInt32        i;
    FdoStringsP     colClauses = FdoStringCollection::Create();
    FdoInt32        colMaxLen = mColTruncMinLen;  // Start at smallest allowed truncated length
    FdoInt32        largeColCount = 0;
    FdoInt32        prevLargeColCount = 0;
    FdoInt32        smallColTotal = 0;

    for ( ; ; ) {
        smallColTotal = 0;
        largeColCount = 0;

        // The following loop calculates 2 things:
        //  - number of large columns that need to be truncated
        //  - total size of columns not requiring truncation

        for ( i = 0; i < columns->GetCount(); i++ ) {
            FdoSmPhColumnP column = columns->GetItem(i);
            FdoInt64 colSize = column->GetDbBinarySize();
            
            if ( colSize <= colMaxLen )
                smallColTotal += (FdoInt32) colSize;
            else
                largeColCount++;
        }

        if ( (largeColCount == 0) || (largeColCount == prevLargeColCount) ) 
            // No large columns or number of large columns 
            // has not decreased, so we're ready to generate 
            // the key column clause
            break;

        // Increase the truncation length to the maximum possible:
        // (max_key_size - space_for_small_columns) / #large_columns
        colMaxLen = (FdoInt32)((mKeyMaxLen - smallColTotal) / largeColCount);

  /* TODO Add error message when more can be added to catalogue.
 * for now, just let MySQL report constraints that are to big.
        if ( colMaxLen < mColTruncMinLen ) {
            //TODO: add an error
            return colClauses;
        }
*/
        // The truncation length must not exceed the maximum. 
        if ( colMaxLen > mColTruncMaxLen ) {
            colMaxLen = mColTruncMaxLen;
            break;
        }

        prevLargeColCount = largeColCount;

        // Truncation length increase can cause large column
        // count to decrease. Loop again to recalculate 
        // large column count. Keep going until large column
        // count stabilizes.
    }

    // Generate key column clause for each column.
    for ( i = 0; i < columns->GetCount(); i++ ) {
        FdoSmPhColumnP column = columns->GetItem(i);

        // Initially assume no truncation required.
        FdoInt32 constrColLen = 0;

        if ( (largeColCount > 0) && (column->GetDbBinarySize() > colMaxLen) )
            // Truncating and column needs to be truncated
            constrColLen = colMaxLen;
        else if ( (column->GetType() == FdoSmPhColType_String) && (column->GetTypeName().ICompare(L"varchar") != 0) )
            // text key columns always need length specified since 
            // maximum length for various text types is greater
            // than maximum key length. In this case we don't 
            // actually truncate, just set the length explicitly.
            constrColLen = column->GetLength();
            
        if ( constrColLen > 0  )
            // Generate key column clause with length specification
            colClauses->Add( 
                FdoStringP::Format( 
                    L"%ls(%d)",
                    (FdoString*)(columns->GetItem(i)->GetDbName()),
                    constrColLen
                )
            );
        else
            // No length specification needed.
            colClauses->Add( (FdoString*)(columns->GetItem(i)->GetDbName()) );
    }

    return colClauses;
}

void FdoSmPhMySqlDbObject::ActivateOwnerAndExecute( FdoStringP sqlStmt )
{
    FdoSmPhMySqlOwner*        objOwner       = static_cast<FdoSmPhMySqlOwner*>((FdoSmPhSchemaElement*) GetParent());

    objOwner->ActivateAndExecute( sqlStmt );
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnBLOB(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnBLOB( columnName, elementState, this, bNullable, rootColumnName, colRdr );
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnChar(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    int length,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnChar(columnName, elementState, this, bNullable, length, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnDate(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnDate(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnDecimal(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    int length,
    int scale,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnDecimal(columnName, elementState, this, bNullable, length, scale, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnSingle(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnSingle(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnDouble(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnDouble(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnGeom(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    FdoSmPhScInfoP AssociatedSCInfo,
    bool bNullable,
    bool bHasElevation,
    bool bHasMeasure,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnGeom(columnName, elementState, this, AssociatedSCInfo, bNullable, bHasElevation, bHasMeasure, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnBool(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnBool(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnByte(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnByte(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnInt16(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    bool bIsAutoincremented,    // Not supported
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnInt16(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnInt32(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    bool bIsAutoincremented,    // Not supported
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnInt32(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnInt64(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
	bool bIsAutoincremented,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnInt64(columnName, elementState, this, bNullable, bIsAutoincremented, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnUnknown(
    FdoStringP columnName,
    FdoStringP typeName,
    FdoSchemaElementState elementState,
    bool bNullable,
    int length,
    int scale,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnUnknown(columnName, typeName, elementState, this, bNullable, length, scale, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhMySqlDbObject::NewColumnDbObject(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhMySqlColumnChar(
        columnName, 
        elementState, 
        this, 
        bNullable, 
        (int) GetManager()->DbObjectNameMaxLen(), 
        rootColumnName,
        colRdr
    );
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhMySqlDbObject::CreateColumnReader()
{
    return new FdoSmPhRdMySqlColumnReader( GetManager(), FDO_SAFE_ADDREF(this) );
}


MySQLOvStorageEngineType FdoSmPhMySqlDbObject::StorageEngineStringToEnum(FdoString* storageEngine)
{
    MySQLOvStorageEngineType ret;

    if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"MyISAM"))
        ret = MySQLOvStorageEngineType_MyISAM;
    else if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"ISAM"))
        ret = MySQLOvStorageEngineType_ISAM;
    else if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"InnoDB"))
        ret = MySQLOvStorageEngineType_InnoDB;
    else if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"BDB"))
        ret = MySQLOvStorageEngineType_BDB;
    else if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"Merge"))
        ret = MySQLOvStorageEngineType_Merge;
    else if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"MEMORY"))
        ret = MySQLOvStorageEngineType_Memory;
    else if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"Federated"))
        ret = MySQLOvStorageEngineType_Federated;
    else if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"Archive"))
        ret = MySQLOvStorageEngineType_Archive;
    else if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"CSV"))
        ret = MySQLOvStorageEngineType_CSV;
    else if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"Example"))
        ret = MySQLOvStorageEngineType_Example;
    else if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"NDBCluster"))
        ret = MySQLOvStorageEngineType_NDBClustered;
    // NOTE: I don't think it makes sense to handle a Default engine here, since it will never appear when querying the database
    //else if (0==FdoCommonStringUtil::StringCompareNoCase(storageEngine, L"Default"))
    //    ret = MySQLOvStorageEngineType_Default;
    else
        ret = MySQLOvStorageEngineType_Unknown;

    return ret;
}


FdoString* FdoSmPhMySqlDbObject::StorageEngineEnumToString(MySQLOvStorageEngineType storageEngine)
{
    FdoString* ret;

    if (storageEngine == MySQLOvStorageEngineType_MyISAM)
        ret = L"MyISAM";
    else if (storageEngine == MySQLOvStorageEngineType_ISAM)
        ret = L"ISAM";
    else if (storageEngine == MySQLOvStorageEngineType_InnoDB)
        ret = L"InnoDB";
    else if (storageEngine == MySQLOvStorageEngineType_BDB)
        ret = L"BDB";
    else if (storageEngine == MySQLOvStorageEngineType_Merge)
        ret = L"Merge";
    else if (storageEngine == MySQLOvStorageEngineType_Memory)
        ret = L"MEMORY";
    else if (storageEngine == MySQLOvStorageEngineType_Federated)
        ret = L"Federated";
    else if (storageEngine == MySQLOvStorageEngineType_Archive)
        ret = L"Archive";
    else if (storageEngine == MySQLOvStorageEngineType_CSV)
        ret = L"CSV";
    else if (storageEngine == MySQLOvStorageEngineType_Example)
        ret = L"Example";
    else if (storageEngine == MySQLOvStorageEngineType_NDBClustered)
        ret = L"NDBCluster";
    else // includes MySQLOvStorageEngineType_Unknown and MySQLOvStorageEngineType_Default cases
        ret = L"MyISAM";  // If we don't specify a default, MySQL 5 will default to InnoDB which does NOT
                          // support geometry so is not a good default choice.

    return ret;
}
