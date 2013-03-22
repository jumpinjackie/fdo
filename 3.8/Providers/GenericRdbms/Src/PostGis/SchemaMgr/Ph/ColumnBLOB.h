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
#ifndef FDOSMPHPOSTGISCOLUMNBLOB_H
#define FDOSMPHPOSTGISCOLUMNBLOB_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnBLOB.h>
#include "Column.h"

/// Represents a type of BLOB column.
/// The class is use by the PostGIS provider.
///
class FdoSmPhPostGisColumnBLOB :
    public FdoSmPhColumnBLOB,
    public FdoSmPhPostGisColumn
{
public:

    /// Constructor.
    FdoSmPhPostGisColumnBLOB(FdoStringP columnName,
        FdoSchemaElementState State,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL);

    /// Returns RDBI constant for BLOB type.
    virtual int GetRdbType();

    /// Returns maximum size of BLOB type.
    virtual FdoInt64 GetDbBinarySize();

    /// Destructor.
    ~FdoSmPhPostGisColumnBLOB();

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
/// 
typedef FdoPtr<FdoSmPhPostGisColumnBLOB> FdoSmPhPostGisColumnBLOBP;

#endif // FDOSMPHPOSTGISCOLUMNBLOB_H
