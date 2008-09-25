#ifndef FDOSMPHFIELD_H
#define FDOSMPHFIELD_H		1
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

#include <Sm/Ph/Column.h>
#include <Sm/Ph/NullIndicator.h>

class FdoSmPhRow;

// Describes a field for selecting or updating a column. Field
// represents a particular row and column in a database object.
class FdoSmPhField : public FdoSmPhSchemaElement
{
public:

    /// Creates a new Field Definition for an existing column.
	//
    /// Parameters:
    /// 	pRow: the row containing the field.
    /// 	fieldName: name of the column that this field represents. This must
    ///      be an existing column in the underlying database object for
    ///      pRow.
    /// 	column:  the column that this field represents.If NULL then
    ///       an existing column, with same name as the field is looked up.
    /// 	defaultValue: default value for inserting into the column
    /// 	bCanBind: true: this field can also serve as a bind variable.
    FdoSmPhField( 
		FdoSmPhRow* pRow, 
        FdoStringP fieldName,
		FdoSmPhColumnP column = (FdoSmPhColumn*) NULL, 
        FdoStringP defaultValue = L"",
        bool bCanBind = true
	);

	~FdoSmPhField(void);

    /// Gets the row that contains this field
    FdoPtr<FdoSmPhRow> GetRow() const;

    /// Gets the definition of the field's column
	//
    /// Parameters:
    /// 	bCreate: 
    ///          true: create the column if it doesn't exist.
    ///              column is only created if constructor's 
    ///              pCreateColumn was not NULL.
    FdoSmPhColumnP GetColumn();

    /// Get the current value of the field.
    FdoStringP GetFieldValue() const;

    /// Get the default value of the field.
    FdoStringP GetDefaultValue() const;

    /// returns true if the field value has been modified
    /// (SetFieldValue has been called).
    bool GetIsModified() const
    {
        return mbIsModified;
    }

    /// returns the fully qualified name in the form
    /// [table_name].[field_name]
    FdoStringP GetQName() const;

    /// Sets the field's value
    void SetFieldValue(FdoStringP sValue );

    /// Resets the field's value back to the default. Subsequent
    /// calls to GetIsModified() return false until the next
    /// call to SetFieldValue().
    void Clear();
   
    /// returns the appropriate identifier for including this
    /// field in a select list.
    FdoStringP GetSelect();

    /// returns the appropriate identifier for including this
    /// field in a column list for an insert or update command.
    FdoStringP GetUpdCol();

    /// returns the appropriate format for including this field
    /// in the value list for an insert or update command.
    FdoStringP GetUpdVal();

    bool GetCanBind()
    {
        return mbCanBind;
    }

    /// Gets a buffer for holding a bind variable for this field
    const char* GetBindString();

    /// Get the maximum size, in bytes, of this field's bind variable
    int GetBindSize()
    {
        GetBindString();
        return mpBindSize;
    }

    /// Get the null indicator for this field's bind variable
    FdoSmPhNullIndicatorP GetNullInd()
    {
        if ( !mNullInd ) 
            mNullInd = GetManager()->CreateNullIndicator();

        return mNullInd;
    }

protected:
    /// unused constructor only for building on Linux
    FdoSmPhField() {}

private:
/* TODO:
    /// returns the SQL format of the field value
    FdoStringP FormatValue( FdoStringP val, FdoStringP valType ) const;
*/
    /// Copy the current field value into the bind buffer.
    void BindValue( );

    /// Current column spec for this field
    FdoSmPhColumnP mColumn;

    /// Current value and default value, and modified indicator
	FdoStringP mFieldValue;
	FdoStringP mDefaultValue;

    bool mbIsModified;

    /// members for binding to a statement.
    bool mbCanBind;
    FdoSmPhNullIndicatorP mNullInd;            // is value null
    char* mpBindString;        // the bind address.
    int   mpBindSize;

};

typedef FdoPtr<FdoSmPhField> FdoSmPhFieldP;

#endif


