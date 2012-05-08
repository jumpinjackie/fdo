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
#ifndef FDOSMPHPOSTGISCOLUMNDECIMAL_H
#define FDOSMPHPOSTGISCOLUMNDECIMAL_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnDecimal.h>
#include "Column.h"

/// Represents a numeric column type with fixed size and scale.
/// This numeric type is based on NUMERIC, variable-size storage,
/// data type from PostgreSQL and SQl standard.
/// This type is used by PostGIS provider.
///
class FdoSmPhPostGisColumnDecimal :
    public FdoSmPhColumnDecimal,
    public FdoSmPhPostGisColumn
{
public:

    /// Create a decimal column type.
    ///
    /// \param
    /// length [in] - maximum number of digits, including digits to
    /// the left and right of the decimal point.
    /// scale [in] - maximum number of digits to the right of
    /// the decimal point.
    ///
    FdoSmPhPostGisColumnDecimal(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        bool isNullable,
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    /// No uninitalization steps are executed.
    ~FdoSmPhPostGisColumnDecimal();

    /// Returns RDBI constant for decimal type.
    /// \return
    /// RDBI type of this column type is RDBI_DOUBLE.
    virtual int GetRdbType();

    /// Checks if the numeric type has maximum number of digits limit.
    /// NUMERIC type in PostgreSQL does have maximum number of digits,
    /// up to the implementation limit on precision.
    ///
    /// \return
    /// For PostGIS, this function always returns true.
    ///
    virtual bool HasMaxLength() const;

    /// Gets maximum number of digits for PostGIS decimal type.
    /// In PostgreSQL, numeric type can store numbers with
    /// up to 1000 digits of precision, where precision of a numeric
    /// is the total count of significant digits in the whole number,
    /// on both sides of the decimal point.
    ///
    /// \return
    /// Maximum number of digits in numeric type.
    ///
    virtual int GetMaxLength() const;

    /// Checks if the numeric type has maximum number limit of digits
    /// in scale fraction.
    ///
    /// \todo TODO: mloskot - AFAIK PostgreSQL does not limit
    ///             number if digits of scale.
    ///
    virtual bool HasMaxScale() const;

    /// 
    virtual int GetMaxScale() const;

    /// Calculates size in datastore based on given number of digits.
    /// \return
    /// 
    virtual FdoInt64 GetDbBinarySize();

protected:

    /// Calculates size in datastore based on given number of digits.
    /// \param
    /// digits [in] - 
    /// \return
    /// 
    FdoInt64 Digits2DbBinarySize(FdoInt32 digits);

    int mScale;

private:

    /// Constants define limits of NUMERIC type.
    ///
    enum Limits
    {
        eMaxPrecision = 1000,
        eMaxScale     = 1000
    };

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisColumnDecimal> FdoSmPhPostGisColumnDecimalP;

#endif // FDOSMPHPOSTGISCOLUMNDECIMAL_H
