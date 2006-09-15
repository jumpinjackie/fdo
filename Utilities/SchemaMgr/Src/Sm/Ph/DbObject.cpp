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
#include <Sm/Ph/DbObject.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Owner.h>
#include <Sm/Ph/DependencyCollection.h>
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Ph/Rd/ColumnReader.h>
#include <Sm/Ph/DependencyReader.h>
#include <Sm/Ph/TableComponentReader.h>


FdoSmPhDbObject::FdoSmPhDbObject(
    FdoStringP name, 
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState
) : 
    FdoSmPhDbElement(name, (FdoSmPhMgr*) NULL, pOwner, elementState ),
    mLtMode(NoLtLock),
    mLockingMode(NoLtLock)
{
}

FdoSmPhDbObject::FdoSmPhDbObject(void)
{
}

FdoSmPhDbObject::~FdoSmPhDbObject(void)
{
}

const FdoSmPhColumnCollection* FdoSmPhDbObject::RefColumns() const
{
    FdoSmPhColumnsP columns = ((FdoSmPhDbObject*) this)->GetColumns();

    return (FdoSmPhColumnCollection*) columns;
}

FdoSmPhColumnsP FdoSmPhDbObject::GetColumns()
{
    LoadColumns();

	return mColumns;
}


const FdoSmPhDependencyCollection* FdoSmPhDbObject::GetDependenciesDown() const
{
	((FdoSmPhDbObject*) this)->LoadDependencies();

	return (FdoSmPhDependencyCollection*) mDependenciesDown;
}

const FdoSmPhDependencyCollection* FdoSmPhDbObject::GetDependenciesUp() const
{
	((FdoSmPhDbObject*) this)->LoadDependencies();

	return (FdoSmPhDependencyCollection*) mDependenciesUp;
}

FdoSmPhDbObjectP FdoSmPhDbObject::GetLowestRootObject()
{
    return FDO_SAFE_ADDREF(this);
}

FdoStringP FdoSmPhDbObject::GetDDLName() const
{
    return GetDbName();
}

FdoStringP FdoSmPhDbObject::GetDDLQName() const
{
    return GetDbQName();
}

FdoStringP FdoSmPhDbObject::GetBestSchemaName() const
{
    return ((FdoSmPhOwner*) GetParent())->GetBestSchemaName();
}

FdoStringP FdoSmPhDbObject::GetBestClassName() const
{
    // Filter out characters not allowed in schema element names.
    return FdoStringP(GetName()).Replace(L":",L"_").Replace(L".",L"_");
}

bool FdoSmPhDbObject::GetHasData()
{
	if ( GetElementState() == FdoSchemaElementState_Added ) 
		return(false);

    bool hasData = true;

    FdoStringP sqlString = FdoStringP::Format(
        L"select 1 from %ls", 
        (FdoString*) GetDbQName() 
    );

    FdoSmPhRowP row = new FdoSmPhRow( GetManager(), L"GetHasData" );

    FdoSmPhRdQueryReaderP reader = GetManager()->CreateQueryReader( row, sqlString );

    hasData = reader->ReadNext();

    return hasData;
}

bool FdoSmPhDbObject::GetSupportsWrite() const
{
    // Return default value if the routine is not overwritten.
    return true;
}

FdoLtLockModeType FdoSmPhDbObject::GetLtMode() const
{
    return mLtMode;
}

FdoLtLockModeType FdoSmPhDbObject::GetLockingMode() const
{
    return mLockingMode;
}

const FdoLockType* FdoSmPhDbObject::GetLockTypes(FdoInt32& size) const
{
    // Objects other than tables are not lockable.
	size = 0;
	return NULL;
}

FdoStringsP FdoSmPhDbObject::GetRefColsSql()
{
	FdoSmPhColumnsP columns = GetColumns();
    return _getRefColsSql( columns );
}
   
FdoStringsP FdoSmPhDbObject::GetKeyColsSql( FdoSmPhColumnCollection* columns )
{
    // By Default, key column references have the same syntax as other 
    // references, such as view column references.
    return _getRefColsSql( columns );
}
   
FdoStringP FdoSmPhDbObject::XMLSerializeProviderAtts() const
{
    return FdoStringP::mEmptyString;
}

