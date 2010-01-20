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
#ifndef FDOSMPHPOSTGISCOLUMNBOOL_H
#define FDOSMPHPOSTGISCOLUMNBOOL_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnBool.h>
#include "Column.h"

/// Represents a boolean column type for the PostGIS provider.
/// This Boolean type is based on standard SQL boolean.
/// In SQL, boolean can have one of only two states: "true" or "false".
/// A third state, "unknown", is represented by the SQL null value.
/// "true" and "false" can be represented in various way, but
/// using keywords TRUE and FALSE is prefered and SQL-compliant.
/// 
/// boolean uses 1 byte of storage, so it should be compatible with
/// RDBI_BOOLEAN from RDBI layer.
///
class FdoSmPhPostGisColumnBool :
    public FdoSmPhColumnBool,
    public FdoSmPhPostGisColumn
{
public:

    /// Constructor.
    FdoSmPhPostGisColumnBool(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    /// No uninitalization steps are executed.
    virtual ~FdoSmPhPostGisColumnBool();

    /// Returns RDBI constant for BLOB type.
    virtual int GetRdbType();

    virtual FdoStringP GetDefaultValueSql();

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
/// 
typedef FdoPtr<FdoSmPhPostGisColumnBool> FdoSmPhPostGisColumnBoolP;

#endif // FDOSMPHPOSTGISCOLUMNBOOL_H