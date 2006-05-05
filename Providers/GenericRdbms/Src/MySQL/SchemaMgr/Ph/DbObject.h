#ifndef FDOSMPHMYSQLDBOBJECT_H
#define FDOSMPHMYSQLDBOBJECT_H        1
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

#include <Sm/Ph/DbObject.h>
#include <Sm/Ph/Rd/ColumnReader.h>
#include <Rdbms/Override/MySQL/MySqlOvStorageEngineType.h>

// MySql Provider implementation of a Database object.
class FdoSmPhMySqlDbObject : virtual public FdoSmPhDbObject
{
public:
    // Get the database-specific qualified name for this database object.
    virtual FdoStringP GetDbQName() const;

    // Getters/setters for override info:
    void SetAutoIncrementColumnName(FdoString* value) { mAutoIncrementColumnName = value; };
    void SetAutoIncrementColumnSeed(FdoInt64 value) { mAutoIncrementColumnSeed = value; };
    void SetStorageEngine(MySQLOvStorageEngineType value) { mStorageEngine = value; };
    void SetDataDirectory(FdoString* value) { mDataDirectory = value; };
    void SetIndexDirectory(FdoString* value) { mIndexDirectory = value; };
	FdoString* GetAutoIncrementColumnName() const { return mAutoIncrementColumnName; };
	FdoInt64 GetAutoIncrementColumnSeed() const { return mAutoIncrementColumnSeed; };
    MySQLOvStorageEngineType GetStorageEngine() const { return mStorageEngine; };
    FdoString* GetDataDirectory() const { return mDataDirectory; };
    FdoString* GetIndexDirectory() const { return mIndexDirectory; };

    // Convert storage engine between string and enumerated type:
    static FdoString* StorageEngineEnumToString(MySQLOvStorageEngineType storageEngine);
    static MySQLOvStorageEngineType StorageEngineStringToEnum(FdoString* storageEngine);


protected:
    // Constructs an instance of a database object.
    //
    // Parameters:
    //      name: object name
    //      pOwner: the owning physical schema.
    FdoSmPhMySqlDbObject(
        FdoStringP name,
        const FdoSmPhOwner* pOwner,
        FdoSmPhRdDbObjectReader* reader = NULL
    );
    FdoSmPhMySqlDbObject() {}

    virtual ~FdoSmPhMySqlDbObject(void);

    // MySQL does not allow "create index" or "drop view" statements to reference
    // the index by qualified name. This function handles switching the current schema
    // to the index owner before executing the statement, and switching back to the 
    // original current schema after.
    void ActivateOwnerAndExecute( FdoStringP sqlStmt );
    
    // Various column creator implementations
    virtual FdoSmPhColumnP NewColumnBLOB(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnChar(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
        int length,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnDate(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnDecimal(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnSingle(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnDouble(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnGeom(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhScInfoP AssociatedSCInfo,
        bool bNullable = true,
        bool bHasElevation = true,
        bool bHasMeasure = false,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnBool(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnByte(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnInt16(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
		bool bIsAutoincremented,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnInt32(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
		bool bIsAutoincremented,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnInt64(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
		bool bIsAutoincremented,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnUnknown(
        FdoStringP columnName,
        FdoStringP TypeName,
        FdoSchemaElementState elementState,
        bool bNullable,
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    virtual FdoSmPhColumnP NewColumnDbObject(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        bool bNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    );

    // Column reader creator implementation.
    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader();

private:
	FdoStringP mAutoIncrementColumnName;
	FdoInt64 mAutoIncrementColumnSeed;
    MySQLOvStorageEngineType mStorageEngine;
    FdoStringP mDataDirectory;
    FdoStringP mIndexDirectory;
};

typedef FdoPtr<FdoSmPhMySqlDbObject> FdoSmPhMySqlDbObjectP;

#endif
