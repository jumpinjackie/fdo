#ifndef FDOSMPHDBOBJECT_H
#define FDOSMPHDBOBJECT_H		1
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

#include <Sm/Ph/DbElement.h>
#include <Sm/Ph/DbObjType.h>
#include <Sm/Ph/ColumnCollection.h>
#include <Sm/Ph/ColumnBLOB.h>
#include <Sm/Ph/ColumnChar.h>
#include <Sm/Ph/ColumnDate.h>
#include <Sm/Ph/ColumnDecimal.h>
#include <Sm/Ph/ColumnSingle.h>
#include <Sm/Ph/ColumnDouble.h>
#include <Sm/Ph/ColumnGeom.h>
#include <Sm/Ph/ColumnBool.h>
#include <Sm/Ph/ColumnByte.h>
#include <Sm/Ph/ColumnInt16.h>
#include <Sm/Ph/ColumnInt32.h>
#include <Sm/Ph/ColumnInt64.h>
#include <Sm/Ph/ColumnUnknown.h>

// some constants

enum FdoLtLockModeType	{
	NoLtLock,
	FdoMode,
	OWMMode
};

class FdoSmPhOwner;
class FdoSmPhRdColumnReader;
class FdoSmPhDependencyCollection;

// This represents a database object (table, view, etc.)
class FdoSmPhDbObject : public FdoSmPhDbElement
{
public:
    /// Returns all the columns in this database object.
    const FdoSmPhColumnCollection* RefColumns() const;
    FdoSmPhColumnsP GetColumns();

    /// Returns all the attribute dependencies (from F_AttributeDependencies)
    /// where this database object is the primary key "table".	
    const FdoSmPhDependencyCollection* GetDependenciesDown() const;

    /// Returns all the attribute dependencies (from F_AttributeDependencies)
    /// where this databaseobject is the foreign key "table".	
    const FdoSmPhDependencyCollection* GetDependenciesUp() const;

    /// Returns the name of the containing database.
    FdoStringP GetDatabase() const
    {
        return GetParent()->GetParent()->GetName();
    }

    /// Returns the name of the containing owner (physical schema).
    FdoStringP GetOwner() const
    {
        return GetParent()->GetName();
    }

    /// Get the database-specific qualified name for this database object.
    virtual FdoStringP GetDbQName() const = 0;

    /// Get the name for referencing this database object in DDL statements.
    /// By default, this function returns the same thing as GetDbQName.
    virtual FdoStringP GetDDLQName() const;

    /// Returns true this database object has data.
	virtual bool GetHasData();

    /// Get the current long transaction mode
    virtual FdoLtLockModeType GetLtMode() const;

    /// Get the current locking mode
    virtual FdoLtLockModeType GetLockingMode() const;

    /// Returns an array of all types of locking supported on this Database Object.
    /// size is set to the size of the array.
    /// Returns NULL when size is 0.
	virtual const FdoLockType* GetLockTypes(FdoInt32& size) const;

    /// The following create various types of columns. See the constructor
    /// declarations of these column types for parameter descriptions.
    //
    /// There is one extra parameter (bAttach). If true, the column is also
    /// added to this database object.
    virtual FdoSmPhColumnP CreateColumnBLOB(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        bool bAttach = true
    );

    virtual FdoSmPhColumnP CreateColumnChar(
		FdoStringP columnName, 
		bool bNullable, 
        int length,
        FdoStringP rootColumnName = L"",
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnDate(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        bool bAttach = true
    );

    virtual FdoSmPhColumnP CreateColumnDecimal(
		FdoStringP columnName, 
		bool bNullable, 
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnSingle(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        bool bAttach = true
    );

    virtual FdoSmPhColumnP CreateColumnDouble(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        bool bAttach = true
    );

    virtual FdoSmPhColumnP CreateColumnGeom(
		FdoStringP columnName,
        FdoSmPhScInfoP AssociatedSCInfo,      
        bool bNullable = true,
        bool bHasElevation = true, 
        bool bHasMeasure = false, 
        FdoStringP rootColumnName = L"",
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnBool(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnByte(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnInt16(
		FdoStringP columnName, 
		bool bNullable, 
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnInt32(
		FdoStringP columnName, 
		bool bNullable, 
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnInt64(
		FdoStringP columnName, 
		bool bNullable, 
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnUnknown(
		FdoStringP columnName, 
        FdoStringP typeName,
		bool bNullable, 
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnDbObject(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        bool bAttach = true
	);

    /// Get list of columns as references into a string collection
    virtual FdoStringsP GetRefColsSql();

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    virtual FdoStringP XMLSerializeProviderAtts() const;

protected:
    /// unused constructor needed only to build on Linux
    FdoSmPhDbObject();

    /// Constructs an instance of a database object.
    /// 
    /// Parameters:
    /// 	name: object name
    /// 	pOwner: the owning physical schema.
    /// 	elementState: Indicates whether the database object is new or 
    ///          already exists.
    FdoSmPhDbObject(
        FdoStringP name, 
        const FdoSmPhOwner* pOwner,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

	virtual ~FdoSmPhDbObject(void);

    virtual void SetLtMode( FdoLtLockModeType mode );

    virtual void SetLockingMode( FdoLtLockModeType mode );

    /// Loads column list for existing database object
    void LoadColumns();
    
    /// Create a column from a column reader and add it to this database object
    virtual FdoSmPhColumnP NewColumn(
        FdoPtr<FdoSmPhRdColumnReader> colRdr
    );

    /// Various functions that each provider must override to customize the 
    /// creation of various types of columns.
    virtual FdoSmPhColumnP NewColumnBLOB(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    ) = 0;

    virtual FdoSmPhColumnP NewColumnChar(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        int length,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnDate(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    ) = 0;

    virtual FdoSmPhColumnP NewColumnDecimal(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnSingle(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    ) = 0;

    virtual FdoSmPhColumnP NewColumnDouble(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
    ) = 0;

    virtual FdoSmPhColumnP NewColumnGeom(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
        FdoSmPhScInfoP AssociatedSCInfo,        
        bool bNullable = true, 
        bool bHasElevation = true, 
        bool bHasMeasure = false, 
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnBool(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnByte(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnInt16(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnInt32(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnInt64(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable,
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnUnknown(
		FdoStringP columnName, 
        FdoStringP typeName,
		FdoSchemaElementState elementState,
		bool bNullable, 
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnDbObject(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    /// Create a reader to get the columns for this database object.
    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader() = 0;

    /// Records this database object as having been updated so that modification 
    /// can be undone on rollback.
    virtual void UpdRollbackCache();

    /// Remove from parent Owner's collection.
    virtual void Discard();

private:
    void LoadDependencies();
/* TODO:
    virtual void Finalize();

	void AddReferenceLoopError(void);
	void AddDeleteNotEmptyError(void);
	void AddPkeyColumnError(FdoStringP columnName);
	void AddNotNullColError(FdoStringP columnName);
    void AddColumnNoexistError( FdoStringP indexName, FdoStringP columnName  );
*/

    /// Column list for this database object.
	FdoSmPhColumnsP mColumns;

	FdoPtr<FdoSmPhDependencyCollection> mDependenciesDown;
	FdoPtr<FdoSmPhDependencyCollection> mDependenciesUp;

    FdoLtLockModeType   mLtMode;
    FdoLtLockModeType   mLockingMode;

    /// Supported Lock types.
	FdoInt32 mLockTypeCount;
	FdoLockType* mLockTypes;
};

typedef FdoPtr<FdoSmPhDbObject> FdoSmPhDbObjectP;

#endif


