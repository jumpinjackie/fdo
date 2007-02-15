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
#include <Sm/Ph/Rd/PropertyReader.h>
#include <Sm/Ph/Table.h>
#include <Sm/Ph/PropertyReader.h>


bool FdoSmPhRdPropertyReader::IsOrdinate(FdoSmPhColumnP column)
{
    bool isOrdinate = false;

    FdoStringP name = column->GetName();

    if (name.ICompare(L"X")==0 ||
        name.ICompare(L"Y")==0 ||
        name.ICompare(L"Z")==0)
    {
        isOrdinate = true;
    }

    return isOrdinate;
}

FdoSmPhRdPropertyReader::FdoSmPhRdPropertyReader(FdoSmPhDbObjectP dbObject, FdoSmPhMgrP mgr) :
	FdoSmPhReader(mgr, MakeRows(mgr)),
    mFieldIdx(-1),
    mFkeyIdx(-1),
    mDbObject(dbObject),
    m_IsGeometryFromOrdinatesWanted(mgr->IsGeometryFromOrdinatesWanted()),
    mFkeyCount(0)
{
    mUsedNames = FdoDictionary::Create();

    if ( !mDbObject ) {
        // We're done right away if the database object does not exist.
        SetEOF(true);
    }
    else {
        // Count the number of foreign keys in the database object.
        mFkeyCount = mDbObject->RefFkeysUp()->GetCount();

        // Determine the identity properties (columns)
        ResolveIdentity();
    }
}


FdoSmPhRdPropertyReader::~FdoSmPhRdPropertyReader(void)
{
}

