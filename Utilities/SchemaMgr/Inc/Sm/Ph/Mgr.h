#ifndef FDOSMPHMGR_H
#define FDOSMPHMGR_H		1
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

#ifdef _WIN32
#pragma once
#endif

class FdoSmPhMgr;
typedef FdoPtr<FdoSmPhMgr> FdoSmPhMgrP;

#include <Sm/Disposable.h>
#include <Sm/Ph/DatabaseCollection.h>
#include <Sm/Ph/Rb/Cache.h>

// Metaschema tables
static const FdoStringP FDO_OPTIONS_TABLE = L"f_options";
static const FdoStringP FDO_DBOPEN_TABLE = L"f_dbopen";


// Metaschema columns

// Misc constants
static const FdoStringP LT_MODE_OPTION	= L"LT_MODE";
static const FdoStringP LCK_MODE_OPTION = L"LOCKING_MODE";
static const FdoStringP FDO_SESSION_COLUMN = L"sessionid";
static const FdoStringP FDO_PROCESS_COLUMN = L"process";
static const FdoStringP FDO_USER_NUM_COLUMN = L"usernum";
static const FdoStringP FDO_DBUSER_COLUMN = L"dbuser";
static const FdoStringP FDO_ACCESS_MODE_COLUMN = L"accessmode";
static const FdoStringP FDO_OPENDATE_COLUMN = L"opendate";

static const int	PROCESS_COLUMN_LENGTH = 12;	


// --------------------
// Lock Table Constants
// --------------------

static const FdoStringP LOCK_CORE_TABLE = L"f_lock";
static const FdoStringP LOCK_USED_TABLE = L"f_lockid_in_table";


// ---------------------------
// Lock Table Column Constants
// ---------------------------

//   -> Table F_LOCK:
static const FdoStringP LOCK_CORE_CREATEDATE    = L"createdate";
static const FdoStringP LOCK_CORE_DESCRIPTION   = L"description";
static const FdoStringP LOCK_CORE_LOCKID        = L"lockid";
static const FdoStringP LOCK_CORE_USERNAME      = L"username";

//   -> Table F_LOCKID_IN_TABLE:
static const FdoStringP LOCK_USED_DATASTORENAME = L"datastorename";
static const FdoStringP LOCK_USED_LOCKID        = L"lockid";
static const FdoStringP LOCK_USED_TABLENAME     = L"tablename";


// --------------------------------
// Long Transaction Table Constants
// --------------------------------

static const FdoStringP LT_ACTIVE_LT_TABLE  = L"f_activelt";
static const FdoStringP LT_CORE_TABLE       = L"f_lt";
static const FdoStringP LT_DEPENDENCY_TABLE = L"f_ltdependency";
static const FdoStringP LT_USED_TABLE       = L"f_ltid_in_table";


// ----------------------------------------
// Long Transaction Table Column Constants
// ----------------------------------------
//   -> Table F_ACTIVELT:
static const FdoStringP LT_ACTIVE_LT_ACTIVATEDATE = L"activatedate";
static const FdoStringP LT_ACTIVE_LT_ACTIVELTID   = L"activeltid";
static const FdoStringP LT_ACTIVE_LT_SESSIONID    = L"sessionid";
static const FdoStringP LT_ACTIVE_LT_USERNAME     = L"username";

//   -> Table F_LT:
static const FdoStringP LT_CORE_CREATEDATE        = L"createdate";
static const FdoStringP LT_CORE_DESCRIPTION       = L"description";
static const FdoStringP LT_CORE_LOCKID            = L"lockid";
static const FdoStringP LT_CORE_LOCKTYPE          = L"locktype";
static const FdoStringP LT_CORE_LTID              = L"ltid";
static const FdoStringP LT_CORE_LTNAME            = L"ltname";
static const FdoStringP LT_CORE_USERNAME          = L"username";

//   -> Table F_LTDEPENDENCY:
static const FdoStringP LT_DEPENDENCY_CHILDLTID   = L"childltid";
static const FdoStringP LT_DEPENDENCY_PARENTLTID  = L"parentltid";

