#ifndef FDOSMPHCOLUMN_H
#define FDOSMPHCOLUMN_H		1
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

#include <Sm/Ph/DbElement.h>
#include <Sm/Ph/Coltype.h>

class FdoSmPhDbObject;

// Describes a column in the database. Usually a column for a
// DataProperty in a Feature Schema.
class FdoSmPhColumn : public FdoSmPhDbElement
{
public:

    /// Returns the database object that contains this column.
    FdoPtr<FdoSmPhDbObject> GetContainingDbObject();

    /// Returns true if this column exists in the database.
    FdoBoolean GetExists() const;

    /// Get the fully qualified name for this column.
    FdoStringP GetQName() const;

    /// Gets the root column name. When this column is in a view
    /// on a foreign table, root column is the corresponding column
    /// in this table.
    /// returns L"" if column is not in such a view.
    FdoStringP GetRootName() const;
    // returns root column name formatted for inclusion in a SQL statement.
    FdoStringP GetDbRootName();

    /// Gets the native RDBMS data type of this property.
    FdoStringP GetTypeName() const;

    /// Returns a bool value that indicates if the values
    /// for this column are generated from a sequence.
    bool GetAutoincrement() const;

    /// Gets the length of a string column or total number
    /// of digits ( right or left of the decimal point )
    /// for a NUMBER column.
    virtual int GetLength() const;

    /// The following Min/Max length functions can be overridden to provide RDBMS-specific
    /// settings.

    /// Returns true if this type of column has a minimum length
    virtual bool HasMinLength() const;
    /// Returns the minimum length for this type of column
    /// Applicable only when HasMinLength() returns true;
    virtual int GetMinLength() const;
    /// Returns true if this type of column has a maximum length
    virtual bool HasMaxLength() const;
    /// Returns the maximum length for this type of column
    /// Applicable only when HasMaxLength() returns true;
    virtual int GetMaxLength() const;

    /// Gets the scale (number of digits to the right of the decimal point) of a
    /// decimal column. 0 if the column represents an integer.
    virtual int GetScale() const;

    /// The following Min/Max scale functions can be overridden to provide RDBMS-specific
    /// settings.

    /// Returns true if this type of column has a minimum scale
    virtual bool HasMinScale() const;
    /// Returns the minimum scale for this type of column
    /// Applicable only when HasMinScale() returns true;
    virtual int GetMinScale() const;
    /// Returns true if this type of column has a maximum scale
    virtual bool HasMaxScale() const;
    /// Returns the maximum scale for this type of column
    /// Applicable only when HasMaxScale() returns true;
    virtual int GetMaxScale() const;

    /// Returns true if the column can have "null" as its value.
    bool GetNullable() const;

    /// returns DBI-format column dimensionality.
    /// See inc/dbi/coord_ty.h for the list of values that can be returned.
    /// If the column is not a geometric column then -1 (no dimensionality)
    /// is always returned.
    /// Note: this function works only for new columns. For existing columns
    /// it always returns -1, regardless of the column dimensionality.

    int GetDimensionality() const;

    void SetDimensionality(int value);

    virtual FdoSmPhColType GetType() = 0;

    virtual FdoStringP GetBestFdoType()
    {
        return L"";
    }

    /// Default size for bind variables for this column
    virtual int GetBindSize()
    {
        // By default it is the column's binary size
        return GetBinarySize();
    }
   
    /// Default size for when this column is defined as
    /// a retrieval field in a query.
    virtual int GetDefineSize()
    {
        // By default it is the column's binary size
        return GetBinarySize();
    }

    /// Default binary size. Maximum size of values in
    /// memory for this column. Measured in bytes.
    virtual int GetBinarySize()
    {
    /// pick a large size to be safe.
    /// derived types must override when larger size required,
        return 50;
    }

    /// Default binary size. Maximum space required for 
    /// column values in the DBMS.
    virtual FdoInt64 GetDbBinarySize()
    {
        // Same as size in memory by default.
        return GetBinarySize();
    }


    /// Returns the RDBI type for this column
    virtual int GetRdbType() = 0;

    /// Set the modification state of this column.
	virtual void SetElementState(FdoSchemaElementState elementState);

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Sets the root column name.
    /// TODO: instead of having a setter, figure out how to 
    /// retrieve the root column name from the RDBMS.
    void SetRootName( FdoStringP rootName );

    // Reverse-engineer a property name from this column's name.
    virtual FdoStringP GetBestPropertyName() const;

    /// Get SQL clause for adding a column
    virtual FdoStringP GetAddSql();

    /// Get SQL sub-clause for column type indicator
    virtual FdoStringP GetTypeSql();

    /// Get SQL sub-clause for column nullability
    virtual FdoStringP GetNullabilitySql();

    /// Get SQL for IDENTITY/AUTOINCREMENT tag
	virtual FdoStringP GetAutoincrementSql();

	void SetAutoincrement(bool value) {	mbAutoIncrement = value; }

    virtual FdoStringP GetNullValueSql();

    virtual FdoStringP GetValueSql( FdoStringP val );

    /// Returns true if this column has any not null values.
    virtual bool GetHasValues();

    // Returns true if this column has the same definition (type,nullability)
    // as the given column.
    // Subclasses must extend this function if more definition attributes need to be checked.
    virtual bool DefinitionEquals( FdoPtr<FdoSmPhColumn> otherColumn );

// TODO:
//    void ChangeAssociatedSpatialContext();

    /// Serialize the column to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

protected:
    /// Creates a new Column Definition.
	//
    /// Parameters:
    /// 	columnName: the name of the column
    ///      typeName: provider-specific type name passed down by implementation class.
    ///      elementState: indicates whether column is new,
    /// 	parentObject: the containing database object.
    /// 	bNullable: true if column is nullable.
    ///      bFeatId: true if this is a FeatId column.
    ///      RootColumnName: set only when column is in a view on 
    ///          a foreign table. Indicates the corresponding
    ///          column in the foreign table.
    FdoSmPhColumn(
		FdoStringP columnName, 
		FdoStringP typeName, 
		FdoSchemaElementState elementState,
		FdoSmPhDbObject* parentObject,
		bool bNullable,
        FdoStringP rootColumnName = L""
	);

    /// NOTE: The following constructor is to satisfy the compiler, and should never actually be called:
    FdoSmPhColumn() {}

	virtual ~FdoSmPhColumn(void);

    /// Records this column as having been updated so that modification can be 
    /// undone on rollback.
    virtual void UpdRollbackCache();

    /// Various functions for posting changes to the current connection.
    /// These do nothing because the actual updates are handled by the containing
    /// table or view.
    virtual bool Add() {return true;}
    virtual bool Modify() {return true;}
    virtual bool Delete() {return true;}

private:

	void AddColHasRowsDelError();

    FdoSmPhDbObject* mDbObject;

    FdoStringP mQName;
   
    FdoStringP mRootName;

    FdoStringP mTypeName;

	bool mbNullable;

    bool mbReadOnly;

	bool mbAutoIncrement;

    int miDimensionality;
};

typedef FdoPtr<FdoSmPhColumn> FdoSmPhColumnP;

#endif


