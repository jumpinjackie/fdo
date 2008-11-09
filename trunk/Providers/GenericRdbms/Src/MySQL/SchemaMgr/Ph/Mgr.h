#ifndef FDOSMPHMYSQLMGR_H
#define FDOSMPHMYSQLMGR_H     1
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

#include "../../../SchemaMgr/Ph/Mgr.h"
#include "../../../SchemaMgr/Ph/Cfg/SchemaReader.h"
#include "../../../SchemaMgr/Ph/Cfg/ClassReader.h"
#include "../../../SchemaMgr/Ph/Cfg/PropertyReader.h"
#include <Sm/Ph/Reader.h>

// MySql Provider implementation of the Physical Schema Manager
class FdoSmPhMySqlMgr : public FdoSmPhGrdMgr
{
public:

    // Create physical schema manager with connection
    // to the MySql Instance.
    FdoSmPhMySqlMgr( GdbiConnection* connection, FdoStringP mqlSchemaName);

    FdoSmPhMySqlMgr( ) { }

    ~FdoSmPhMySqlMgr(void);

    // Temporary object creator implementation
    virtual FdoSmPhDbObjectP CreateTempObject();

    // Create various readers for AutoGenerating schemas based on config document directives.
    virtual FdoPtr<FdoSmPhCfgSchemaReader> CreateCfgSchemaReader( FdoSmPhRowsP rows );
    virtual FdoPtr<FdoSmPhCfgClassReader> CreateCfgClassReader( FdoSmPhRowsP rows, FdoStringP schemaName );
    virtual FdoPtr<FdoSmPhCfgPropertyReader> CreateCfgPropertyReader( FdoStringP schemaName, FdoStringP className, FdoSmPhDbObjectP dbObject );

    // Extract the MySQL database name from a Schema Override set
    virtual FdoStringP GetOverrideOwner(FdoRdbmsOvSchemaMappingP mapping);
    // Extract the MySQL table storage name (data directory name) from a Schema Override set
    virtual FdoStringP GetOverrideTableStorage(FdoRdbmsOvSchemaMappingP mapping);
    // Given a schema override set, extract the index storage setting
    virtual FdoStringP GetOverrideIndexStorage(FdoRdbmsOvSchemaMappingP mapping);
    // Given a schema override set, extract the table storage engine setting
    virtual FdoStringP GetOverrideTableStorageEngine(FdoRdbmsOvSchemaMappingP mapping);


/* TODO: this is not quite right yet. The readers that read from a non-default
   database link need to have the database name associated with them. These
   queries cannot be reused when database link name changes.

    FdoSmPhReaderP GetDbObjectsReader( bool databaseSet );

    void SetDbObjectsReader( FdoSmPhReaderP reader, bool databaseSet );

    FdoSmPhReaderP GetDbObjectReader( bool databaseSet );

    void SetDbObjectReader( FdoSmPhReaderP reader, bool databaseSet );
*/
    virtual FdoStringP FormatDefaultedField( FdoStringP fieldName, FdoStringP colName, FdoStringP defaultValue, FdoSmPhColType fieldType );

	// Given a field value, returns the formatted equivalent that can be 
	// embedded in a SQL statement.
	//
	// If value is zero length then "null" is returned.
	// Otherwise if valueType is string or date then "'<value>'" is returned.
    // Otherwise "<value>" is returned
	virtual FdoStringP FormatSQLVal( FdoStringP value, FdoSmPhColType valueType );

    virtual FdoPtr<FdoDataValue> ParseSQLVal( FdoStringP stringValue );

    virtual FdoStringP FormatCurrentDateField();    

    virtual FdoStringP FormatBindField( int pos );    

    virtual FdoBoolean IsDbObjectNameReserved( FdoStringP objName );

    virtual bool IsRdbObjNameAscii7();

    virtual FdoSize DbObjectNameMaxLen();

    virtual FdoSize ColNameMaxLen();

protected:

    virtual FdoSmPhDatabaseP CreateDatabase(FdoStringP database);

private:
    FdoSmPhReaderP mDbObjectsReader;
    FdoSmPhReaderP mDbObjectsReaderD;

    FdoSmPhReaderP mDbObjectReader;
    FdoSmPhReaderP mDbObjectReaderD;

    class MySqlStringMap : public StringMap
    {
    public:
        MySqlStringMap();
        virtual ~MySqlStringMap() {}
    };

    // Lists all reserved names (not allowed as database object or column
    // names in MySql).
    static MySqlStringMap mMySqlReservedDbObjectNames;

};

typedef FdoPtr<FdoSmPhMySqlMgr> FdoSmPhMySqlMgrP;


#endif

