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
#ifndef FDOSMPHPOSTGISCOLUMNBYTE_H
#define FDOSMPHPOSTGISCOLUMNBYTE_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnByte.h>
#include "Column.h"

/// Represents a byte column type for the PostGIS provider.
/// This Boolean type is based on SMALLINT, 2-bytes integer
/// data type from PostgreSQL.
///
/// TODO: mloskot - Do we need to control range 0-255 for this type?
///
class FdoSmPhPostGisColumnByte :
    public FdoSmPhColumnByte,
    public FdoSmPhPostGisColumn
{
public:

    /// Constructor.
    FdoSmPhPostGisColumnByte(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    /// No uninitalization steps are executed.
    virtual ~FdoSmPhPostGisColumnByte();

    /// Returns RDBI constant for byte type.
    /// RDBI type of this column type is RDBI_SHORT.
    virtual int GetRdbType();

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
/// 
typedef FdoPtr<FdoSmPhPostGisColumnByte> FdoSmPhPostGisColumnByteP;

#endif // FDOSMPHPOSTGISCOLUMNBYTE_H