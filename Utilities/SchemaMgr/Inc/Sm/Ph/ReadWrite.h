#ifndef FDOSMPHREADWRITE_H
#define FDOSMPHREADWRITE_H		1
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

#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/RowCollection.h>
#include <Sm/Ph/../Disposable.h>

// This class is used to write data to the MetaSchema tables.
//
// To Add a row:
//      - call the various setter functions to set the fields for the row.
//          - if the setter for a field is not called, the field's default
//            value is used.
//      - call Add() to add the row.
//
// To modify a row
//      - call the various setter functions. to set the fields for the row.
//      - call Modify() to modify the row, only fields whose setters have
//        been called are modified
//
// To delete a row
//      - call Delete().


class FdoSmPhReadWrite : public FdoSmDisposable
{
public:
    /// Create an empty reader-writer
    FdoSmPhReadWrite();

    FdoSmPhReadWrite(FdoPtr<FdoSmPhReadWrite> subReadWrite);
    /// Create the reader-writer.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the database to query.
    /// 	row: row and field definitions.
	FdoSmPhReadWrite(FdoSmPhMgrP mgr, FdoSmPhRowsP rows );
	
	~FdoSmPhReadWrite(void);

    /// Sets all fields back to their default values.
    void Clear();

    /// Get a string column value by name.
    /// Value for current row is returned.
    /// Returns L"" if value is null. 
	virtual FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

    /// Get an integer column value by name.
    /// Value for current row is returned.
    /// Returns 0 if value is null. 
	virtual int GetInteger( FdoStringP tableName, FdoStringP fieldName );
	
    /// Get an integer column value by name.
    /// Value for current row is returned.
    /// Returns 0 if value is null. 
	virtual long GetLong( FdoStringP tableName, FdoStringP fieldName );
	
    /// Get an 64bit integer column value by name.
    /// Value for current row is returned.
    /// Returns 0 if value is null. 
	virtual FdoInt64 GetInt64( FdoStringP tableName, FdoStringP fieldName );
	
    /// Get an double precision column value by name.
    /// Value for current row is returned.
    /// Returns 0.0 if value is null. 
	virtual double GetDouble( FdoStringP tableName, FdoStringP fieldName );
	
    /// Get a boolean column value by name.
    /// Value for current row is returned.
    /// Returns false if value is null. 
	virtual bool GetBoolean( FdoStringP tableName, FdoStringP fieldName );

    /// Get the column's field definition.
    virtual FdoSmPhFieldP GetField( FdoStringP tableName, FdoStringP fieldName );

    FdoSmPhMgrP GetManager();

    FdoSmPhRowsP GetRows()
    {
        return mSubReadWrite ? mSubReadWrite->GetRows() : mRows;
    }

protected:
    /// Set a string column value by name.
	void SetString( FdoStringP tableName, FdoStringP fieldName, FdoStringP sValue );

    /// Set a integer column value by name.
	void SetInteger( FdoStringP tableName, FdoStringP fieldName, int iValue );
	
    /// Set a integer column value by name.
	void SetLong( FdoStringP tableName, FdoStringP fieldName, long lValue );
	
    /// Set a 64bit integer column value by name.
	void SetInt64( FdoStringP tableName, FdoStringP fieldName, FdoInt64 i64Value );
	
    /// Set a double precision column value by name.
	void SetDouble( FdoStringP tableName, FdoStringP fieldName, double dValue );
	
    /// Set a boolean column value by name.
	void SetBoolean( FdoStringP tableName, FdoStringP fieldName, bool bValue );

    // Sets the row collection for this reader.
    void SetRows( FdoSmPhRowsP rows );

    /// Setter:
    void SetSubReaderWriter(FdoPtr<FdoSmPhReadWrite> subReaderWriter);

private:
	FdoSmPhMgr*                 mMgr;
    FdoSmPhRowsP                mRows;
    FdoPtr<FdoSmPhReadWrite>    mSubReadWrite;
};

typedef FdoPtr<FdoSmPhReadWrite> FdoSmPhReadWriteP;

#endif


