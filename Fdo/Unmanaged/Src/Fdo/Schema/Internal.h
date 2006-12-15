#ifndef FDO_SCHEMA_INTERNAL
#define FDO_SCHEMA_INTERNAL

//
// Copyright (C) 2004-2006  Autodesk, Inc.
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

#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>
#include <Fdo/Schema/PropertyValueConstraintList.h>
#include <Fdo/Schema/PropertyValueConstraintRange.h>

// Temporary wrapper classes that extend the functionality of the schema classes.
// These functions should eventually be migrated into the API.
// NOTE: Each class has a schema class member, but this member is not ref-counted
// Therefore these wrapper classes must not be kept around but must be destroyed before their members.
class FdoInternalPropertyValueConstraint : public FdoIDisposable
{
public:
    static FdoInternalPropertyValueConstraint* Create( FdoPropertyValueConstraint* constraint );

    // \brief
    // Checks if a constraint domain is contained within another
    // 
    // \param value 
    // The Property Value constraint to compare with
    // 
    // \return
    // Returns true if this constraint's domain is a superset of the domain
    // for pConstraint (all values that do not violate pConstraint also do not
    // violate this constraint).
    virtual bool Contains( FdoPropertyValueConstraint* pConstraint ) = 0;

protected:
    virtual void Dispose()
    {
        delete this;
    }
};

class FdoInternalPropertyValueConstraintList : public FdoInternalPropertyValueConstraint
{
public:
    FdoInternalPropertyValueConstraintList( FdoPropertyValueConstraintList* constraint );

    virtual ~FdoInternalPropertyValueConstraintList();

    virtual bool Contains( FdoPropertyValueConstraint* pConstraint );

    FdoPropertyValueConstraintList* operator->()
    {
        return mConstraint;
    }

private:
    FdoPropertyValueConstraintList* mConstraint;
};

class FdoInternalPropertyValueConstraintRange : public FdoInternalPropertyValueConstraint
{
public:
    FdoInternalPropertyValueConstraintRange( FdoPropertyValueConstraintRange* constraint );

    virtual ~FdoInternalPropertyValueConstraintRange();

    virtual bool Contains( FdoPropertyValueConstraint* pConstraint );

    FdoPropertyValueConstraintRange* operator->()
    {
        return mConstraint;
    }

private:
    // \brief
    // Compares the ends of two range constraints.
    // 
    // \param myInclusive 
    // First constraint's inclusivity setting. When the first and second values are equal, the 
    // inclusive settings are then checked:
    //    When isMax is true, inclusive values are greater than exclusive values
    //    When isMax is false, inclusive values are less than exclusive values
    // There is currently a limitation in that, for integral types, inclusive values are not checked
    // when the values differ by 1. Therefore, values are sometimes reported as not equal when 
    // they are equal.
    // 
    // \param myValue 
    // Value from the first constraint
    // 
    // \param myInclusive 
    // Second constraint's inclusivity setting
    //
    // \param theirValue 
    // Value from the second constraint
    //
    // \param isMax
    // Determines how null and not null values are compared:
    //   true: values are maximum end of ranges. NULL values are considered to be greater than not null values.
    //   false: they are the minimum ends. NULL values are considered to be less than not null values.
    //
    // \return
    // Returns:
    //     FdoCompareType_Equal when first and second values are equal
    //     FdoCompareType_Greater when the first value is greater than the second value
    //     FdoCompareType_Less when the first value is less than the second value
    //     FdoCompareType_Undefined when these two values have incompatible types.

    FdoCompareType CompareEnd( FdoBoolean myInclusive, FdoPtr<FdoDataValue> myValue, FdoBoolean theirInclusive, FdoPtr<FdoDataValue> theirValue, FdoBoolean isMax );
    FdoPropertyValueConstraintRange* mConstraint;
};


#endif


