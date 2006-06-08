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
#include <Sm/Ph/Column.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/DbObject.h>
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Error.h>

FdoSmPhColumn::FdoSmPhColumn(
	FdoStringP columnName, 
	FdoStringP typeName, 
	FdoSchemaElementState elementState,
	FdoSmPhDbObject* parentObject,
	bool bNullable, 
    FdoStringP rootColumnName
): 
    FdoSmPhDbElement( 
        columnName, 
        (FdoSmPhMgr*) NULL,
   		(const FdoSmPhSchemaElement*) parentObject,
        elementState
    ),
    mDbObject(parentObject),
    mRootName( rootColumnName ),
	mTypeName( typeName ),
	mbNullable(bNullable),
    miDimensionality(-1),
	mbAutoIncrement(false)
{
	SetElementState(elementState);
}

FdoSmPhColumn::~FdoSmPhColumn(void)
{
}

FdoSmPhDbObjectP FdoSmPhColumn::GetContainingDbObject()
{
    return FDO_SAFE_ADDREF( mDbObject );
}

FdoBoolean FdoSmPhColumn::GetExists() const
{
    FdoSchemaElementState elementState = GetElementState();

	return (elementState != FdoSchemaElementState_Added);
}

FdoStringP FdoSmPhColumn::GetQName() const
{
    if ( mQName.GetLength() == 0 )
    {
        ((FdoSmPhColumn*) this)->mQName = FdoStringP::Format( L"%ls.%ls", (FdoString*)(GetParent()->GetQName()), GetName() );
    }

	return mQName;
}

FdoStringP FdoSmPhColumn::GetRootName() const
{
	return mRootName;
}

FdoStringP FdoSmPhColumn::GetDbRootName()
{
    if ( GetManager()->SupportsAnsiQuotes() ) 
        return FdoStringP(L"\"") + GetRootName() + L"\"";
    else
        return GetRootName();
}

FdoStringP FdoSmPhColumn::GetTypeName() const
{
	return mTypeName;
}

bool FdoSmPhColumn::GetAutoincrement() const
{
	return mbAutoIncrement;
}

int FdoSmPhColumn::GetLength()const
{
	return 0;
}

/* The following functions provide default min/max length settings for all column types.
   They must be overridden by types that have different settings
*/

// 0 is a common minimum length for most column types
bool FdoSmPhColumn::HasMinLength() const
{
    return true;
}

int FdoSmPhColumn::GetMinLength() const
{
    return 0;
}

// Max length defaults to unlimited
bool FdoSmPhColumn::HasMaxLength() const
{
    return false;
}

int FdoSmPhColumn::GetMaxLength() const
{
    return 0;
}

int FdoSmPhColumn::GetScale() const
{
	return 0;
}

/* The following functions provide default min/max length settings for all column types.
   They must be overridden by types that have different settings
*/

// 0 is a common minimum scale for most column types
bool FdoSmPhColumn::HasMinScale() const
{
    return true;
}

int FdoSmPhColumn::GetMinScale() const
{
    return 0;
}

// Max scale defaults to unlimited
bool FdoSmPhColumn::HasMaxScale() const
{
    return false;
}

int FdoSmPhColumn::GetMaxScale() const
{
    return 0;
}

bool FdoSmPhColumn::GetNullable()const
{
	return mbNullable;
}

int FdoSmPhColumn::GetDimensionality()const
{
	return( miDimensionality );
}

void FdoSmPhColumn::SetDimensionality(int value)
{
	miDimensionality = value;
}

FdoStringP FdoSmPhColumn::GetBestPropertyName() const
{
    // Filter out characters not allowed in schema element names.
    return FdoStringP(GetName()).Replace(L":",L"_").Replace(L".",L"_");
}

FdoStringP FdoSmPhColumn::GetAddSql()
{
    FdoStringP sqlClause = FdoStringP::Format(
        L"%ls %ls %ls %ls",
        (FdoString*) GetDbName(),
        (FdoString*) GetTypeSql(),
		(FdoString*) GetAutoincrementSql(),
        (FdoString*) GetNullabilitySql()
    );

    return sqlClause;
}

FdoStringP FdoSmPhColumn::GetTypeSql()
{
    FdoStringP typeSql = GetTypeName();
    
    if ( GetLength() > 0 ) {
        FdoStringP scaleSql;
        if ( GetScale() != 0 ) 
            scaleSql = FdoStringP::Format( L", %d", GetScale() );
    
        FdoStringP sizeSql = FdoStringP::Format( 
            L" (%d%ls)", 
            GetLength(), 
            (FdoString*) scaleSql 
        );

        typeSql += sizeSql;
    }

    return typeSql;
}

FdoStringP FdoSmPhColumn::GetNullabilitySql()
{
    FdoStringP sqlNull = GetNullable() ? L"null" : L"not null";

    return sqlNull;
}

FdoStringP FdoSmPhColumn::GetNullValueSql()
{
    return L"null";
}

