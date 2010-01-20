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
#ifndef FDOSMPHPOSTGISCOLUMNINT32_H
#define FDOSMPHPOSTGISCOLUMNINT32_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnInt32.h>
#include "Column.h"

/// Represents a short column type (32-bit integer).
/// This type is based on PostgreSQL type INTEGER,
/// storage size is 4 bytes and range is -2147483648 to +2147483647.
///
class FdoSmPhPostGisColumnInt32 :
    public FdoSmPhColumnInt32,
    public FdoSmPhPostGisColumn
{
public:
    
    // Create an int32 column type.
    FdoSmPhPostGisColumnInt32(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        bool isAutoincremented,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    virtual ~FdoSmPhPostGisColumnInt32();

    /// Returns RDBI constant for usual-range integer type.
    /// \return
    /// RDBI type of this column type is RDBI_INT.
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
typedef FdoPtr<FdoSmPhPostGisColumnInt32> FdoSmPhPostGisColumnInt32P;

#endif // FDOSMPHPOSTGISCOLUMNINT32_H