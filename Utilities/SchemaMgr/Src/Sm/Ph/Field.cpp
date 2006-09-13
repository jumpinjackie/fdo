/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"
#include <Sm/Ph/Row.h>
#include <Sm/Error.h>


FdoSmPhField::FdoSmPhField(
	FdoSmPhRow* pRow, 
    FdoStringP fieldName,
	FdoSmPhColumnP column, 
    FdoStringP defaultValue,
    bool bCanBind
): 
	FdoSmPhSchemaElement( 
        fieldName, 
		L"", 
        (FdoSmPhMgr*) NULL,
		pRow 
    ),
    mColumn(column),
    mDefaultValue( defaultValue ),
    mbIsModified(false),
    mbCanBind(bCanBind),
    mpBindString(NULL),
	mNullInd(false)
{
    // Auto-add the field to its parent row
	FdoSmPhFieldsP pFields = pRow->GetFields();
    pFields->Add(this);
}

FdoSmPhField::~FdoSmPhField(void)
{
    if ( mpBindString )
        delete[] mpBindString;
}

FdoSmPhRowP FdoSmPhField::GetRow() const
{
	return ( FDO_SAFE_ADDREF((FdoSmPhRow*) GetParent()) );
}

FdoSmPhColumnP FdoSmPhField::GetColumn()
{
    if ( !mColumn ) {
        FdoSmPhRowP row = GetRow();

        if ( row ) {
            // Find the column
            FdoSmPhDbObject* pDbObject = row->GetDbObject();

            if ( pDbObject ) {
                // Try case-sensitive name match first
                mColumn = pDbObject->GetColumns()->FindItem(GetName());
                if ( !mColumn ) 
                    // No match, so try to match to column with default case name.
                    mColumn = pDbObject->GetColumns()->FindItem( GetManager()->GetDcColumnName(GetName()) );
            }
        }
    }
        
    return mColumn;
}

FdoStringP FdoSmPhField::GetFieldValue()const
{
	return mbIsModified ? mFieldValue : mDefaultValue;
}

FdoStringP FdoSmPhField::GetDefaultValue()const
{
	return mDefaultValue;
}

FdoStringP FdoSmPhField::GetQName() const
{
    FdoStringP          qName;
    FdoSmPhRowP         row = GetRow();

    if ( row ) 
        qName = FdoStringP( row->GetQName() ) + L".";

    qName += GetName();

    return qName;
}

void FdoSmPhField::SetFieldValue(FdoStringP sValue )
{
    mFieldValue = sValue;
    mbIsModified = true;
    BindValue();
}

void FdoSmPhField::Clear()
{
    mFieldValue = L"";
    mbIsModified = false;
    BindValue();
}
    

FdoStringP FdoSmPhField::GetSelect()
{
    FdoStringP          selectSyntax;
    FdoSmPhColumnP   pColumn = GetColumn();

    if ( pColumn->GetExists() ) {
        // Column exists

        if ( mDefaultValue.GetLength() == 0 ) {
            // no default value, just select by qualified name. 

            selectSyntax = GetQName();

        }
        else { 
            // default value, generate syntax for a function that
            // returns the default value when underlying column
            // value is null
            selectSyntax = GetManager()->FormatDefaultedField( 
                GetName(), 
                pColumn->GetName(), 
                mDefaultValue, 
                pColumn->GetType() 
            );
        }
    }
    else {
        // column does not exist.
        // return the default value as a literal plus an alias (the field name).
        selectSyntax = FdoStringP(L"(") + 
            GetManager()->FormatSQLVal(mDefaultValue, pColumn->GetType()) + L") \"" + GetName() + L"\"";
    }

    return selectSyntax;
}

FdoStringP FdoSmPhField::GetUpdCol()
{
    FdoStringP colSyntax;

    if ( GetColumn() ) {
        colSyntax = GetName();
    }

    return colSyntax;
}

FdoStringP FdoSmPhField::GetUpdVal()
{
    FdoStringP valSyntax;
    FdoSmPhColumnP pColumn = GetColumn();

    if ( pColumn ) {
        FdoStringP val = GetFieldValue();

        valSyntax = pColumn->GetValueSql( val );
    }

    return valSyntax;
}

const char* FdoSmPhField::GetBindString()
{
    if ( !mpBindString ) {
        // Create the string with size appropriate to this column type.
        FdoSmPhColumnP   column = GetColumn();

        // Cannot bind if column doesn't exist.
        if ( !column ) 
            throw FdoSchemaException::Create (
                FdoSmError::NLSGetMessage(
                    FDO_NLSID(FDOSM_310), 
		            (FdoString*) GetQName() 
                )
            );

        mpBindSize = column->GetBindSize();

        // Temporary, while binds are all strings.
        if ( mpBindSize < 50 ) 
            mpBindSize = 50;

        mpBindString = new char[mpBindSize * (GetManager()->IsRdbUnicode() ? sizeof(wchar_t) : 6)];

        BindValue();
    }

    return mpBindString;
}

void FdoSmPhField::BindValue( )
{
    if ( mpBindString ) {
        FdoStringP          fieldValue = GetFieldValue();
     
        // Trap bind string overflow
        if ( ((int)fieldValue.GetLength()) >= mpBindSize ) 
            throw FdoSchemaException::Create (
                FdoSmError::NLSGetMessage(
                    FDO_NLSID(FDOSM_311), 
		            (FdoString*) GetQName(), 
                    (FdoString*) fieldValue
                )
	        );

        // Null indicators are provider specific. They will be properly set in GdbiCommands::Bind()
        if ( fieldValue.GetLength() == 0 ) {
            // set bind value to null
            mNullInd = true; 
            mpBindString[0] = 0;
        }
        else {
            // set bind value to field value.
            mNullInd = false; 
            if ( GetManager()->IsRdbUnicode() )
                wcscpy( (wchar_t*) mpBindString, fieldValue );
            else
                strcpy( mpBindString, fieldValue );
        }
    }
}