//   -> Table F_LTID_IN_TABLE:
static const FdoStringP LT_USED_DATASTORENAME     = L"datastorename";
static const FdoStringP LT_USED_LTID              = L"ltid";
static const FdoStringP LT_USED_TABLENAME         = L"tablename";


// -----------------------------------------------------------------
// Long Transaction General Constants (for example: used as aliases)
// -----------------------------------------------------------------

static const FdoStringP LT_INFO_CREATEDATE    = L"LT_CREATEDATE";
static const FdoStringP LT_INFO_DESCRIPTION   = L"LT_DESCRIPTION";
static const FdoStringP LT_INFO_FREEZE_MODE   = L"LT_FREEZE_MODE";
static const FdoStringP LT_INFO_FREEZE_OWNER  = L"LT_FREEZE_OWNER";
static const FdoStringP LT_INFO_FREEZE_WRITER = L"LT_FREEZE_WRITER";
static const FdoStringP LT_INFO_LTID          = L"LT_ID";
static const FdoStringP LT_INFO_LTNAME        = L"LT_NAME";
static const FdoStringP LT_INFO_USERNAME      = L"LT_OWNER";
static const FdoStringP LT_INFO_IS_ACTIVE     = L"LT_IS_ACTIVE";
static const FdoStringP LT_INFO_IS_REFRESHED  = L"LT_IS_REFRESHED";
static const FdoStringP LT_INFO_LOCK_ID       = L"LT_LOCK_ID";
   
class FdoSmPhSchemaReader;
class FdoSmPhClassReader;
class FdoSmPhAssociationReader;
class FdoSmPhOptionsReader;
class FdoSmPhCfgSchemaReader;
class FdoSmPhCfgClassReader;
class FdoSmPhCfgPropertyReader;
class FdoSmPhRdSchemaReader;
class FdoSmPhRdClassReader;
class FdoSmPhRdPropertyReader;
class FdoSmPhSpatialContextReader;
class FdoSmPhSpatialContextGroupReader;
class FdoSmPhSpatialContextGeomReader;
class FdoSmPhRowCollection;
class FdoSmPhRow;
class FdoSmPhRdQueryReader;
class FdoSmPhCommandWriter;
class FdoSmPhClassWriter;
class FdoSmPhPropertyWriter;
class FdoSmPhSchemaWriter;
class FdoSmPhDependencyWriter;
class FdoSmPhAssociationWriter;
class FdoSmPhSADWriter;
class FdoSmPhSpatialContextWriter;
class FdoSmPhRdSpatialContextReader;
class FdoSmPhSpatialContextGroupWriter;
class FdoSmPhSpatialContextGeomWriter;
/* TODO
class FdoSmPhIndexWriter;
class FdoSmPhIndexPropertyWriter;
class FdoSmPhFieldCollection;
class FdoSmPhRdQueryReader;
*/
// Abstract class for providing access to the physical schema of an FDO DataStore
// Each provider must define an implementing class.

class FdoSmPhMgr : public FdoSmDisposable
{
public:

    /// Create a Physical Schema Manager

	~FdoSmPhMgr(void);

    /// Get reader that lists all Feature Schemas
    /// from the given owner.
    /// If owner is NULL then schemas from the current owner are listed.
    virtual FdoPtr<FdoSmPhSchemaReader> CreateSchemaReader( FdoSmPhOwnerP owner = (FdoSmPhOwner*) NULL);

    /// Get reader to retrieve all classes, for the given
    /// feature schema
	virtual FdoPtr<FdoSmPhClassReader> CreateClassReader(FdoStringP schemaName);

    /// Get reader to retrieve all spatial contexts for the connection.
	virtual FdoPtr<FdoSmPhSpatialContextReader> CreateSpatialContextReader();

    /// Get reader to retrieve all spatial context groupss for the connection.
	virtual FdoPtr<FdoSmPhSpatialContextGroupReader> CreateSpatialContextGroupReader();

