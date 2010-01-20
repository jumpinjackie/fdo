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
#ifndef FDOSMPHPOSTGISCOLUMNCHAR_H
#define FDOSMPHPOSTGISCOLUMNCHAR_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnChar.h>
#include "Column.h"

/// Represents a character column type for the PostGIS provider.
/// This type is based on PostgreSQL types:
/// variable-length with limit: character varying(n), varchar(n)
/// or variable unlimited length: text.
/// The final type selection is based on lenght limit.
/// If requested length is <= mVarcharMaxLen, then varchar is used,
/// otherwise text of unlimited lentgh.
///
/// NOTE: The ColumnChar does not support following PostgreSQL extension:
/// If character varying is used without length specifier,
/// the type accepts strings of any size. 
///
/// TODO: mloskot - Verify the message written above in practice.
///
class FdoSmPhPostGisColumnChar :
    public FdoSmPhColumnChar,
    public FdoSmPhPostGisColumn
{
public:
    
    /// Constructor creates the column type.
    ///
    /// \param
    /// length [in] - maximum number of characters.
    ///
    FdoSmPhPostGisColumnChar(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        int length,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    /// No uninitalization steps are executed.
    virtual ~FdoSmPhPostGisColumnChar();

    /// Returns RDBI constant for byte type.
    /// RDBI type of this column type is RDBI_STRING.
    virtual int GetRdbType();

    /// Get SQL sub-clause for column type indicator
    virtual FdoStringP GetTypeSql();

    // TODO: mloskot - Change following values to reflect PostgreSQL types.
    //       Remove unused values, ie. mTextMaxLen

    static const FdoInt64 mVarcharMaxLen = 65535;
    static const FdoInt64 mTextMaxLen = 65535;
    static const FdoInt64 mMediumTextMaxLen = 16777215;
};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
/// 
typedef FdoPtr<FdoSmPhPostGisColumnChar> FdoSmPhPostGisColumnCharP;

#endif // FDOSMPHPOSTGISCOLUMNCHAR_H
