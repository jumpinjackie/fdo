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
#ifndef FDOSMPHPOSTGISCOLUMNUNKNOWN_H
#define FDOSMPHPOSTGISCOLUMNUNKNOWN_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnUnknown.h>
#include "Column.h"

/// Represents a type of column not known to the PostGIS provider.
/// Therefore it is handled in a generic manner, meaning
/// that the caller must provide the native RDBMS type for this column.
///
/// The caller may also provide the length and scale if relevant.
/// These are used to format the SQL clause for adding the column if new.
/// Therefore, this column type allows callers to created columns of types
/// not explicitly handled by the PostGIS provider.
///
class FdoSmPhPostGisColumnUnknown :
    public FdoSmPhColumnUnknown,
    public FdoSmPhPostGisColumn
{
public:

    /// Create an unknown column type.
    ///
    /// \param
    /// length [in] - maximum number of digits, including
    /// digits to the left and right of the decimal point.
    /// \param
    /// scale [in] - maximum number of digits to the right
    /// of the decimal point.
    ///
    FdoSmPhPostGisColumnUnknown(FdoStringP columnName,
        FdoStringP typeName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    /// Currently, no destruction steps for this class are required.
    ~FdoSmPhPostGisColumnUnknown();

    /// Returns RDBI type for unknown column type.
    /// Unknown column type is interpreted as a string,
    /// as a most universal representation.
    virtual int GetRdbType();

protected:

    /// Default constructor.
    /// Unused constructor needed only to build on Linux.
    FdoSmPhPostGisColumnUnknown();

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisColumnUnknown> FdoSmPhPostGisColumnUnknownP;

#endif // FDOSMPHPOSTGISCOLUMNUNKNOWN_H