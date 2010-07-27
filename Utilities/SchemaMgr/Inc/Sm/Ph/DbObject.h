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
#include <Sm/Ph/BaseObjectCollection.h>
#include <Sm/Ph/FkeyCollection.h>
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

class FdoSmPhIndex;
class FdoSmPhIndexCollection;
class FdoSmPhReader;
class FdoSmPhTableColumnReader;
class FdoSmPhRdIndexReader;
class FdoSmPhRdBaseObjectReader;
class FdoSmPhRdPkeyReader;
class FdoSmPhRdFkeyReader;
class FdoSmPhTableIndexReader;
class FdoSmPhTableComponentReader;
class FdoSmPhTableDependencyReader;

// some constants

enum FdoLtLockModeType	{
	NoLtLock,
	FdoMode,
	OWMMode
};

class FdoSmPhOwner;
class FdoSmPhRdColumnReader;
class FdoSmPhDependencyCollection;
class FdoSmPhDependencyReader;

// This represents a database object (table, view, etc.)
class FdoSmPhDbObject : public FdoSmPhDbElement
{
public:
    /// Returns all the columns in this database object.
    const FdoSmPhColumnCollection* RefColumns() const;
    virtual FdoSmPhColumnsP GetColumns();

    /// Returns all the primary key columns in this database object.
    const FdoSmPhColumnCollection* RefPkeyColumns() const;
    virtual FdoSmPhColumnsP GetPkeyColumns();

    /// Returns all the indexes in this database object.
    const FdoSmPhIndexCollection* RefIndexes() const;
    virtual FdoPtr<FdoSmPhIndexCollection> GetIndexes();

    /// Returns all foreign keys for which this database object is the foreign "table"
    const FdoSmPhFkeyCollection* RefFkeysUp() const;
    virtual FdoSmPhFkeysP GetFkeysUp();

    /// Returns all the attribute dependencies (from F_AttributeDependencies)
    /// where this database object is the primary key "table".	
    const FdoSmPhDependencyCollection* GetDependenciesDown() const;

    /// Returns all the attribute dependencies (from F_AttributeDependencies)
    /// where this databaseobject is the foreign key "table".	
    const FdoSmPhDependencyCollection* GetDependenciesUp() const;

    /// Returns the database object's best identity columns.
    /// These are the primary key columns if the database object has a primary key.
    /// If the object has no primary key but it has a root object then the best identity
    /// for the root object (whose columns are all present in this object) is returned. 
    /// Typically, a view may have a table or another view as its root object.
    /// NULL is returned if the database object has no identity and none can be derived
    /// from its root object. 
    FdoSmPhColumnsP GetBestIdentity();
    
    /// Supporting function for previous GetBestIdentity() function. 
    /// Returns the database object's best identity columns where all columns are in the
    /// given database object. The given database object is usually one that has this 
    /// object as its base. 
    /// If there is no best identity on this database object, its root object is not checked.
    /// NULL is returned if the database object has no identity suitable for the given object.
    virtual FdoSmPhColumnsP GetBestIdentity( FdoPtr<FdoSmPhDbObject> dbObject );

    /// Returns the name of this database object's primary key
	FdoStringP GetPkeyName() const;

    /// Get the root database object. A root object is a base object for which identity 
    /// can be derived. If this object has exactly one base object then the 
    /// base object is returned as the root object. If this object has multiple base objects then
    /// the root object is considered ambiguous and NULL is returned. The reason for this is 
    /// that if we can't derive the best identity from the base objects when there are 
    /// more than one of them. 
    const FdoSmPhDbObject* RefRootObject() const;
    FdoPtr<FdoSmPhDbObject> GetRootObject();

    /// Get the lowest root object (The one at the top of the root object chain).
    virtual FdoPtr<FdoSmPhDbObject> GetLowestRootObject();

    /// Get the root object name. Returns L"" if this object has no root object.
    FdoStringP GetRootObjectName() const;

    /// Get the root object owner name. Returns L"" if this object has no root object.
    FdoStringP GetRootOwner() const;

    /// Get the root object's database name. Returns L"" if this object has no root object.
    FdoStringP GetRootDatabase() const;

    /// Returns the name of the containing database.
    FdoStringP GetDatabase() const
    {
        return GetParent()->GetParent()->GetName();
    }

    /// Returns all base objects for this object. A base object is a object from which
    /// this object is derived (e.g. a view can be based on one or more tables or other views)
    const FdoSmPhBaseObjectCollection* RefBaseObjects() const;
    FdoSmPhBaseObjectsP GetBaseObjects();

    /// Returns the name of the containing owner (physical schema).
    FdoStringP GetOwner() const
    {
        return GetParent()->GetName();
    }

    /// Get the database-specific qualified name for this database object.
    virtual FdoStringP GetDbQName() const = 0;