// Each column creator function delegates to a provider-specific implementation function.
// It also adds the column to this database object, if requested.

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnBLOB(
    FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column =  NewColumnBLOB( columnName, FdoSchemaElementState_Added, bNullable, rootColumnName );

    if (bAttach ) 
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnChar(
    FdoStringP columnName, 
	bool bNullable, 
    int length,
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnChar( columnName, FdoSchemaElementState_Added, bNullable, length, rootColumnName );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnDate(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnDate( columnName, FdoSchemaElementState_Added, bNullable, rootColumnName );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnDecimal(
    FdoStringP columnName, 
	bool bNullable, 
    int length,
    int scale,
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnDecimal( 
        columnName, 
        FdoSchemaElementState_Unchanged, 
        bNullable, 
        length,
        scale,
        rootColumnName 
    );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnSingle(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnSingle( 
        columnName, 
        FdoSchemaElementState_Added, 
        bNullable, 
        rootColumnName 
    );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnDouble(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnDouble( 
        columnName, 
        FdoSchemaElementState_Added, 
        bNullable, 
        rootColumnName 
    );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnGeom(
	FdoStringP columnName, 
    FdoSmPhScInfoP AssociatedSCInfo,      
    bool bNullable,
    bool bHasElevation, 
    bool bHasMeasure, 
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnGeom( 
        columnName, 
        FdoSchemaElementState_Added, 
        AssociatedSCInfo,        
        bNullable,
        bHasElevation,  
        bHasMeasure, 
        rootColumnName 
    );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnBool(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnBool( columnName, FdoSchemaElementState_Added, bNullable, rootColumnName );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnByte(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnByte( columnName, FdoSchemaElementState_Added, bNullable, rootColumnName );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnInt16(
	FdoStringP columnName, 
	bool bNullable, 
	bool bIsAutoincremented,
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnInt16( columnName, FdoSchemaElementState_Added, bNullable, bIsAutoincremented, rootColumnName );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnInt32(
	FdoStringP columnName, 
	bool bNullable, 
	bool bIsAutoincremented,
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnInt32( columnName, FdoSchemaElementState_Added, bNullable, bIsAutoincremented, rootColumnName );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnInt64(
	FdoStringP columnName, 
	bool bNullable, 
	bool bIsAutoincremented,
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnInt64( columnName, FdoSchemaElementState_Added, bNullable, bIsAutoincremented, rootColumnName );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnUnknown(
    FdoStringP columnName, 
    FdoStringP typeName,
	bool bNullable, 
    int length,
    int scale,
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnUnknown( 
        columnName, 
        typeName,
        FdoSchemaElementState_Unchanged, 
        bNullable, 
        length,
        scale,
        rootColumnName 
    );

    if (bAttach )
	{
		FdoSmPhColumnsP columns = GetColumns();
        columns->Add(column);
	}

    return column;
}

FdoSmPhColumnP FdoSmPhDbObject::CreateColumnDbObject(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnDbObject( columnName, FdoSchemaElementState_Added, bNullable, rootColumnName );

    if (bAttach )
	{
        FdoSmPhColumnsP pColumns = GetColumns();
		pColumns->Add(column);
	}

    return column;
}
/*
void FdoSmPhDbObject::CheckNewColumn(
    FdoStringP columnName, 
    FdoSmPhColumnP colType, 
    bool bNullable, 
    FdoStringP rootColumnName
)
{
	if ( GetColumns()->RefItem(columnName) ) 
		throw FdoSchemaException::Create(
			FdoStringP( 
				NlsMsgGet2(
					FDORDBMS_212,
					"Column %1$ls%.2$ls already exists, cannot create it", 
					(FdoString*) GetQName(),
					(FdoString*) columnName 
				)
			)
		);

	// A non-nullable column cannot be added if the table has rows.
	if ( !bNullable && GetHasRows() )
		AddNotNullColError( columnName );

	FdoSmPhColumnP newColumn = new FdoSmPhColumn(columnName, colType, bNullable, bFeatId, FdoSchemaElementState_Added, this, GetSchema(), rootColumnName );
		
	GetColumns()->Add(newColumn);

	return(newColumn);
}


void FdoSmPhDbObject::RemoveNewColumn(FdoStringP columnName)
{
    FdoSmPhColumnP pColumn = GetColumns()->GetItem( columnName );

    if ( pColumn->GetElementState() != FdoSchemaElementState_Added ) {
        // Existing columns must be flagged for delete and committed to get 
        // rid of them.
		throw FdoSchemaException::Create(
			FdoStringP( 
				NlsMsgGet2(
					FDORDBMS_334,
					"Column %1$ls%.2$ls already exists, cannot remove it without dropping it", 
					(FdoString*) GetQName(),
					(FdoString*) columnName 
				)
			)
		);
    }
    else {
        GetColumns()->Remove(pColumn);
    }
}

*/

void FdoSmPhDbObject::CacheColumns( FdoSmPhRdColumnReaderP rdr )
{
    // Do nothing if check constraints already loaded
	if ( !mColumns ) {
		mColumns = new FdoSmPhColumnCollection();

        FdoSmPhTableColumnReaderP groupReader = new FdoSmPhTableColumnReader(
            GetName(),
            rdr
        );

        LoadColumns( groupReader );
    }
}

FdoSchemaExceptionP FdoSmPhDbObject::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	// Need to finalize table to discover all errors.
//	((FdoSmPhDbObject*) this)->Finalize();

	// Tack on errors for this element
	FdoSchemaExceptionP pException = FdoSmPhDbElement::Errors2Exception(pFirstException);

    const FdoSmPhColumnCollection* pColumns = RefColumns();

	// Add errors for the database object's columns.
	for ( int i = 0; i < pColumns->GetCount(); i++ )
		pException = pColumns->RefItem(i)->Errors2Exception(pException);

	return pException;
}
/*
void FdoSmPhDbObject::XMLSerialize( FILE* xmlFp, int ref ) const
{
    const FdoSmColumnCollection* pColumns = RefColumns();

    if ( ref == 0 ) {
		for ( int i = 0; i < pColumns->GetCount(); i++ ) 
			pColumns->RefItem(i)->XMLSerialize(xmlFp, ref);
	}
}

void FdoSmPhDbObject::Finalize()
{
	if ( GetState() == FdoRdbmsSmObjectState_Finalizing ) {
		if ( GetElementState() != FdoSchemaElementState_Deleted ) 
			AddReferenceLoopError();
		return;
	}

	if ( GetState() == FdoRdbmsSmObjectState_Initial ) {
		SetState( FdoRdbmsSmObjectState_Finalizing);
*/
/*TODO:
		// Get all dependencies that this table participates in.

		FdoSmPhDependencyReader rdr( GetName(), GetName(), false, (FdoSmPhMgr*) GetSchema() );

		while( rdr.ReadNext() ) {
			FdoSmPhDependency* pDep = rdr.GetDependency(this);

			// If this table is the pkey table then this is a down dependency.
#ifdef _WIN32
			if ( wcsicmp(GetName(), rdr.GetPkTableName()) == 0 )
#else
			if ( wcscasecmp(GetName(), rdr.GetPkTableName()) == 0 )
#endif
				mDependenciesDown.Add( pDep );

			// If this table is the pkey table then this is an up dependency.
#ifdef _WIN32
			if ( wcsicmp(GetName(), rdr.GetFkTableName())== 0 )
#else
			if ( wcscasecmp(GetName(), rdr.GetFkTableName())== 0 )
#endif
				mDependenciesUp.Add( pDep );

			pDep->Release();
		}

		mDependenciesDown.Finalize();
		mDependenciesUp.Finalize();
*/
/*
		SetState( FdoRdbmsSmObjectState_Final);
	}
}
*/

void FdoSmPhDbObject::LoadDependencies()
{
	if ( !mDependenciesDown ) {
        mDependenciesDown = new FdoSmPhDependencyCollection();
        mDependenciesUp   = new FdoSmPhDependencyCollection();

        // Get all dependencies that this table participates in.

        FdoStringP myName = GetName();

		FdoSmPhDependencyReaderP rdr = new FdoSmPhDependencyReader( myName, myName, false, GetManager() );

		while( rdr->ReadNext() ) {
			FdoSmPhDependencyP pDep = rdr->GetDependency(this);

			// If this table is the pkey table then this is a down dependency.
			if ( (myName == rdr->GetPkTableName()) ||
                 (myName == GetManager()->GetDcDbObjectName(rdr->GetPkTableName()))
            )
				mDependenciesDown->Add( pDep );

			// If this table is the fkey table then this is an up dependency.
			if ( (myName == rdr->GetFkTableName()) ||
                 (myName == GetManager()->GetDcDbObjectName(rdr->GetFkTableName()))
            )
				mDependenciesUp->Add( pDep );
		}
    }
}

FdoStringsP FdoSmPhDbObject::_getRefColsSql( FdoSmPhColumnCollection* columns )
{
    FdoInt32        i;
    FdoStringsP     colClauses = FdoStringCollection::Create();

    for ( i = 0; i < columns->GetCount(); i++ ) {
        colClauses->Add( columns->GetItem(i)->GetDbName() );
    }

    return colClauses;
}

void FdoSmPhDbObject::SetLtMode( FdoLtLockModeType mode )
{
    mLtMode = mode;
}

void FdoSmPhDbObject::SetLockingMode( FdoLtLockModeType mode )
{
    mLockingMode = mode;
}

void FdoSmPhDbObject::LoadColumns()
{
    if ( !mColumns ) {
        // Columns not loaded so initialize the list.
        mColumns = new FdoSmPhColumnCollection();

        // Skip database access when not necessary: object is new or has no name.
        // Some temporary objects created by various readers have no name.
        if ( (GetElementState() != FdoSchemaElementState_Added) && (wcslen(GetName()) > 0) ) {
            FdoPtr<FdoSmPhRdColumnReader> colRdr = CreateColumnReader();

            // Read each column from the database and add it to this database object.
            if ( colRdr ) {
                FdoSmPhTableColumnReaderP groupReader = new FdoSmPhTableColumnReader(
                    GetName(),
                    colRdr
                );

                LoadColumns( groupReader );
            }
        }
    }
}

void FdoSmPhDbObject::LoadColumns( FdoPtr<FdoSmPhTableColumnReader> colRdr )
{
    while ( colRdr->ReadNext() ) {
        FdoSmPhColumnP newColumn = NewColumn( colRdr->GetColumnReader() );
        mColumns->Add( newColumn );
    }
}

FdoSmPhColumnP FdoSmPhDbObject::NewColumn(
    FdoPtr<FdoSmPhRdColumnReader> colRdr
)
{
    // Create column based on column type and reader fields.

    switch ( colRdr->GetType() ) {
    case FdoSmPhColType_BLOB:
        return NewColumnBLOB(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,    // column was read from database
                                                // so not new.
            colRdr->GetBoolean(L"",L"nullable"),
            L"",
            colRdr
        );

    case FdoSmPhColType_Date:
        return NewColumnDate(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            L"",
            colRdr
        );

    case FdoSmPhColType_Decimal:
        return NewColumnDecimal(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            colRdr->GetLong(L"",L"size"),
            colRdr->GetLong(L"",L"scale"),
            L"",
            colRdr
        );

    case FdoSmPhColType_Single:
        return NewColumnSingle(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            L"",
            colRdr
        );

    case FdoSmPhColType_Double:
        return NewColumnDouble(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            L"",
            colRdr
        );

    case FdoSmPhColType_Geom:
        return NewColumnGeom(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            (FdoSmPhScInfo*)NULL,
            colRdr->GetBoolean(L"",L"nullable"),
            true,
            false,
            L"",
            colRdr
        );

    case FdoSmPhColType_Bool:
        return NewColumnBool(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            L"",
            colRdr
        );

    case FdoSmPhColType_Byte:
        return NewColumnByte(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            L"",
            colRdr
        );

    case FdoSmPhColType_Int16:
        return NewColumnInt16(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
			colRdr->GetBoolean(L"",L"is_autoincremented"),
            L"",
            colRdr
        );

    case FdoSmPhColType_Int32:
        return NewColumnInt32(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
			colRdr->GetBoolean(L"",L"is_autoincremented"),
            L"",
            colRdr
        );

    case FdoSmPhColType_Int64:
        return NewColumnInt64(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
			colRdr->GetBoolean(L"",L"is_autoincremented"),
            L"",
            colRdr
        );

    case FdoSmPhColType_String:
        return NewColumnChar(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            colRdr->GetLong(L"",L"size"),
            L"",
            colRdr
        );

    case FdoSmPhColType_Unknown:
        return NewColumnUnknown(
            colRdr->GetString(L"",L"name"),
            colRdr->GetString(L"",L"type"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            colRdr->GetLong(L"",L"size"),
            colRdr->GetLong(L"",L"scale"),
            L"",
            colRdr
        );

    default:
        return (FdoSmPhColumn*) NULL;
    }
}

void FdoSmPhDbObject::UpdRollbackCache()
{
    GetManager()->AddRollbackTable( GetQName(), GetCommitState() );
}

void FdoSmPhDbObject::Discard()
{
    ((FdoSmPhOwner*) GetParent())->DiscardDbObject( this );
}


/*
void FdoSmPhDbObject::AddReferenceLoopError(void)
{
	mpErrors->Add( FdoRdbmsSmErrorType_Other, 
		FdoStringP( 
			NlsMsgGet1(
				FDORDBMS_215,
				"Tried to finalize table %1$ls when already finalizing",
				GetName()
			)
		)
	);
}


void FdoSmPhDbObject::AddPkeyColumnError(FdoStringP columnName)
{
	mpErrors->Add( FdoRdbmsSmErrorType_Other, 
		FdoStringP( 
			NlsMsgGet2(
				FDORDBMS_217,
				"Primary Key column %1$ls is not a column in table %2$ls",
				(FdoString*) columnName, 
				GetName()
			)
		)
	);
}

void FdoSmPhDbObject::AddNotNullColError(FdoStringP columnName)
{
	mpErrors->Add( FdoRdbmsSmErrorType_Other, 
		FdoStringP( 
			NlsMsgGet2(
				FDORDBMS_218,
				"Table %1$ls has rows; cannot add non nullable column %2$ls to it",
				GetName(), 
				(FdoString*) columnName 
			)
		)
	);
}

*/

