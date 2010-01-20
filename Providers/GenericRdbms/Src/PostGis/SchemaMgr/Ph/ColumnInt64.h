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
#ifndef FDOSMPHPOSTGISCOLUMNINT64_H
#define FDOSMPHPOSTGISCOLUMNINT64_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnInt64.h>
#include "Column.h"

/// Represents a short column type (64-bit integer).
/// This type is based on PostgreSQL type BIGINT,
/// storage size is 8 bytes and
/// range is -9223372036854775808 to 9223372036854775807.
///
class FdoSmPhPostGisColumnInt64 :
    public FdoSmPhColumnInt64,
    public FdoSmPhPostGisColumn
{
public:

    // Create an int64 column type.
    FdoSmPhPostGisColumnInt64(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        bool isAutoincremented,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    virtual ~FdoSmPhPostGisColumnInt64();

    /// Returns RDBI constant for large-range integer type.
    /// \return
    /// RDBI type of this column type is RDBI_LONGLONG.
    ///
    virtual int GetRdbType();

    virtual FdoStringP GetTypeSql();

    virtual FdoStringP GetNullabilitySql();

    virtual FdoStringP GetAutoincrementSql();

    virtual FdoStringP GetDefaultValueSql();
};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisColumnInt64> FdoSmPhPostGisColumnInt64P;

#endif // FDOSMPHPOSTGISCOLUMNINT64_H
