#ifndef FDOSMPHSQSMGR_H
#define FDOSMPHSQSMGR_H     1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include "Rd/SchemaReader.h"
#include <Sm/Ph/Reader.h>

// SqlServer Provider implementation of the Physical Schema Manager
class FdoSmPhSqsMgr : public FdoSmPhGrdMgr
{
public:

    // Create physical schema manager with connection
    // to the SqlServer Instance.
    FdoSmPhSqsMgr( GdbiConnection* connection, FdoStringP SqsSchemaName);

    FdoSmPhSqsMgr( ) { }

    ~FdoSmPhSqsMgr(void);

    // Get the SqlServer version number for the current connection.
    // Note: This function returns 0.0 for Sql Server 2000. 
    // It might not be totally reliable for Sql Server 2005 and is currently
    // only used by the unit tests.
    FdoStringP GetDbVersion();

    virtual CoordinateSystemMatchLevel GetCoordinateSystemMatchLevel();
    virtual bool SupportsNumericCoordinateSystemName();

    bool IsGeogLatLong();

    // Gets the extended coordinate system file name.
    // SQL Server's coordinate system catalogue (sys.spatial_reference_systems)
    // contains only geodetic systems. However, SQL Server allows geometries
    // to have coordinate systems not in this catalogue. In this case, the only info
    // on these coordinate systems, that can be retrieved from the RDBMS, is the SRID
    // (usually an EPSG number). The provider also needs the WKT 
    // so it can give enough information about the coordinate system, via 
    // FdoIGetSpatialContexts, to applications such as Map and MapGuide.
    //
    // The WKT's for non-catalogued coordinate systems can be specified the file returned
    // by this function.
    FdoStringP GetExtendedCsysFilename();

    // Temporary object creator implementation
    virtual FdoSmPhDbObjectP CreateTempObject();

    // Create various readers for AutoGenerating schemas based on config document directives.
    virtual FdoPtr<FdoSmPhCfgSchemaReader> CreateCfgSchemaReader( FdoSmPhRowsP rows );
    virtual FdoPtr<FdoSmPhCfgClassReader> CreateCfgClassReader( FdoSmPhRowsP rows, FdoStringP schemaName );
    virtual FdoPtr<FdoSmPhCfgPropertyReader> CreateCfgPropertyReader( FdoStringP schemaName, FdoStringP className, FdoSmPhDbObjectP dbObject );

    virtual FdoPtr<FdoSmPhRdSchemaReader> CreateRdSchemaReader( 
        FdoPtr<FdoSmPhRowCollection> rows, 
        FdoPtr<FdoSmPhOwner> owner, 
        bool dsInfo 
    );

    // Extract the SqlServer server name from a Schema Override set
    virtual FdoStringP GetOverrideDatabase(FdoRdbmsOvSchemaMappingP mapping);
    // Extract the SqlServer database name from a Schema Override set
    virtual FdoStringP GetOverrideOwner(FdoRdbmsOvSchemaMappingP mapping);
    // Extract the SqlServer tablestorage (i.e. table filegroup) name from a Schema Override set
    virtual FdoStringP GetOverrideTableStorage(FdoRdbmsOvSchemaMappingP mapping);
    // Given a schema override set, extract the index storage setting
    virtual FdoStringP GetOverrideIndexStorage(FdoRdbmsOvSchemaMappingP mapping);
    // Given a schema override set, extract the text storage setting
    virtual FdoStringP GetOverrideTextStorage(FdoRdbmsOvSchemaMappingP mapping);
    // Given a schema override set, extract the text-in-row setting
    virtual FdoStringP GetOverrideTextInRow(FdoRdbmsOvSchemaMappingP mapping);



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

    virtual FdoStringP FormatOrderCol( FdoStringP colName, FdoSmPhColType colType );
    
    virtual FdoStringP FormatCurrentDateField();    

    virtual FdoStringP FormatBindField( int pos );    

    virtual FdoBoolean IsDbObjectNameReserved( FdoStringP objName );

    virtual bool IsRdbObjNameAscii7();

    virtual FdoSize DbObjectNameMaxLen();

    virtual FdoSize ColNameMaxLen();

    virtual bool SupportsMixedCase();

    // No case conversion for SqlServer. 
    virtual FdoStringP GetDcRdbmsObjectName( FdoStringP objectName );

    // Converts object name to default case and defaults object user
    // to dbo.
    virtual FdoStringP GetDcDbObjectName( FdoStringP objectName );

    // Given a database object name, returns the actual name it would
    // have in the RDBMS.
    // Prepends dbo user name if objectName does not have a user
    // name part.
    virtual FdoStringP GetRealDbObjectName( FdoStringP objectName );

    // Given a database object name, return the name as it appears in 
    // MetaSchema tables. For forward datastore compatibility, the user
    // name prefix is removed if the user is dbo.
    virtual FdoStringP DbObject2MetaSchemaName( FdoStringP objectName );

    FdoInt64 IndexName2Srid( FdoStringP indexName );

    // Utility function for various readers that bind in owner and object name.
    // Sets up binds for owner plus user and local object name parts of object name.
    // Also sets the value for each bind field.
    // Returns the created bind fields.
    FdoSmPhRowP MakeByDbObjectBinds (FdoStringP object_owner, FdoStringP object_name);

    // Resets bind values for bind fields created by MakeByDbObjectBinds.
    void SetByDbObjectBinds (FdoSmPhRowP binds, FdoStringP object_owner, FdoStringP object_name);

    virtual FdoStringP ClassName2DbObjectName(FdoStringP schemaName, FdoStringP className);

protected:

    virtual FdoSmPhDatabaseP CreateDatabase(FdoStringP database);

private:
    FdoSmPhReaderP mDbObjectsReader;
    FdoSmPhReaderP mDbObjectsReaderD;

    FdoSmPhReaderP mDbObjectReader;
    FdoSmPhReaderP mDbObjectReaderD;

    class SqsStringMap : public StringMap
    {
    public:
        SqsStringMap();
        virtual ~SqsStringMap() {}
    };

    // Lists all reserved names (not allowed as database object or column
    // names in SqlServer).
    static SqsStringMap mSqsReservedDbObjectNames;

    FdoStringP mDbVersion;

};

typedef FdoPtr<FdoSmPhSqsMgr> FdoSmPhSqsMgrP;


#endif