    /// Get reader to retrieve all spatial context associations to geometric properties for the connection.
	virtual FdoPtr<FdoSmPhSpatialContextGeomReader> CreateSpatialContextGeomReader();

	/// Get reader to retrieve all spatial contexts for the connection (no metaschema).
	virtual FdoPtr<FdoSmPhRdSpatialContextReader> CreateRdSpatialContextReader();

    /// Convert spatial context name to number in RDBMS catalogue.
    /// >= 0 if found, -1 if not found.
    virtual FdoInt64    FindScIdFromName(FdoString * scName);

    /// Get a reader to retrieve the association definition for the given tables.
    /// The query depends on the parameters below.
    /// 
    /// When pkTableName and fkTableName are both L"" then all definitions are
    /// retrieved.
	//
    /// When only fkTableName is L"" then all definitions referenced by 
    /// pkTableName are selected.
	//
    /// When only pkTableName is L"" then all definitions referencing 
    /// fkTableName are selected.
	//
    /// When neither is L"" then the select depends on bAnd:
    /// 	- if true then definitions that match the pkTableName and the
    /// 	fkTableName are selected.
    /// 	- if false then definitions that match the pkTableName or the
    /// 	fkTableName are selected.
	//
    /// Parameters:
    /// 	pkTableName: primary key table name
    /// 	fkTableName: foreign key table name.
    /// 	bAnd: 
    /// 		True: select definitions for the given pkTable and fkTable.
    /// 		False: select definitions for the given pkTable or fkTable.
    /// 	mgr: the current physical schema manager.
	virtual FdoPtr<FdoSmPhAssociationReader> CreateAssociationReader(
        FdoStringP pkTableName, 
        FdoStringP fkTableName, 
        bool bAnd
    );

    /// Get reader to retrieve the contents of the F_OPTIONS MetaSchema table.
    /// Always retrieved from the current owner.
	virtual FdoPtr<FdoSmPhOptionsReader> CreateOptionsReader( FdoStringP owner = L"");

    /// Execute a query and get a reader for its results set
    /// Query is specified by table list (rows) and optional where clause.
    virtual FdoPtr<FdoSmPhRdQueryReader> CreateQueryReader(
        FdoPtr<FdoSmPhRowCollection> rows,      // defines the database objects and fields in the results set
        FdoStringP where,                       // where clause. L"" if no where clause
        FdoPtr<FdoSmPhRow> binds = (FdoSmPhRow*) NULL         // Bind variables, NULL if not a bound query
    ) = 0;

    /// Execute a query and get a reader for its results set
    /// Query is specified by full SQL statement.
    virtual FdoPtr<FdoSmPhRdQueryReader> CreateQueryReader(
        FdoPtr<FdoSmPhRow> fields,         // query's select column list.
        FdoStringP sStatement,      // query statement. Must reference each column in the fields
    /// collection and each field in the binds collection.
        FdoPtr<FdoSmPhRow> binds = (FdoSmPhRow*) NULL    // Fields to bind into query (Query parameters).
    /// If NULL, then this is not a bound query.
    ) = 0;

    /// Create generic writer for writing to a database table.
    virtual FdoPtr<FdoSmPhCommandWriter> CreateCommandWriter( FdoPtr<FdoSmPhRow> row ) = 0;

    /// Create various readers for AutoGenerating schemas based on config document directives.
    virtual FdoPtr<FdoSmPhCfgSchemaReader> CreateCfgSchemaReader( FdoPtr<FdoSmPhRowCollection> rows ) = 0;
    virtual FdoPtr<FdoSmPhCfgClassReader> CreateCfgClassReader( FdoPtr<FdoSmPhRowCollection> rows, FdoStringP schemaName ) = 0;
    virtual FdoPtr<FdoSmPhCfgPropertyReader> CreateCfgPropertyReader( FdoStringP schemaName, FdoStringP className, FdoSmPhDbObjectP dbObject ) = 0;