    // Get unqualified name for use in DDL statements
    virtual FdoStringP GetDDLName() const;

    /// Get the name for referencing this database object in DDL statements.
    /// By default, this function returns the same thing as GetDbQName.
    virtual FdoStringP GetDDLQName() const;

    // Reverse-engineers an FDO feature schema name from this object.
    // Default implementation returns best schema name for owning datastore.
    virtual FdoStringP GetBestSchemaName() const;

    // Reverse-engineers an FDO class name from this object.
    // Default implementation returns this object's name with 
    // characters, not allowed in class names (':','.') converted
    // to '_'.
    virtual FdoStringP GetBestClassName() const;

    virtual FdoStringP GetBestClassName(FdoStringP schemaName) const;

    /// Returns true this database object has data.
	virtual bool GetHasData();

    /// Get the indicator on whether or not the class supports write.
    virtual bool GetSupportsWrite() const;

    /// Get the current long transaction mode
    virtual FdoLtLockModeType GetLtMode() const;

    /// Get the current locking mode
    virtual FdoLtLockModeType GetLockingMode() const;

    /// Returns an array of all types of locking supported on this Database Object.
    /// size is set to the size of the array.
    /// Returns NULL when size is 0.
	virtual const FdoLockType* GetLockTypes(FdoInt32& size) const;

    /// Get the current polygon vertex order rule. Different geometry type may have 
    /// different polygon vertex order rule.
    virtual FdoPolygonVertexOrderRule GetPolygonVertexOrderRule(FdoString* propName) const;

    /// Get the current polygon vertex order strictness rule. Different geometry type
    /// may have different rule.
    virtual FdoBoolean GetPolygonVertexOrderStrictness(FdoString* propName) const;

    // Returns true if the given unique key has the same columns as the primary key.
    FdoBoolean IsUkeyPkey( FdoSmPhColumnsP ukeyColumns );

    // Returns true if this database object has all of the given columns. The columns
    // must match by both name and definition.
    FdoBoolean HasColumns( FdoSmPhColumnsP columns );

    /// Removes the given index from the cache without
    /// deleting it from the datastore.
    void DiscardIndex( FdoSmPhIndex* index );

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
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnDate(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        bool bAttach = true
    );

