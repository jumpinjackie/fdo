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
#ifndef FDOSMPHPOSTGISTABLE_H
#define FDOSMPHPOSTGISTABLE_H

#include "DbObject.h"
#include "../../../SchemaMgr/Ph/Table.h"
#include "Fkey.h"

/// Definition of Table class for PostGIS provider.
/// Represents an PostGIS schema (user).
///
/// \todo TODO: mloskot - Consider usage of TABLESPACE feature from
///       PostgreSQL. This could mimic DATA/INDEX DIRECTORY options
///       from MySQL. Medium priority.
///
class FdoSmPhPostGisTable :
    public FdoSmPhGrdTable,
    public FdoSmPhPostGisDbObject
{
public:

    /// Constructs an instance of Table.
    ///
    /// \param
    /// name [in] - name of table.
    /// \param
    /// owner [in] - owning schema for table.
    /// \param
    /// state [in] - indicates whether this is a new or existing table.
    /// \param
    /// pkeyName [in] - name for table primary key. Applies only to
    /// new tables. Primary key name on existing tables not changed.
    ///
    FdoSmPhPostGisTable(FdoStringP name,
        const FdoSmPhOwner* owner,
        FdoSchemaElementState state = FdoSchemaElementState_Added,
        FdoStringP pkeyName = L"",
        FdoSmPhRdDbObjectReader* reader = NULL);

    /// Destructor.
    ~FdoSmPhPostGisTable();

    /// No lock types currently supported in PostGIS.
    ///
    /// \param
    /// size [out] - size of lock types collection.
    /// Currently, always 0 is assigned.
    /// \return
    /// Always returns NULL and size = 0.
    ///
	virtual const FdoLockType* GetLockTypes(FdoInt32& size) const;

    FdoSmPhDbObjectsP GetSubDeletedTables();

    /// Set the modification state of this table.
	virtual void SetElementState(FdoSchemaElementState elementState);

    /// Creates primary key reader for table.
    virtual FdoPtr<FdoSmPhRdPkeyReader> CreatePkeyReader() const;

    /// Creates foreign key reader for table.
    virtual FdoPtr<FdoSmPhRdFkeyReader> CreateFkeyReader() const;

    /// Creates constraint reader for table.
    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader(
        FdoString* type) const;

    FdoStringP GetAddHdgSql();

    /// Returns a CHECK() clause, each provider must implement. It should be pure virtual.
	virtual FdoStringP GetCkeyClause( FdoStringP columnName, FdoDataPropertyP fdoProp );

private:

    /// Destructor is private.
    /// Direct construction of an object of this class is forbidden.
    FdoSmPhPostGisTable();

    virtual void CommitChildren( bool isBeforeParent );
    virtual void Commit( bool fromParent, bool isBeforeParent );

    virtual bool Add();
    virtual bool Modify();
    virtual bool Delete();

    virtual bool AddColumn( FdoSmPhColumnP column );
    virtual bool DeleteColumn( FdoSmPhColumnP column );

    // Given a constraint name, return the name to use in DDL operations, such 
    // as dropping the constraint.
    virtual FdoStringP GetConstraintDDLName( FdoStringP constraintName ) const;

    /// Foreign Key creator implementation.
    virtual FdoSmPhFkeyP NewFkey(FdoStringP name,
        FdoStringP pkeyTableName,
        FdoStringP pkeyTableOwner,
        FdoSchemaElementState state = FdoSchemaElementState_Added);

    FdoStringP ConvertCkey( FdoDataValue *val );

    FdoSmPhDbObjectsP mSubDeletedTables;
};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisTable> FdoSmPhPostGisTableP;

#endif // FDOSMPHPOSTGISTABLE_H
