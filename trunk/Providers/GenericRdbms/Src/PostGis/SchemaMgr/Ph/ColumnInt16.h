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
#ifndef FDOSMPHPOSTGISCOLUMNINT16_H
#define FDOSMPHPOSTGISCOLUMNINT16_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnInt16.h>
#include "Column.h"

/// Represents a short column type (16-bit integer).
/// This type is based on PostgreSQL type SMALLINT,
/// storage size is 2 bytes and range is -32768 to +32767.
///
class FdoSmPhPostGisColumnInt16 :
    public FdoSmPhColumnInt16,
    public FdoSmPhPostGisColumn
{
public:

    // Create an int16 column type.
    FdoSmPhPostGisColumnInt16(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    virtual ~FdoSmPhPostGisColumnInt16();

    /// Returns RDBI constant for small-range integer type.
    /// \return
    /// RDBI type of this column type is RDBI_SHORT.
    virtual int GetRdbType();

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisColumnInt16> FdoSmPhPostGisColumnInt16P;

#endif // FDOSMPHPOSTGISCOLUMNINT16_H
