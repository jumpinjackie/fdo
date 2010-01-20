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
#ifndef FDOSMPHPOSTGISCOLUMNSINGLE_H
#define FDOSMPHPOSTGISCOLUMNSINGLE_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnSingle.h>
#include "Column.h"

/// Represents a floating-point single precision column type.
/// This type is based on PostgreSQL type REAL, an inexact,
/// variable-precision numeric type.
/// Storage size of REAL is 4 bytes, with 6 decimal digits precision.
///
class FdoSmPhPostGisColumnSingle :
    public FdoSmPhColumnSingle,
    public FdoSmPhPostGisColumn
{
public:

    /// Create a double precision floating-point column type.
    ///
    FdoSmPhPostGisColumnSingle(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    /// No uninitalization steps are executed.
    virtual ~FdoSmPhPostGisColumnSingle();

    /// Returns RDBI constant for floating point type.
    /// \return
    /// RDBI type of this column type is RDBI_FLOAT.
    ///
    virtual int GetRdbType();

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisColumnSingle> FdoSmPhPostGisColumnSingleP;

#endif // FDOSMPHPOSTGISCOLUMNSINGLE_H