FdoStringP FdoSmPhColumn::GetAutoincrementSql()
{
	return L"";
}

FdoStringP FdoSmPhColumn::GetValueSql( FdoStringP val )
{
    FdoStringP valSyntax = GetNullValueSql();

    if ( val != L"" ) 
        valSyntax = val;

    return valSyntax;
}

bool FdoSmPhColumn::GetHasValues()
{
	if ( GetElementState() == FdoSchemaElementState_Added ) 
		return(false);

    bool hasData = true;

    FdoStringP sqlString = FdoStringP::Format(
        L"select 1 from %ls where (%ls is not null)", 
        (FdoString*) (GetContainingDbObject()->GetDbQName()),
        (FdoString*) GetDbName()
    );

    FdoSmPhRowP row = new FdoSmPhRow( GetManager(), L"GetHasData" );

    FdoSmPhRdQueryReaderP reader = GetManager()->CreateQueryReader( row, sqlString );

    hasData = reader->ReadNext();

    return hasData;
}

void FdoSmPhColumn::SetElementState(FdoSchemaElementState elementState)
{
    FdoSchemaElementState oldState = GetElementState();
	FdoSmPhDbElement::SetElementState(elementState);
	FdoSmPhDbObject* pObject = (FdoSmPhDbObject*) GetParent();
    int colHasValues = 0;

	switch (elementState) {
	case FdoSchemaElementState_Added:
        if ( wcslen(GetParent()->GetName()) > 0 ) {
            int i = 0;
        }

		break;

	case FdoSchemaElementState_Modified:
		break;

	case FdoSchemaElementState_Deleted:
        // Skip the check for data if column previously in Added state, since
        // it is not yet in the datastore.
        // Also skip the check if this is a column in a view.
        if ( (oldState != FdoSchemaElementState_Added) && (pObject->GetElementState() != FdoSchemaElementState_Deleted) && (dynamic_cast<FdoSmPhTable*>(pObject)) ) {
            if ( GetHasValues() ) {
			    // Error when try to delete a column that still has not null values.
			    AddColHasRowsDelError();
		    }
        }

		break;
	}
}

FdoSchemaExceptionP FdoSmPhColumn::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	// Tack on errors for this element
	FdoSchemaExceptionP pException = FdoSmPhDbElement::Errors2Exception(pFirstException);

    if ( GetElementState() != FdoSchemaElementState_Deleted ) {
        // Validate lenght and scale settings to ensure they are within the 
        // allowed limits.

        if ( HasMaxLength() && (GetMaxLength() < GetLength()) )
	        pException = FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
                    FDO_NLSID(FDOSM_7), 
			        (FdoString*) GetQName(),
                    GetLength(),
                    GetMaxLength()
                ),
                pException
	        );

        if ( HasMinLength() && (GetMinLength() > GetLength()) )
	        pException = FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
                    FDO_NLSID(FDOSM_6), 
			        (FdoString*) GetQName(),
                    GetLength(),
                    GetMinLength()
                ),
                pException
	        );

        if ( HasMaxScale() && (GetMaxScale() < GetScale()) )
	        pException = FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
                    FDO_NLSID(FDOSM_9), 
			        (FdoString*) GetQName(),
                    GetScale(),
                    GetMaxScale()
                ),
                pException
	        );

        if ( HasMinScale() && (GetMinScale() > GetScale()) )
	        pException = FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
                    FDO_NLSID(FDOSM_8), 
			        (FdoString*) GetQName(),
                    GetScale(),
                    GetMinScale()
                ),
                pException
	        );
    }

	return pException;
}

void FdoSmPhColumn::SetRootName( FdoStringP rootName )
{
    mRootName = rootName;
}

void FdoSmPhColumn::UpdRollbackCache()
{
    const FdoSmPhDbObject* dbObject = static_cast<const FdoSmPhDbObject*>(GetParent());

    if ( dbObject ) 
        GetManager()->AddRollbackColumn( dbObject->GetQName(), GetName(), GetCommitState() );
}

void FdoSmPhColumn::XMLSerialize( FILE* xmlFp, int ref ) const
{
	if ( ref == 0 ) {
		fprintf( xmlFp, "<column name=\"%ls\" description=\"%ls\" dataType=\"%ls\" length=\"%d\" scale=\"%d\" nullable=\"%s\" >\n",
				GetName(), GetDescription(),
				(FdoString*) GetTypeName(),
				GetLength(), 
				(GetLength() > 0 ) ? GetScale() : 0, 
				GetNullable() ? "True" : "False"
		);

		FdoSmPhSchemaElement::XMLSerialize( xmlFp, ref );

		fprintf( xmlFp, "</column>\n" );
	}
	else {
		fprintf( xmlFp, "<column name=\"%ls\" />\n",
				GetName()
		);
	}
}

void FdoSmPhColumn::AddColHasRowsDelError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_209), 
				(FdoString*) GetParent()->GetQName(),
                GetName()
            )
        )
	);
}
