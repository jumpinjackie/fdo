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
#ifndef FDOSMPHPOSTGISCOLUMNDATE_H
#define FDOSMPHPOSTGISCOLUMNDATE_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnDate.h>
#include "Column.h"

/// Represents a character column type for the PostGIS provider.
/// It is based on PostgreSQL type: TIMESTAMP WITHOUT TIME ZONE
///
/// Example: '2004-10-19 10:23:54'
///
/// TODO: mloskot - It should not be required to explicitly
/// include WITHOUT TIME ZONE but better test it.
///
class FdoSmPhPostGisColumnDate :
    public FdoSmPhColumnDate,
    public FdoSmPhPostGisColumn
{
public:

    /// Constructor.
    FdoSmPhPostGisColumnDate(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    /// No uninitalization steps are required.
    ~FdoSmPhPostGisColumnDate();

    /// Returns RDBI constant for byte type.
    /// RDBI type of this column type is RDBI_DATE.
    virtual int GetRdbType();

    /// Returns size of storade of this type.
    /// In PostgreSQL, TIMESTAMP size is 8 bytes.
    /// \return
    /// Size of storage in bytes.
    ///
    virtual FdoInt64 GetDbBinarySize();
    
    
    virtual FdoStringP GetDefaultValueSql();
};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
/// 
typedef FdoPtr<FdoSmPhPostGisColumnDate> FdoSmPhPostGisColumnDateP;

#endif // FDOSMPHPOSTGISCOLUMNDATE_H