    /// Create various readers for AutoGenerating schemas directly from the RDBMS.
    /// Create various readers for AutoGenerating schemas directly from the RDBMS.
    virtual FdoPtr<FdoSmPhRdSchemaReader> CreateRdSchemaReader( 
        FdoPtr<FdoSmPhRowCollection> rows, 
        FdoPtr<FdoSmPhOwner> owner, 
        bool dsInfo 
    );
    virtual FdoPtr<FdoSmPhRdClassReader> CreateRdClassReader( 
        FdoPtr<FdoSmPhRowCollection> rows, 
        FdoStringP schemaName,
        FdoBoolean keyedOnly = true,// If true, skip tables without key.
        FdoStringP database = L"",  // Database where RDBMS schema resides (current connection by default)
        FdoStringP owner = L""      // the RDBMS schema (defaults to current schema)
    );
    virtual FdoPtr<FdoSmPhRdPropertyReader> CreateRdPropertyReader( FdoSmPhDbObjectP dbObject );

    /// The following functions retrieve a database from the current connection.
    //
    /// Parameters:
    ///  database: the name of the database to retrieve. If L"" then the default 
    ///      database is retrieved.

    /// Get read-only pointer to database, NULL if database not found
    const FdoSmPhDatabase* RefDatabase(FdoStringP database = L"") const;

    /// Get read-write pointer to database, NULL if database not found
    FdoSmPhDatabaseP FindDatabase(FdoStringP database = L"", bool caseSensitive = true);
    /// Get read-write pointer to database, Throws exception if database not found
    FdoSmPhDatabaseP GetDatabase(FdoStringP database = L"");


    /// Get read-write pointer to an owner, NULL if owner not found
    FdoSmPhOwnerP FindOwner(FdoStringP ownerName = L"", FdoStringP database = L"", bool caseSensitive = true);

    /// Get read-write pointer to an owner, exception if owner not found
    FdoSmPhOwnerP GetOwner(FdoStringP ownerName = L"", FdoStringP database = L"", bool caseSensitive = true);

    /// Get a database object from the current connection.
    //
    /// Parameters:
    ///  database: the name of the database object to retrieve. 
    ///  database: the name of the owning schema where database object resides. L"" for the 
    ///      default schema. 
    ///  database: the name of the database where owner resides. L"" for the default 
    ///      database .

    /// Get read-only pointer to database object, NULL if database object not found
	const FdoSmPhDbObject* RefDbObject(FdoStringP objectName, FdoStringP owner = L"", FdoStringP database = L"") const;

    /// Get read-write pointer to database, NULL if database object not found
	FdoSmPhDbObjectP FindDbObject(FdoStringP objectName, FdoStringP owner = L"", FdoStringP database = L"", bool caseSensitive = true);

    FdoStringP GetDefaultOwnerName()
    {
        return mDefaultOwnerName;
    }

    /// Set the config document information.
    virtual void SetConfiguration( 
        FdoStringP providerName,
        FdoIoStreamP configDoc,
        FdoFeatureSchemasP configSchemas,
        FdoSchemaMappingsP configMappings 
    );

    /// Get current provider name.
    /// This is only set when a config document is in effect
    FdoStringP GetProviderName();

    FdoIoStreamP GetConfigDoc();

    /// Get the config document feature schemas 
    FdoFeatureSchemasP GetConfigSchemas();

    /// Get the config document schema overrides
    FdoSchemaMappingsP GetConfigMappings();

    /// Get the corresponding qualified class name for the given database object.
    /// Returns L"" if the database object has not yet been classified
    FdoStringP GetDbObjectClassification( FdoStringP dbObjectName );

    /// Set the classification for a database object.
    /// This function is called when a class is autogenerated for the database object.
    void SetDbObjectClassification( 
        FdoStringP dbObjectName, 
        FdoStringP className        // Qualified class name
    );

/* TODO
    /// Returns true if the given table name is a valid name.
	bool IsObjectNameValid( FdoStringP objectName );

*/

