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
#ifndef FDOSMPHPOSTGISCOLUMNDOUBLE_H
#define FDOSMPHPOSTGISCOLUMNDOUBLE_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnDecimal.h>
#include "Column.h"

/// Represents a floating-point double precision column type.
/// This type is based on PostgreSQL type DOUBLE PRECISION,
/// an inexact, variable-precision numeric type.
/// Storage size of DOUBLE PRECISION is 8 bytes,
/// with 15 decimal digits precision.
///
class FdoSmPhPostGisColumnDouble :
    public FdoSmPhColumnDouble,
    public FdoSmPhPostGisColumn
{
public:

    /// Create a double precision floating-point column type.
    ///
    FdoSmPhPostGisColumnDouble(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    /// No uninitalization steps are executed.
    virtual ~FdoSmPhPostGisColumnDouble();

    /// Returns RDBI constant for floating point type.
    /// \return
    /// RDBI type of this column type is RDBI_DOUBLE.
    virtual int GetRdbType();

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisColumnDouble> FdoSmPhPostGisColumnDoubleP;

#endif // FDOSMPHPOSTGISCOLUMNDOUBLE_H
