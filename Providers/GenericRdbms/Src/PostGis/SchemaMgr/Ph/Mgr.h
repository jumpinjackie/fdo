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
#ifndef FDOSMPHPOSTGISMGR_H
#define FDOSMPHPOSTGISMGR_H

#include "../../../SchemaMgr/Ph/Mgr.h"
#include "../../../SchemaMgr/Ph/Cfg/SchemaReader.h"
#include "../../../SchemaMgr/Ph/Cfg/ClassReader.h"
#include "../../../SchemaMgr/Ph/Cfg/PropertyReader.h"
#include "Rd/SchemaReader.h"
#include <Sm/Ph/Reader.h>

/// Definition of Physical Schema Manager for PostGIS provider.
///
class FdoSmPhPostGisMgr : public FdoSmPhGrdMgr
{
public:

    /// \todo To be documented
    FdoSmPhPostGisMgr();


    /// Create physical schema manager with connection
    /// to the PostGIS instance.
    FdoSmPhPostGisMgr(GdbiConnection* conn, FdoStringP schemaName);

    /// \todo To be documented
    ~FdoSmPhPostGisMgr();

    // Get the PostGres server version number for the current connection.
    FdoStringP GetDbVersion();

    virtual bool SupportsNumericCoordinateSystemName();

    virtual bool SupportsMixedCase();

    /// Constructor of temporary object in schema.
    virtual FdoSmPhDbObjectP CreateTempObject();

    // Create various readers for AutoGenerating schemas based on config document directives.

    virtual FdoPtr<FdoSmPhRdClassReader> CreateRdClassReader( 
        FdoPtr<FdoSmPhRowCollection> rows, 
        FdoStringP schemaName,
        FdoStringP className,
        FdoBoolean keyedOnly = true,// If true, skip tables without key.
        FdoStringP database = L"",  // Database where RDBMS schema resides (current connection by default)
        FdoStringP owner = L""      // the RDBMS schema (defaults to current schema)
    );

    virtual FdoPtr<FdoSmPhRdSchemaReader> CreateRdSchemaReader( 
        FdoPtr<FdoSmPhRowCollection> rows, 
        FdoPtr<FdoSmPhOwner> owner, 
        bool dsInfo 
    );

    /// \todo To be documented
    virtual FdoPtr<FdoSmPhCfgSchemaReader> CreateCfgSchemaReader(
        FdoSmPhRowsP rows);

    /// \todo To be documented
    virtual FdoPtr<FdoSmPhCfgClassReader> CreateCfgClassReader(
        FdoSmPhRowsP rows,
        FdoStringP schemaName);

    /// \todo To be documented
    virtual FdoPtr<FdoSmPhCfgPropertyReader> CreateCfgPropertyReader(
        FdoStringP schemaName,
        FdoStringP className,
        FdoSmPhDbObjectP dbObject);

    /// Extract the PostGIS database name from a Schema Override set.
    ///
    /// \todo To be documented
    ///
    virtual FdoStringP GetOverrideOwner(FdoRdbmsOvSchemaMappingP mapping);

    /// \todo To be documented.
    virtual FdoStringP FormatDefaultedField(FdoStringP fieldName,
        FdoStringP colName,
        FdoStringP defaultValue,
        FdoSmPhColType fieldType);

	/// Given a field value, returns the formatted equivalent
    /// that can be embedded in a SQL statement.
	///
    /// \param
    /// value [in] - if value is zero length then "null" is returned.
    /// If valueType is string or date then "'<value>'" is returned,
    /// otherwise "<value>" is returned.
	///
    /// \param
    /// type [in] - type of column value.
	virtual FdoStringP FormatSQLVal(FdoStringP value, FdoSmPhColType type);

    /// Returns name of CURRENT_DATE function to be embedded
    /// in the SQL query string.
    ///
    /// \return
    /// String with name of CURRENT_DATE function.
    virtual FdoStringP FormatCurrentDateField();

    // Converts object name to default case and defaults object schema
    // to public.
    virtual FdoStringP GetDcDbObjectName( FdoStringP objectName );

    // Given a database object name, returns the actual name it would
    // have in the RDBMS.
    // Prepends public schema name if objectName does not have a schema
    // name part.
    virtual FdoStringP GetRealDbObjectName( FdoStringP objectName );

    // Given a database object name, return the name as it appears in 
    // MetaSchema tables. For forward datastore compatibility, the schema
    // name prefix is removed if the schema is public.
    virtual FdoStringP DbObject2MetaSchemaName( FdoStringP objectName );

    /// Formats placeholder for bind parameter.
    /// PostgreSQL refers to parameters  in query $1, $2, etc.
    /// \param
    /// pos [in] - position of parameter placeholder in query string,
    /// \return
    /// String with formated parameter placeholder.
    virtual FdoStringP FormatBindField(int pos);    

    FdoStringP FormatCollateColumnSql( FdoStringP columnName);

    /// \todo To be documented.
    virtual FdoBoolean IsDbObjectNameReserved(FdoStringP objName);

    /// \todo To be documented.
    virtual FdoSize DbObjectNameMaxLen();

    /// \todo To be documented.
    virtual FdoSize ColNameMaxLen();

protected:

    /// \todo To be documented.
    virtual FdoSmPhDatabaseP CreateDatabase(FdoStringP database);

    virtual FdoPtr<FdoSmPhSpatialContextGroupWriter> NewSpatialContextGroupWriter();

private:

    /// Collection of reserved words for PostgreSQL/PostGIS.
    /// This subclass is extends list of reserved words defined
    /// in base class FdoSmPhGrdMgr::StringMap.
    ///
    /// PostgreSQL/PostGIS specific words are inserted
    /// in the constructor of the class.
    ///
    class ReservedWordsMap : public StringMap
    {
    public:
        ReservedWordsMap();
        ~ReservedWordsMap();
    };

    unsigned long mDbVersion;

    /// Lists all reserved words not allowed as database object or
    /// column names in PostgreSQL/PostGIS.
    static ReservedWordsMap mReservedWords;

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisMgr> FdoSmPhPostGisMgrP;

#endif // FDOSMPHPOSTGISMGR_H