    /* Functions for handling the rollback cache. The rollback cache lists all
       tables and columns that have been modified in this session. The rollback
       cache can be used to determine which table and column changes need to be rolled
       back when a transaction rollback is performed. 
      
       The rollback cache stores table and column names, but no information on the 
       modification that was performed.
    */

    /// Add a table to the rollback cache for the current connection.
    //
    /// Parameters:
    ///      name - the table name.
    ///      elementState - whether the table was created or deleted.
	void AddRollbackTable(FdoString* name, FdoSchemaElementState elementState);
	
    /// Add a table to the rollback cache for the current connection.
    //
    /// Parameters:
    ///      tableName - the name of the column's table.
    ///      columnName - the column name.
    ///      elementState - whether the column was created or deleted.
    void AddRollbackColumn(FdoString* tableName, FdoString* columnName, FdoSchemaElementState elementState);
	
    /// Clears all rollback entries for the current connection.
    void ClearRollback();

    /// Gets the rollback entries for the current connection.
    /// Returns NULL if there are no entries.
	bool HasRollbackEntries();

    /// Gets the rollback entry for the given table in the current connection.
	const FdoSmPhRbTable* RefRollbackTable(FdoString* name);
 
    /// Gets the rollback entry for the table and column, from the current connection.
	const FdoSmPhRbColumn* RefRollbackColumn(FdoString* tableName, FdoString* columnName);

    /// Get writer for writing feature schemas to the current connection.
    FdoPtr<FdoSmPhSchemaWriter> GetSchemaWriter();

    /// Get writer for writing classes to the current connection.
    FdoPtr<FdoSmPhClassWriter> GetClassWriter();

    /// Get writer for writing properties to the current connection.
    FdoPtr<FdoSmPhPropertyWriter> GetPropertyWriter();

    /// Get writer for writing attribute dependencies to the current connection.
    FdoPtr<FdoSmPhDependencyWriter> GetDependencyWriter();

    /// Get writer for writing association definitions to the current connection.
    FdoPtr<FdoSmPhAssociationWriter> GetAssociationWriter();

    /// Get writer for writing spatial contexts to the current connection.
    FdoPtr<FdoSmPhSpatialContextWriter> GetSpatialContextWriter();

    /// Get writer for writing spatial context groups to the current connection.
    FdoPtr<FdoSmPhSpatialContextGroupWriter> GetSpatialContextGroupWriter();

    /// Get writer for writing spatial context associations to the current connection.
    FdoPtr<FdoSmPhSpatialContextGeomWriter> GetSpatialContextGeomWriter();

/*
    /// Get writer for writing indexes to the current connection.
    FdoSmPhIndexWriter* GetIndexWriter();

    /// Get writer for writing index properties to the current connection.
    FdoSmPhIndexPropertyWriter* GetIndexPropertyWriter();
*/
    /// Get writer for writing schema attribute dictionary entries to the current connection.
    FdoPtr<FdoSmPhSADWriter> GetSADWriter();

/*	// Execute the given select statement. Get every row but don't return anything.
    /// This function is useful for applying RDBMS locks ( select ... for update of ... ).
	void ExecuteSelect( FdoSmPhFieldCollection* fields, FdoStringP whereClause );
*/
    /// Post all table modifications to the current database.
	void Commit();

    /// Given a field value, returns the formatted equivalent that can be 
    /// embedded in a SQL statement.
	//
    /// If value is zero length then "null" is returned.
    /// Otherwise if valueType is string or date then "'[value]'" is returned.
    /// Otherwise "[value]" is returned
	virtual FdoStringP FormatSQLVal( FdoStringP value, FdoSmPhColType valueType ) = 0;

    /// Given a field and default value, returns a column expression that can be 
    /// embedded in a SQL statement. The expression returns the underlying column value
    /// when this value is not null. The default value is returned when the column value
    /// is null.
	//
    /// Parameters:
    ///      fieldName: the name of the field
    ///      colName: name of the underlying column
    ///      defaultValue: field's default value
    ///      fieldType: the type of the underlying column
    virtual FdoStringP FormatDefaultedField( FdoStringP fieldName, FdoStringP colName, FdoStringP defaultValue, FdoSmPhColType fieldType ) = 0;