bool FdoSmPhRdPropertyReader::ReadNext()
{
    FdoSmPhRowsP rows = GetRows();
    FdoSmPhRowP row;
    bool rowFound = false;

    if ( rows ) {
        // Read each non-foreign column and foreign key.
        // Keep going until the next valid property is generated or
        // we run out of columns and foreign keys.
        while ( !IsEOF() && !rowFound ) {
            // Check if EOF (out of columns and foreign keys)
            if ( !IsEOF() ) {
                mFieldIdx++;
                if ( mFieldIdx >= mDbObject->RefColumns()->GetCount() ) {
                    mFkeyIdx++;
                    if ( mFkeyIdx >= mFkeyCount) 
                        SetEOF(true);
                }
            }

            if ( !IsEOF() ) {
                if ( mFieldIdx < mDbObject->RefColumns()->GetCount() ) {
                    // Still reading columns, turn current column into a property
					
					FdoSmPhColumnsP columns = mDbObject->GetColumns();
                    FdoSmPhColumnP column = columns->GetItem(mFieldIdx);

                    row = rows ? rows->GetItem(0) : NULL;

                    // Reverse engineering columns with unknown type is not supported.
                    // If required, a property for this type of column must be 
                    // specified in a config document.
                    if ( row && CheckColumn(column) ) {
						FdoSmPhFieldsP	pFields = row->GetFields();

                        // Set the property attributes from the column attributes

                        FdoStringP propName = UniquePropName( column->GetBestPropertyName(), L"");
                        FdoSmPhFieldP field = pFields->GetItem(L"attributename");
                        field->SetFieldValue( propName );
                        AddUsedName( propName );

                        field = pFields->GetItem(L"columnname");
                        field->SetFieldValue( column->GetName() );

                        field = pFields->GetItem(L"tablename");
                        field->SetFieldValue( mDbObject->GetName() );

                        field = pFields->GetItem(L"columntype");
                        field->SetFieldValue( column->GetTypeName() );

                        field = pFields->GetItem(L"attributetype");
                        field->SetFieldValue( column ? column->GetBestFdoType() : L"" );

                        field = pFields->GetItem(L"columnsize");
                        field->SetFieldValue( FdoStringP::Format(L"%d", column ? column->GetLength() : 0) );

                        field = pFields->GetItem(L"columnscale");
                        field->SetFieldValue( FdoStringP::Format(L"%d", column ? column->GetScale() : 0) );

                        field = pFields->GetItem(L"isnullable");
                        field->SetFieldValue( column->GetNullable() ? L"1" : L"0" );

                        field = pFields->GetItem(L"isfixedcolumn");
                        field->SetFieldValue( L"1" );

                        field = pFields->GetItem(L"idposition");
                        field->SetFieldValue( GetIdPosn(column->GetName()) );

                        field = pFields->GetItem(L"isfeatid");
                        field->SetFieldValue( L"0" );

                        field = pFields->GetItem(L"isautogenerated");
                        field->SetFieldValue( column->GetAutoincrement() ? L"1" : L"0" );

                        FdoSmPhColumnGeomP geomColumn = column->SmartCast<FdoSmPhColumnGeom>();

                        if ( geomColumn ) {
                            SetString( L"", L"hasmeasure", geomColumn->GetHasMeasure() ? L"1" : L"0" );
                            SetString( L"", L"haselevation", geomColumn->GetHasElevation() ? L"1" : L"0" );
                            SetString( L"", L"geometrytype", geomColumn->GetBestFdoGeometryType());
                        }
                        else {
                            SetString( L"", L"hasmeasure", L"0" );
                            SetString( L"", L"haselevation", L"0" );
                            SetString( L"", L"geometrytype", L"0" );
                        }
                        rowFound = true;
                    }
                }
                else {

                    // No more columns; get the next foreign key, and turn it into 
                    // an association property.
					FdoSmPhFkeysP	pFkeys = mDbObject->GetFkeysUp();
                    FdoSmPhFkeyP pFkey = pFkeys->GetItem(mFkeyIdx);
                    const FdoSmPhTable* pPkeyTable = pFkey->RefPkeyTable();

                    row = rows ? rows->GetItem(0) : NULL;

                    // Skip foreign keys with non-existent pkey table or pkey
                    // table in a different owner. Reverse-engineering schemas
                    // from multiple owners not supported.
                    if ( row && pPkeyTable && 
                         ( pPkeyTable->GetParent()->GetQName() == mDbObject->GetParent()->GetQName() ) &&
                         CheckFkey(pFkey) 
                    ) {
						FdoSmPhFieldsP	pFields = row->GetFields();

                        // Set property attributes from foreign key attributes.
                        FdoStringP assocPropName = UniquePropName( pFkey->GetBestPropertyName(), L"Assoc" );
                        FdoSmPhFieldP field = pFields->GetItem(L"attributename");
                        field->SetFieldValue( assocPropName );
                        AddUsedName( assocPropName );

                        field = pFields->GetItem(L"columnname");
                        field->SetFieldValue( L"" );

                        field = pFields->GetItem(L"tablename");
                        field->SetFieldValue( mDbObject->GetName() );

                        field = pFields->GetItem(L"columntype");
                        field->SetFieldValue( L"Association" );

                        field = pFields->GetItem(L"columnsize");
                        field->SetFieldValue( L"0" );

                        field = pFields->GetItem(L"columnscale");
                        field->SetFieldValue( L"0" );

                        field = pFields->GetItem(L"isnullable");
                        field->SetFieldValue( L"0" );

                        field = pFields->GetItem(L"isfixedcolumn");
                        field->SetFieldValue( L"1" );

                        field = pFields->GetItem(L"idposition");
                        field->SetFieldValue( L"0" );

                        // Generate attribute type class from primary table and schema name.
                        if ( pPkeyTable ) {
                            field = pFields->GetItem(L"attributetype");
                            field->SetFieldValue( 
                                pPkeyTable->GetBestSchemaName() +
                                L":" + 
                                pPkeyTable->GetBestClassName()
                            );

                            rowFound = true;
                        }
                    }
                }

                row = (rows && (rows->GetCount() > 0)) ? rows->GetItem(1) : NULL;

                if ( row ) {
					FdoSmPhFieldsP	pFields = row->GetFields();
                    FdoSmPhFieldP field = pFields->GetItem(L"classname");
                    field->SetFieldValue( mDbObject->GetBestClassName() );
                }

                SetBOF(false);
            }
        }
    }
    else {
        SetEOF(true);
    }

	return(!IsEOF());
}

FdoSmPhRowsP FdoSmPhRdPropertyReader::MakeRows( FdoSmPhMgrP mgr )
{
    FdoSmPhRowsP rows = FdoSmPhPropertyReader::MakeRows( mgr );

    return rows;
}

void FdoSmPhRdPropertyReader::ResolveIdentity()
{
    mIdCols = mDbObject->GetBestIdentity();
}

FdoStringP FdoSmPhRdPropertyReader::GetIdPosn( FdoStringP colName )
{
    const FdoSmPhColumnCollection* pCols = NULL;

    int idx;

    if ( mIdCols ) {
        for ( idx = 0; idx < mIdCols->GetCount(); idx++ ) {
			FdoSmPhColumnP	column = mIdCols->GetItem(idx);
            if ( colName == column->GetName() )
                return( FdoStringP::Format(L"%d", idx+1) );
        }
    }

    return( L"0" );
}