    virtual FdoSmPhColumnP CreateColumnDecimal(
		FdoStringP columnName, 
		bool bNullable, 
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnSingle(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        bool bAttach = true
    );

    virtual FdoSmPhColumnP CreateColumnDouble(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
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
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnByte(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnInt16(
		FdoStringP columnName, 
		bool bNullable, 
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnInt32(
		FdoStringP columnName, 
		bool bNullable, 
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        bool bAttach = true
	);

    virtual FdoSmPhColumnP CreateColumnInt64(
		FdoStringP columnName, 
		bool bNullable, 
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
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

    /// Add a foreign key to this database object
    FdoSmPhFkeyP CreateFkey(
        FdoStringP name, 
        FdoStringP pkeyTableName,
        FdoStringP pkeyTableOwner = L""
    );

    /// Commit modifications
    /// Ensures that root object is commit before this object.
    virtual void Commit( bool fromParent = false, bool isBeforeParent = false );

    // Executes a DDL statement that affects this database object (e.g. add/delete an index
    // or foreign key).
    // The default implementation simply executes the statement. However, providers
    // that need to do special things can override this function.
    // For example, the Oracle provider must execute these statements within a 
    // DDL transaction when this is an OWM-enabled table.
    virtual void ExecuteDDL( 
        FdoStringP sqlStmt, // the DDL statement
        FdoSmPhDbObject* refTable = NULL, // referenced table for foreign key.
                                          // must be specified if the statement operates
                                          // on a foreign key.
        bool isDDL = true                 // true if the statement must be executed
                                          // as a DDL statement. 
    );

    /// Get list of columns as references into a string collection
    virtual FdoStringsP GetRefColsSql();

    // Generate key column SQL clauses for the given columns.
    virtual FdoStringsP GetKeyColsSql( FdoSmPhColumnCollection* columns );

    /// Get SQL for creating primary key on new database object
    FdoStringP GetAddPkeySql();

    /// Sets the primary key name for this database object.
    /// Can only be called for new database objects.
    virtual void SetPkeyName( FdoStringP pkeyName );

    /// Add a column to the primary key.
	void AddPkeyCol(FdoStringP columnName );

    // Load this object's columns from the given reader
    virtual void CacheColumns( FdoPtr<FdoSmPhRdColumnReader> rdr );

    // Load this database object's indexes from the given reader
    virtual bool CacheIndexes( FdoPtr<FdoSmPhRdIndexReader> rdr );

    // Returns true if this database object's indexes have been cached.
    virtual bool IndexesLoaded();

    // Load this object's columns from the given reader
    virtual void CacheBaseObjects( FdoPtr<FdoSmPhRdBaseObjectReader> rdr );

    // Load this object's primary key from the given reader
    virtual void CachePkeys( FdoPtr<FdoSmPhRdPkeyReader> rdr );

    // Load this objects's foreign keys from the given reader
    virtual void CacheFkeys( FdoPtr<FdoSmPhRdFkeyReader> rdr );

    // Load this object's "up" dependencies from the given reader.
    // "Up" dependencies are the ones where this object is the FK table.
    virtual void CacheDependenciesUp( FdoPtr<FdoSmPhDependencyReader> rdr );

    /// Commit modifications to foreign keys.
    virtual void CommitFkeys( bool isBeforeParent );

    /// Drops this database object whether or not it has data.
    void ForceDelete();

    // Checks each foreign key for this DbObject and adds the referenced (primary) DbObject
    // to the cache candidates list for its owner. This allows more efficient fetch of 
    // these referenced DbObjects when they are not yet cached.
    virtual void LoadFkeyRefCands();

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    virtual FdoStringP XMLSerializeProviderAtts() const;

    // Returns true if the object is a table or view. Sub-classes can override this function
    // to allow RDBMS-specific types to be classified.
    // The classifyDefaultTypes parameter has no effect in this function. However, specific 
    // RDBMS's might have some types that can be classified but are not classified by default.
    // When classifyDefaultTypes is true then this function must return true only for types
    // that can be classified by default. 
    // 
    virtual bool ClassifyObjectType(FdoBoolean classifyDefaultTypes );

    // Returns the name of the database object if it corresponds to a class is in the given Feature Schema.
    // No checking is done against the object's type. This is performed by ClassifyObjectType().
    virtual FdoStringP GetClassifiedObjectName( FdoStringP schemaName );

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

    // Retrieve current base object list without going to the RDBMS if not already cached.
    // Returns null pointer if not cached.
    FdoSmPhBaseObjectsP Get_BaseObjects();

    // Remove all cached base objects.
    void DiscardBaseObjects();

    // Utility function for generating SQL clauses for referencing given columns.
    FdoStringsP _getRefColsSql( FdoSmPhColumnCollection* columns );

    virtual void SetLtMode( FdoLtLockModeType mode );

    virtual void SetLockingMode( FdoLtLockModeType mode );

    // Sets the root object for this database object.
    // Pass in NULL to remove current root object relationship.
    // Discards any previously set base objects.
    void SetRootObject( FdoPtr<FdoSmPhDbObject> rootObject );

    /// Loads column list for existing database object
    void LoadColumns();
    void LoadColumns( FdoPtr<FdoSmPhTableColumnReader> colRdr );

    /// Loads objects that this object is based on, if not yet loaded. 
    void LoadBaseObjects();
    virtual void LoadBaseObjects( FdoPtr<FdoSmPhTableComponentReader> baseObjRdr, bool isSkipAdd = false );

    /// Load Primary Key if not yet loaded
    void LoadPkeys();
    void LoadPkeys( FdoPtr<FdoSmPhReader> pkeyRdr, bool isSkipAdd = false );
    
    // Load a primary key column from a reader.
    virtual void LoadPkeyColumn( FdoPtr<FdoSmPhReader> pkeyRdr, FdoSmPhColumnsP pkeyColumns );

    /// Load Indexes if not yet loaded
    bool LoadIndexes();
    bool LoadIndexes( FdoPtr<FdoSmPhTableIndexReader> indexRdr, bool isSkipAdd );
	
    // Load a column from a reader into an index.
    virtual void LoadIndexColumn( FdoPtr<FdoSmPhTableIndexReader> indexRdr, FdoPtr<FdoSmPhIndex> index );

    /// Load Foreign Keys if not yet loaded
    void LoadFkeys();
    void LoadFkeys( FdoPtr<FdoSmPhReader> fkeyRdr, bool isSkipAdd );
    // Load a foreign key column from a reader.
    virtual void LoadFkeyColumn( FdoPtr<FdoSmPhReader> fkeyRdr, FdoSmPhFkeyP fkey );


    // Create new base object group reader
    virtual FdoPtr<FdoSmPhTableComponentReader> NewTableBaseReader( FdoPtr<FdoSmPhRdBaseObjectReader> rdr );

    /// Create a column from a column reader and add it to this database object
    virtual FdoSmPhColumnP NewColumn(
        FdoPtr<FdoSmPhRdColumnReader> colRdr
    );

    /// Add an index from an index reader
    virtual FdoPtr<FdoSmPhIndex> CreateIndex(
        FdoPtr<FdoSmPhTableIndexReader> rdr
    );

    /// Create a base object reference from a base object reader
    virtual FdoSmPhBaseObjectP NewBaseObject(
        FdoPtr<FdoSmPhTableComponentReader> baseObjRdr
    );

    // Create a base object reference specified by qualified name
    virtual FdoSmPhBaseObjectP NewBaseObject(
        FdoStringP name,
        FdoStringP ownerName,
        FdoStringP databaseName
    );

    // Create a base object reference to the given database object.
    virtual FdoSmPhBaseObjectP NewBaseObject(
        FdoPtr<FdoSmPhDbObject> dbObject
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
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnDate(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colRdr = NULL
    ) = 0;

    virtual FdoSmPhColumnP NewColumnDecimal(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnSingle(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colRdr = NULL
    ) = 0;

    virtual FdoSmPhColumnP NewColumnDouble(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
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
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnByte(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnInt16(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnInt32(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable, 
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colRdr = NULL
	) = 0;

    virtual FdoSmPhColumnP NewColumnInt64(
		FdoStringP columnName, 
		FdoSchemaElementState elementState,
		bool bNullable,
		bool bIsAutoincremented = false,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
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

    /// Foreign key object creator
    virtual FdoSmPhFkeyP NewFkey(
        FdoStringP name, 
        FdoStringP pkeyTableName,
        FdoStringP pkeyTableOwner,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    /// Readers for Columns, Base Objects, Primary Key, and Foreign Keys
    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader() = 0;
    virtual FdoPtr<FdoSmPhRdBaseObjectReader> CreateBaseObjectReader() const;
    virtual FdoPtr<FdoSmPhRdPkeyReader> CreatePkeyReader() const;
    virtual FdoPtr<FdoSmPhRdFkeyReader> CreateFkeyReader() const;
    virtual FdoPtr<FdoSmPhRdIndexReader> CreateIndexReader() const = 0;

    /// Index object creator
    virtual FdoPtr<FdoSmPhIndex> NewIndex(
        FdoStringP name, 
        bool isUnique,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    ) = 0;

    /// Spatial Index object creator
    virtual FdoPtr<FdoSmPhIndex> NewSpatialIndex(
        FdoStringP name, 
        bool isUnique,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    ) = 0;


    /// Autogenerate a unique primary key name for this database object.
	virtual FdoStringP GenPkeyName();

    /// Records this database object as having been updated so that modification 
    /// can be undone on rollback.
    virtual void UpdRollbackCache();

    /// Remove from parent Owner's collection.
    virtual void Discard();

    virtual void AddPkeyColumnError(FdoStringP columnName);

    virtual void AddFkeyColumnCountError(FdoStringP fkeyName);

    virtual void AddIndexColumnError(FdoStringP columnName);

    FdoSmPhColumnsP mPkeyColumns;

private:
    void LoadDependencies( bool up );
    void LoadDependenciesUp( FdoPtr<FdoSmPhTableDependencyReader> depRdr );

    // Create new foreign key group reader
    virtual FdoPtr<FdoSmPhTableComponentReader> NewTableFkeyReader( FdoPtr<FdoSmPhRdFkeyReader> rdr );
    
    // Create new primary key group reader
    virtual FdoPtr<FdoSmPhTableComponentReader> NewTablePkeyReader( FdoPtr<FdoSmPhRdPkeyReader> rdr );

    // Create new index group reader
    virtual FdoPtr<FdoSmPhTableIndexReader> NewTableIndexReader( FdoPtr<FdoSmPhRdIndexReader> rdr );

    // Check for loop when walking up root objects. 
    // Level is incremented each time this function is called. 
    // Caller is responsible for initializing level to 1 before the first call.
    bool CheckRootObjectLoop( FdoInt32& level );

/* TODO:
    virtual void Finalize();

	void AddReferenceLoopError(void);
	void AddDeleteNotEmptyError(void);
	void AddPkeyColumnError(FdoStringP columnName);
	void AddNotNullColError(FdoStringP columnName);
    void AddColumnNoexistError( FdoStringP indexName, FdoStringP columnName  );
*/
    void AddFkeyColumnError(FdoStringP columnName);
    void AddCreateFkeyError(FdoStringP fkeyName);
    
    /// Column list for this database object.
	FdoSmPhColumnsP mColumns;

    // Foreign key list.
    FdoSmPhFkeysP mFkeysUp;

    // index list
    FdoPtr<FdoSmPhIndexCollection> mIndexes;

	FdoPtr<FdoSmPhDependencyCollection> mDependenciesDown;
	FdoPtr<FdoSmPhDependencyCollection> mDependenciesUp;

    FdoLtLockModeType   mLtMode;
    FdoLtLockModeType   mLockingMode;

    /// Supported Lock types.
	FdoInt32 mLockTypeCount;
	FdoLockType* mLockTypes;

    FdoStringP mPkeyName;

    FdoPtr<FdoSmPhBaseObjectCollection> mBaseObjects;
};

typedef FdoPtr<FdoSmPhDbObject> FdoSmPhDbObjectP;

#endif