    /// Get the provider-specific SQL format for the current date
    virtual FdoStringP FormatCurrentDateField() = 0;    

    /// Get the provider-specific SQL format for a bind field.
    /// pos is 0-based.
    virtual FdoStringP FormatBindField( int pos ) = 0;    

    virtual bool SupportsAnsiQuotes();

    virtual bool IsRdbUnicode() = 0;

    virtual bool IsRdbObjNameAscii7();

    /// Gets the provider-specific maximum length of a database object (table, view etc.) 
    /// name.
    virtual FdoSize DbObjectNameMaxLen() = 0;

    /// Gets the provider-specific maximum length of a table
    /// name. By default, returns the same value as DbObjectNameMaxLen()
    virtual FdoSize TableNameMaxLen();

    /// Gets the provider-specific maximum length of a column name.
    virtual FdoSize ColNameMaxLen() = 0;

    // The following functions take various types of object names and 
    // convert them to the default case for the current RDBMS
    // (e.g. upper case for Oracle, lower for most others).
    // The default implementations always convert to lower case.
    // Each Provider, where the default case is upper, must override 
    /// these functions. 
    virtual FdoStringP GetDcDatabaseName( FdoStringP databaseName );

    virtual FdoStringP GetDcOwnerName( FdoStringP ownerName );

    virtual FdoStringP GetDcDbObjectName( FdoStringP objectName );

    virtual FdoStringP GetDcColumnName( FdoStringP columnName );

    virtual FdoStringP GetDcRdbmsObjectName( FdoStringP columnName );

    // Given a database object name, returns the actual name it would
    // have in the RDBMS.
    // returns the given name unaltered by default
    virtual FdoStringP GetRealDbObjectName( FdoStringP objectName );

    // Given a database object name, return the name as it appears in 
    // MetaSchema tables. 
    virtual FdoStringP DbObject2MetaSchemaName( FdoStringP objectName );

    /// Censor a database object name. Only alphanumeric characters 
    /// ( or '_' or '$' ) are allowed in a table or column name.
    /// This function replaces other characters with '_'.
	virtual FdoStringP CensorDbObjectName( FdoStringP objName );

    /// Returns true if the given objName is a reserved word.
    virtual FdoBoolean IsDbObjectNameReserved( FdoStringP objName )
    {
    /// No reserved words in generic Schema Manager.
        return false;
    }

    /// Returns true if the given owner (datastore) name is a reserved word.
    virtual FdoBoolean IsOwnerNameReserved( FdoStringP ownerName )
    {
    /// By default, treat owner name like any other object name.
        return IsDbObjectNameReserved( ownerName );
    }

    /// Returns true if a default geometric property should be generated
    /// from ordinate columns.
    virtual FdoBoolean IsGeometryFromOrdinatesWanted()
    {
        return false;
    }

    /// Checks that the length of a string is within the given limit. The length of
    /// of the UTF8 version of pString is checked. An exception is thrown if pString
    /// is too long.
    //
    /// Parameters:
    ///      pString - the string to check.
    ///      limit - throw exception if string length is > than this limit.
    ///      pElementName - the name of the Schema Element that has this value
    ///          (used in the exception message).
    ///      pItemName - the name of the Schema Element attribute that has this value
    ///          (used in the exception message).
    void ValidateStringLength( FdoStringP pString, size_t limit, FdoStringP pElementName, FdoStringP pItemName ) const;

    /// Sets the rollback cache for this physical schema manager.
    /// It is passed in since it can be shared among schema managers.
    void SetRollbackCache( FdoSmPhRbCacheP rollbackCache );

    /// Create a temporary database object.
    virtual FdoSmPhDbObjectP CreateTempObject() = 0;