bool FdoSmPhRdPropertyReader::InFkey( FdoSmPhColumnP column )
{
    FdoStringP colName = column->GetName();
    int idx;

    // Special case: do not weed out identity columns. Data properties need to 
    // be generated for them.
    if ( FdoSmPhColumnP(FdoSmPhColumnsP(mDbObject->GetPkeyColumns())->FindItem( colName )) ) 
        return false;

    // Check if the column is in a foreign key.
    FdoSmPhFkeysP	pFkeys = mDbObject->GetFkeysUp();
	for ( idx = 0; idx < pFkeys->GetCount(); idx++ ) {
        FdoSmPhFkeyP fkey = pFkeys->GetItem(idx);
    	FdoSmPhColumnsP	columns = fkey->GetFkeyColumns();
        if ( FdoSmPhColumnP(columns->FindItem(colName)) ) 
            return true;
    }

    return false;
}

bool FdoSmPhRdPropertyReader::CheckFkey( FdoSmPhFkeyP fkey )
{
    FdoInt32 idx;
    FdoSmPhColumnP fkeyColumn;
    FdoSmPhColumnP pkeyColumn;

    FdoSmPhColumnsP fkeyColumns = fkey->GetFkeyColumns();
    FdoSmPhColumnsP pkeyColumns = fkey->GetPkeyColumns();

    if ( fkeyColumns->GetCount() != pkeyColumns->GetCount() ) 
        // This would be a malformed foreign key so skip it.
        return false;

    for ( idx = 0; idx < fkeyColumns->GetCount(); idx++ ) {
        fkeyColumn = fkeyColumns->GetItem( idx );

        if ( !CheckColumn(fkeyColumn) )
            // Skipping fkey column so skip entire foreign key
            return false;

        pkeyColumn = pkeyColumns->GetItem( idx );

        if ( !CheckColumn(pkeyColumn) )
            // Skipping pkey column so skip entire foreign key
            return false;

        if ( fkeyColumn->GetType() != pkeyColumn->GetType() ) 
            // FDO does not support association properties whose corresponding
            // identity property pairs aren't of the same type. This foreign key
            // generates such an association property so skip it. 
            return false;

        if ( fkeyColumn->GetType() == FdoSmPhColType_Geom ) 
            // FDO Association property does not support geometric properties as identity properties
            return false;

        if ( fkeyColumn->GetAutoincrement() ) 
            // Association reverse identity properties cannot be autogenerated so skip
            // foreign keys with autogenerated fkey columns.
            return false;

    }

    return true;
}

bool FdoSmPhRdPropertyReader::CheckColumn( FdoSmPhColumnP column )
{
    // We'll skip any ordinates, and let the caller figure out a geometric
    // property from such columns.
    bool isOrdinate =
        m_IsGeometryFromOrdinatesWanted &&
        column &&
        IsOrdinate(column);

    // Reverse engineering columns with unknown type is not supported.
    // If required, a property for this type of column must be 
    // specified in a config document.
    return( !isOrdinate && column && (column->GetType() != FdoSmPhColType_Unknown) );
}

FdoStringP FdoSmPhRdPropertyReader::UniquePropName( FdoStringP origName, FdoStringP prefix )
{
    FdoStringP uniqueName = origName;
    FdoInt32 idx = (prefix == L"") ? 1: 0;
    FdoBoolean done = false;

    while ( !done ) {
        if ( mUsedNames->Contains(uniqueName) ) {
            // Name not unique, adjust it to be unique.
            // First try pre-pending prefix, if specified.
            // After that, add a number to the end. Start
            // at 1 and keep incrementing until unique name
            // generated.
            uniqueName = FdoStringP::Format( 
                L"%ls%ls%ls",
                (FdoString*) prefix,
                (FdoString*) origName,
                (idx == 0) ? L"" : (FdoString*) FdoStringP::Format( L"%d", idx )
            );

            idx++;
        }
        else {
            done = true;
        }
    }

    return uniqueName;
}

void FdoSmPhRdPropertyReader::AddUsedName( FdoStringP propName )
{
    FdoDictionaryElementP elem = FdoDictionaryElement::Create( propName, L"" );
    mUsedNames->Add( elem );
}
