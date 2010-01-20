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
#ifndef FDOSMPHPOSTGISDBOBJECT_H
#define FDOSMPHPOSTGISDBOBJECT_H

#include <Sm/Ph/DbObject.h>
#include <Sm/Ph/Rd/BaseObjectReader.h>
#include <Sm/Ph/Rd/ColumnReader.h>

/// Definition of database object for PostGIS provider.
///
class FdoSmPhPostGisDbObject : virtual public FdoSmPhDbObject
{
public:
    
    /// Get the database-specific qualified name for this
    /// database object.
    virtual FdoStringP GetDbQName() const;

    /// Get the indicator on whether or not the class supports write.
    ///
    virtual bool GetSupportsWrite() const;

    // Reverse-engineer FDO feature schema name from this object.
    // Returns object's user name.
    virtual FdoStringP GetBestSchemaName() const;

    // Reverse-engineer FDO class name from this object.
    // Returns object name with user name part removed.
    virtual FdoStringP GetBestClassName() const;

protected:

    /// Default constructor.
    FdoSmPhPostGisDbObject();

    // Constructs an instance of a database object.
    //
    // Parameters:
    //      name: object name
    //      pOwner: the owning physical schema.
    FdoSmPhPostGisDbObject(FdoStringP name, const FdoSmPhOwner* owner,
        FdoSmPhRdDbObjectReader* reader = NULL);

    virtual ~FdoSmPhPostGisDbObject();

    // PostGIS does not allow "create index" or "drop view" statements to reference
    // the index by qualified name. This function handles switching the current schema
    // to the index owner before executing the statement, and switching back to the 
    // original current schema after.
    void ActivateOwnerAndExecute(FdoStringP sqlStmt);
    
    // Various column creator implementations
    virtual FdoSmPhColumnP NewColumnBLOB(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnChar(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
        int length,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnDate(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnDecimal(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnSingle(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnDouble(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnGeom(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhScInfoP AssociatedSCInfo,
        bool isNullable = true,
        bool hasElevation = true,
        bool hasMeasure = false,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnBool(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnByte(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnInt16(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
		bool isAutoincremented,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnInt32(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
		bool isAutoincremented,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnInt64(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
		bool isAutoincremented,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnUnknown(FdoStringP columnName,
        FdoStringP typeName,
        FdoSchemaElementState state,
        bool isNullable,
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colReader = NULL);

    virtual FdoSmPhColumnP NewColumnDbObject(FdoStringP columnName,
        FdoSchemaElementState state,
        bool isNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* colReader = NULL);

    // Index creator implementation
    virtual FdoSmPhIndexP NewIndex(
        FdoStringP name,
        bool isUnique,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    // Spatial Index creator implementation
    virtual FdoSmPhIndexP NewSpatialIndex(
        FdoStringP name,
        bool isUnique,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    virtual FdoPtr<FdoSmPhRdBaseObjectReader> CreateBaseObjectReader() const;
    // Column reader creator implementation.
    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader();
    virtual FdoPtr<FdoSmPhRdIndexReader> CreateIndexReader() const;

private:
};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisDbObject> FdoSmPhPostGisDbObjectP;

#endif // FDOSMPHPOSTGISDBOBJECT_H