    /// Clear all physical element caches.
    virtual void Clear();

    /// Different schema element types that can have Schema Attribute 
    /// Dictionaries. These define the different values found in the 
    /// F_SAD.ElementType database column.
	static const FdoStringP SchemaType;
	static const FdoStringP ClassType;
	static const FdoStringP PropertyType;

    /// Various Schema Element attribute value length limits. 1 is subtracted to 
    /// account for null terminator. 
    static const size_t SADValueLimit = 3999;
 
    /// Prepended to object names that don't have an alphabetic first character.
	static const FdoStringP ObjPrefix;

    /// Prepended to names of feature schemas generated from physical schemas.
    static const FdoStringP RdSchemaPrefix;

    /// Output the schemas to an XML file.
    /// Mainly for unit testing.
	void XMLSerialize( FdoString* sFileName ) const;

protected:
    void SetDefaultOwnerName( FdoStringP name)
    {
        mDefaultOwnerName = name;
    }

	FdoSmPhMgr();

    /// Provider-specific function to create a database.
    virtual FdoSmPhDatabaseP CreateDatabase(FdoStringP database) = 0;

    /// Get writer for writing feature schemas to the current connection.
    virtual FdoPtr<FdoSmPhSchemaWriter> NewSchemaWriter() = 0;

    /// Get writer for writing classes to the current connection.
    virtual FdoPtr<FdoSmPhClassWriter> NewClassWriter() = 0;

    /// Get writer for writing properties to the current connection.
    virtual FdoPtr<FdoSmPhPropertyWriter> NewPropertyWriter() = 0;

    /// Get writer for writing attribute dependencies to the current connection.
    virtual FdoPtr<FdoSmPhDependencyWriter> NewDependencyWriter() = 0;

    /// Get writer for writing associations to the current connection.
    virtual FdoPtr<FdoSmPhAssociationWriter> NewAssociationWriter() = 0;

    /// Get writer for writing classes to the current connection.
    virtual FdoPtr<FdoSmPhSADWriter> NewSADWriter() = 0;
    /// Get writer for writing spatial contexts to the current connection.
    virtual FdoPtr<FdoSmPhSpatialContextWriter> NewSpatialContextWriter() = 0;

    /// Get writer for writing spatial context groups to the current connection.
    virtual FdoPtr<FdoSmPhSpatialContextGroupWriter> NewSpatialContextGroupWriter() = 0;

private:
    /// Cache of databases retrieved so far.
	FdoSmPhDatabasesP mDatabases;

    /// re-usable metaschema writers.
    FdoPtr<FdoSmPhSchemaWriter> mSchemaWriter;
    FdoPtr<FdoSmPhClassWriter> mClassWriter;
    FdoPtr<FdoSmPhPropertyWriter> mPropertyWriter;
    FdoPtr<FdoSmPhDependencyWriter> mDependencyWriter;
    FdoPtr<FdoSmPhAssociationWriter> mAssociationWriter;
    FdoPtr<FdoSmPhSpatialContextWriter> mSpatialContextWriter;
    FdoPtr<FdoSmPhSpatialContextGroupWriter> mSpatialContextGroupWriter;
    FdoPtr<FdoSmPhSpatialContextGeomWriter> mSpatialContextGeomWriter;
/*
    FdoPtr<FdoSmPhIndexWriter> mpIndexWriter;
    FdoPtr<FdoSmPhIndexPropertyWriter> mpIndexPropertyWriter;
*/
    FdoPtr<FdoSmPhSADWriter> mpSADWriter;

	FdoSmPhRbCacheP mRollbackCache;

    FdoStringP mDefaultOwnerName;

protected:
    // These members are protected instead of private to allow access by 
    // an overridden constructor.
    FdoStringP mProviderName;
    FdoIoStreamP mConfigDoc;
    FdoFeatureSchemasP mConfigSchemas;
    FdoSchemaMappingsP mConfigMappings;

private:
    FdoDictionaryP mDbObjectClassification;
};


#endif


