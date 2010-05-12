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
#include <Sm/Ph/IndexCollection.h>
#include <Sm/Ph/DependencyCollection.h>
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Ph/Rd/ColumnReader.h>
#include <Sm/Ph/Rd/BaseObjectReader.h>
#include <Sm/Ph/Rd/PkeyReader.h>
#include <Sm/Ph/Rd/FkeyReader.h>
#include <Sm/Ph/Rd/IndexReader.h>
#include <Sm/Ph/DependencyReader.h>
#include <Sm/Ph/TableComponentReader.h>
#include <Sm/Error.h>

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

const FdoSmPhColumnCollection* FdoSmPhDbObject::RefPkeyColumns() const
{
    FdoSmPhColumnsP columns = ((FdoSmPhDbObject*) this)->GetPkeyColumns();

    return (FdoSmPhColumnCollection*) columns;
}

FdoSmPhColumnsP FdoSmPhDbObject::GetPkeyColumns()
{
    LoadPkeys();

	return mPkeyColumns;
}

const FdoSmPhIndexCollection* FdoSmPhDbObject::RefIndexes() const
{
    return (FdoSmPhIndexCollection*) ((FdoSmPhDbObject*) this)->GetIndexes();
}

FdoSmPhIndexesP FdoSmPhDbObject::GetIndexes()
{
    LoadIndexes();

	return mIndexes;
}

const FdoSmPhFkeyCollection* FdoSmPhDbObject::RefFkeysUp() const
{
    return (FdoSmPhFkeyCollection*) ((FdoSmPhDbObject*) this)->GetFkeysUp();
}

FdoSmPhFkeysP FdoSmPhDbObject::GetFkeysUp()
{
    LoadFkeys();

	return mFkeysUp;
}

const FdoSmPhDependencyCollection* FdoSmPhDbObject::GetDependenciesDown() const
{
	((FdoSmPhDbObject*) this)->LoadDependencies(false);

	return (FdoSmPhDependencyCollection*) mDependenciesDown;
}

const FdoSmPhDependencyCollection* FdoSmPhDbObject::GetDependenciesUp() const
{
	((FdoSmPhDbObject*) this)->LoadDependencies(true);

	return (FdoSmPhDependencyCollection*) mDependenciesUp;
}

FdoSmPhColumnsP FdoSmPhDbObject::GetBestIdentity()
{
    FdoSmPhColumnsP bestIdentity = GetBestIdentity( (FdoSmPhDbObject*) NULL );

    // Choose primary key only if present

    FdoSmPhDbObjectP rootObject = FDO_SAFE_ADDREF(this);

    // If this database object does not have best identity of its own, look for a 
    // best identity from its root objects. 
    FdoInt32 level = 1;
    while ( bestIdentity == NULL ) {
        rootObject = rootObject->GetRootObject();
        if ( rootObject ) 
            bestIdentity = rootObject->GetBestIdentity( FDO_SAFE_ADDREF(this) );
        else
            break;

        // Give up if this object has circular root object dependencies. 
        if ( !CheckRootObjectLoop(level) )
            break;
    }

    return bestIdentity;
}

FdoSmPhColumnsP FdoSmPhDbObject::GetBestIdentity( FdoSmPhDbObjectP dbObject )
{
    FdoSmPhColumnsP bestIdentity = GetPkeyColumns();

    // Choose primary key only if present
    if ( bestIdentity->GetCount() == 0 ) {
        bestIdentity = NULL;
    }

    if ( bestIdentity ) {
        if ( dbObject && !dbObject->HasColumns(bestIdentity) ) 
            bestIdentity = NULL;
    }

    if ( bestIdentity == NULL ) {

        // No primary key, use best unique index.

        int idx = 0;
        long ixSize = (FdoSmPhIndex::mMaxWeight * 2);
        long bestIxSize = (FdoSmPhIndex::mMaxWeight * 2);
        FdoSmPhIndexP bestIndex;

        FdoSmPhIndexesP indexes = GetIndexes();

        for ( idx = 0; idx < indexes->GetCount(); idx++ ) {
            FdoSmPhIndexP index = indexes->GetItem(idx);
            FdoSmPhColumnsP idxCols = index->GetColumns();

            // Weed out non-unique indexes or indexes with no columns
            if ( index->GetIsUnique() && (index->RefColumns()->GetCount() > 0) ) {
                // Calculate index compactness score.
                ixSize = index->GetWeight();

                // Weed out indexes whose columns are too big.
                // If a database object was specified, skip indexes whose columns
                // are not all present on the database object.
                if ( (ixSize < FdoSmPhIndex::mMaxWeight) && ((!dbObject) || dbObject->HasColumns(idxCols)) ) {
                    if ( bestIndex ) {
                        // Index already chosen, see if this one is better.
                        if (idxCols->GetCount() < bestIndex->RefColumns()->GetCount() ) {
                            // This one has few columns, take it instead.
                            bestIndex = index;
                            bestIxSize = ixSize;
                        }
                        else if ( idxCols->GetCount() == bestIndex->RefColumns()->GetCount() ) {
                            // same number of columns, take this index only if it has a better
                            // compactness score.
                            if ( ixSize < bestIxSize ) {
                                bestIndex = index;
                                bestIxSize = ixSize;
                            }
                        }
                    }
                    else {
                        // No index chosen yet, chose this one initially.
                        bestIndex = index;
                        bestIxSize = ixSize;
                    }
                }
            }
        }

        if ( bestIndex )
            bestIdentity = bestIndex->GetColumns();
    }

    return bestIdentity;
}

FdoStringP FdoSmPhDbObject::GetPkeyName() const
{
    // Trip a load of the primary key.
	RefPkeyColumns();

	return mPkeyName;
}

const FdoSmPhDbObject* FdoSmPhDbObject::RefRootObject() const
{
    FdoSmPhDbObjectP rootObject = ((FdoSmPhDbObject*) this)->GetRootObject();

    return (FdoSmPhDbObject*) rootObject;
}

FdoSmPhDbObjectP FdoSmPhDbObject::GetRootObject()
{
    FdoSmPhDbObjectP rootObject;

    FdoSmPhBaseObjectsP baseObjects = GetBaseObjects();

    if ( baseObjects->GetCount() == 1 ) {
        FdoSmPhBaseObjectP baseObject = baseObjects->GetItem(0);
        if ( baseObject->GetBaseRefCount() < 2 ) 
            rootObject = baseObject->GetDbObject();
    }

    return rootObject;
}

FdoSmPhDbObjectP FdoSmPhDbObject::GetLowestRootObject()
{
    FdoSmPhDbObjectP rootObject = GetRootObject();

    if ( rootObject ) 
        rootObject = rootObject->GetLowestRootObject();
    else
        rootObject = FDO_SAFE_ADDREF(this);

    return rootObject;
}

FdoStringP FdoSmPhDbObject::GetRootObjectName() const
{
    FdoStringP rootObjectName;

    const FdoSmPhBaseObjectCollection* baseObjects = RefBaseObjects();

    if ( baseObjects->GetCount() == 1 ) {
        const FdoSmPhBaseObject* baseObject = baseObjects->RefItem(0);
        if ( baseObject->GetBaseRefCount() < 2 ) 
            rootObjectName = baseObject->GetObjectName();
    }

    return rootObjectName;
}

FdoStringP FdoSmPhDbObject::GetRootOwner() const
{
    FdoStringP rootOwnerName;

    const FdoSmPhBaseObjectCollection* baseObjects = RefBaseObjects();

    if ( baseObjects->GetCount() == 1 ) {
        const FdoSmPhBaseObject* baseObject = baseObjects->RefItem(0);
        if ( baseObject->GetBaseRefCount() < 2 ) 
            rootOwnerName = baseObject->GetOwnerName();
    }

    return rootOwnerName;
}

FdoStringP FdoSmPhDbObject::GetRootDatabase() const
{
    FdoStringP rootDatabaseName;

    const FdoSmPhBaseObjectCollection* baseObjects = RefBaseObjects();

    if ( baseObjects->GetCount() == 1 ) {
        const FdoSmPhBaseObject* baseObject = baseObjects->RefItem(0);
        if ( baseObject->GetBaseRefCount() < 2 ) 
            rootDatabaseName = baseObject->GetDatabaseName();
    }

    return rootDatabaseName;
}


const FdoSmPhBaseObjectCollection* FdoSmPhDbObject::RefBaseObjects() const
{
    FdoSmPhBaseObjectsP baseObjects = ((FdoSmPhDbObject*) this)->GetBaseObjects();

    return (FdoSmPhBaseObjectCollection*) baseObjects;
}

FdoSmPhBaseObjectsP FdoSmPhDbObject::GetBaseObjects()
{
    LoadBaseObjects();

	return mBaseObjects;
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

FdoStringP FdoSmPhDbObject::GetBestClassName(FdoStringP schemaName) const
{
    FdoStringP classifiedObjectName;
    // Only tables and views are supported
    if ( ((FdoSmPhDbObject*)this)->ClassifyObjectType(true) )
    {
        classifiedObjectName = ((FdoSmPhDbObject*)this)->GetClassifiedObjectName(schemaName);
    }

    // Filter out characters not allowed in schema element names.
    return FdoStringP(classifiedObjectName).Replace(L":",L"_").Replace(L".",L"_");
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

FdoBoolean FdoSmPhDbObject::IsUkeyPkey( FdoSmPhColumnsP ukeyColumns )
{
    FdoBoolean      isUkeyPkey  = false;
    FdoInt32        idx;
    FdoSmPhColumnsP pkeyColumns = GetPkeyColumns();

    if ( (ukeyColumns->GetCount() > 0) && (ukeyColumns->GetCount() == pkeyColumns->GetCount()) ) {
        isUkeyPkey = true;

        for ( idx = 0; idx < ukeyColumns->GetCount(); idx++ ) {
            FdoSmPhColumnP ukeyColumn = ukeyColumns->GetItem( idx );

            if ( pkeyColumns->IndexOf(ukeyColumn->GetName()) < 0 ) {
                isUkeyPkey = false;
                break;
            }
        }
    }

    return isUkeyPkey;
}

FdoBoolean FdoSmPhDbObject::HasColumns( FdoSmPhColumnsP columns )
{
    FdoBoolean hasColumns = true;
    FdoInt32 ix;

    for ( ix = 0; ix < columns->GetCount(); ix++ ) {
        FdoSmPhColumnP column = columns->GetItem( ix );
        FdoSmPhColumnP thisColumn = GetColumns()->FindItem(column->GetName());
        if ( (!thisColumn) || !column->DefinitionEquals(thisColumn) ) {
            hasColumns = false;
            break;
        }
    }

    return hasColumns;
}

void FdoSmPhDbObject::DiscardIndex( FdoSmPhIndex* index )
{
    FdoSmPhIndexesP indexes = GetIndexes();
    indexes->Remove( index );
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
   
FdoStringP FdoSmPhDbObject::GetAddPkeySql()
{
    FdoSmPhColumnsP     pkeyColumns = GetPkeyColumns();
    FdoStringP          pkeySql;
    bool                ansiQuotes = GetManager()->SupportsAnsiQuotes();

    if ( pkeyColumns->GetCount() > 0 ) {
        FdoStringsP pkColNames = GetKeyColsSql( pkeyColumns );

        FdoStringP pkeyName = this->GenPkeyName();

        // Remove qualification by physical schema if present.
        if ( pkeyName.Contains(L".") ) 
            pkeyName = mPkeyName.Right( L"." );

        pkeySql = FdoStringP::Format( 
            L"constraint %ls%ls%ls primary key ( %ls )",
            ansiQuotes ? L"\"" : L"",
            (FdoString*) pkeyName,
            ansiQuotes ? L"\"" : L"",
            (FdoString*) pkColNames->ToString()
        );
    }

    return pkeySql;
}

void FdoSmPhDbObject::SetPkeyName( FdoStringP pkeyName )
{
    if ( GetElementState() != FdoSchemaElementState_Added )
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_20), 
				(FdoString*) GetQName()
            )
        );

    mPkeyName = pkeyName;
}

void FdoSmPhDbObject::AddPkeyCol(FdoStringP columnName )
{
    LoadPkeys();

	FdoSmPhColumnP column = GetColumns()->FindItem( columnName );

	if ( column ) 
		mPkeyColumns->Add( column );
	else
		throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_213),				
   				(FdoString*) columnName, 
				GetName()
			)
		);
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
	FdoPtr<FdoDataValue> defaultValue,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnChar( columnName, FdoSchemaElementState_Added, bNullable, length, rootColumnName, defaultValue );

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
	FdoPtr<FdoDataValue> defaultValue,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnDate( columnName, FdoSchemaElementState_Added, bNullable, rootColumnName, defaultValue );

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
	FdoPtr<FdoDataValue> defaultValue,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnDecimal( 
        columnName, 
        FdoSchemaElementState_Added, 
        bNullable, 
        length,
        scale,
        rootColumnName,
		defaultValue
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
	FdoPtr<FdoDataValue> defaultValue,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnSingle( 
        columnName, 
        FdoSchemaElementState_Added, 
        bNullable, 
        rootColumnName,
		defaultValue
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
	FdoPtr<FdoDataValue> defaultValue,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnDouble( 
        columnName, 
        FdoSchemaElementState_Added, 
        bNullable, 
        rootColumnName,
		defaultValue
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
	FdoPtr<FdoDataValue> defaultValue,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnBool( columnName, FdoSchemaElementState_Added, bNullable, rootColumnName, defaultValue );

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
	FdoPtr<FdoDataValue> defaultValue,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnByte( columnName, FdoSchemaElementState_Added, bNullable, rootColumnName, defaultValue );

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
	FdoPtr<FdoDataValue> defaultValue,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnInt16( columnName, FdoSchemaElementState_Added, bNullable, bIsAutoincremented, rootColumnName, defaultValue );

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
	FdoPtr<FdoDataValue> defaultValue,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnInt32( columnName, FdoSchemaElementState_Added, bNullable, bIsAutoincremented, rootColumnName, defaultValue );

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
	FdoPtr<FdoDataValue> defaultValue,
    bool bAttach
)
{
    FdoSmPhColumnP column = NewColumnInt64( columnName, FdoSchemaElementState_Added, bNullable, bIsAutoincremented, rootColumnName, defaultValue );

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
        FdoSchemaElementState_Added, 
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

FdoSmPhFkeyP FdoSmPhDbObject::CreateFkey(
    FdoStringP name, 
    FdoStringP pkeyTableName,
    FdoStringP pkeyTableOwner
)
{
    FdoStringP lTableOwner = pkeyTableOwner;

    if ( lTableOwner == L"" ) 
        lTableOwner = GetParent()->GetName();

    FdoSmPhFkeyP fkey = NewFkey( name, pkeyTableName, lTableOwner );
    if ( fkey == NULL ) 
        AddCreateFkeyError( name );

    FdoSmPhFkeysP fkeys = GetFkeysUp();
    fkeys->Add( fkey );

    return fkey;
}

void FdoSmPhDbObject::Commit( bool fromParent, bool isBeforeParent )
{
    FdoSmPhDbObjectP rootObject = this->GetRootObject();

    if ( rootObject ) 
        rootObject->Commit(fromParent, isBeforeParent);

    FdoSmPhDbElement::Commit( fromParent, isBeforeParent );
}

void FdoSmPhDbObject::ExecuteDDL( FdoStringP sqlStmt, FdoSmPhDbObject* refTable, bool isDDL )
{
    GetManager()->ExecuteSQL( sqlStmt, isDDL );
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

void FdoSmPhDbObject::CacheBaseObjects( FdoSmPhRdBaseObjectReaderP rdr )
{
    // Do nothing if base objects already loaded
	if ( !mBaseObjects ) {
		mBaseObjects = new FdoSmPhBaseObjectCollection( this );

        FdoSmPhTableComponentReaderP groupReader = NewTableBaseReader(
            rdr
        );

        LoadBaseObjects( groupReader );
    }
}

void FdoSmPhDbObject::CachePkeys( FdoSmPhRdPkeyReaderP rdr )
{
    // Do nothing if primary key already loaded
	if ( !mPkeyColumns ) {
        mPkeyColumns = new FdoSmPhColumnCollection();

        LoadPkeys( NewTablePkeyReader(rdr)->SmartCast<FdoSmPhReader>(), false );
    }
    else
        LoadPkeys( NewTablePkeyReader(rdr)->SmartCast<FdoSmPhReader>(), true );
}

void FdoSmPhDbObject::CacheFkeys( FdoSmPhRdFkeyReaderP rdr )
{
    // Do nothing if foreign keys already loaded.
    if ( !mFkeysUp ) {
        mFkeysUp = new FdoSmPhFkeyCollection();

        LoadFkeys( NewTableFkeyReader(rdr)->SmartCast<FdoSmPhReader>(), false );
    }
    else
        LoadFkeys( NewTableFkeyReader(rdr)->SmartCast<FdoSmPhReader>(), true );
}

void FdoSmPhDbObject::CacheDependenciesUp( FdoSmPhDependencyReaderP rdr )
{
    // Do nothing if "up" dependencies already loaded
	if ( !mDependenciesUp ) {
		mDependenciesUp = new FdoSmPhDependencyCollection();

        // Determine the name of the row containing the table name field.
        FdoStringP rowName;

        if ( rdr->GetRows() && (rdr->GetRows()->GetCount() > 0) )
            rowName = FdoSmPhRowP(rdr->GetRows()->GetItem(0))->GetName();

        // Create a group reader to extract dependencies just for this table. 
        FdoSmPhTableDependencyReaderP groupReader = new FdoSmPhTableDependencyReader(
            GetName(),
            rowName,
            L"fktablename",
            rdr
        );

        LoadDependenciesUp( groupReader );
    }
}

void FdoSmPhDbObject::CommitFkeys( bool isBeforeParent )
{
    FdoInt32        i;
    FdoSmPhFkeysP   fkeys = GetFkeysUp();
    FdoStringsP     fkeyClauses = FdoStringCollection::Create();

    for ( i = (fkeys->GetCount() - 1); i >= 0; i-- ) {
        FdoSmPhFkeyP(fkeys->GetItem(i))->Commit(true, isBeforeParent);
    }
}

void FdoSmPhDbObject::ForceDelete()
{
    FdoInt32 i;

  	FdoSmPhSchemaElement::SetElementState( FdoSchemaElementState_Deleted );

    // Must explicitly delete any foreign keys when table is deleted.
    FdoSmPhFkeysP   fkeys = GetFkeysUp();
    for ( i = 0; i < fkeys->GetCount(); i++ ) {
        FdoSmPhFkeyP(fkeys->GetItem(i))->SetElementState(FdoSchemaElementState_Deleted);
    }
}

void FdoSmPhDbObject::LoadFkeyRefCands()
{
    int idx;

    if ( mFkeysUp ) {
        for ( idx = 0; idx < mFkeysUp->GetCount(); idx++ ) {
            FdoSmPhFkeyP fkey = mFkeysUp->GetItem(idx);
            fkey->LoadRefCand();
        }
    }
}

FdoSchemaExceptionP FdoSmPhDbObject::Errors2Exception(FdoSchemaException* pFirstException ) const
{

	// Tack on errors for this element
	FdoSchemaExceptionP pException = FdoSmPhDbElement::Errors2Exception(pFirstException);

    const FdoSmPhColumnCollection* pColumns = RefColumns();
    int i;

	// Add errors for the database object's columns.
	for ( i = 0; i < pColumns->GetCount(); i++ )
		pException = pColumns->RefItem(i)->Errors2Exception(pException);

    if ( mIndexes ) {
        // Add errors for the indexes.
	    for ( i = 0; i < mIndexes->GetCount(); i++ )
		    pException = mIndexes->RefItem(i)->Errors2Exception(pException);
    }

    if ( mFkeysUp ) {

	    // Add errors for the database object's foreign keys.
	    for ( i = 0; i < mFkeysUp->GetCount(); i++ )
		    pException = mFkeysUp->RefItem(i)->Errors2Exception(pException);
    }

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

*/
void FdoSmPhDbObject::LoadDependencies( bool up )
{
	if ( (up && !mDependenciesUp) || ((!up) && !mDependenciesDown) ) {
        mDependenciesDown = new FdoSmPhDependencyCollection();
        mDependenciesUp   = new FdoSmPhDependencyCollection();

        // Skip database access when not necessary: object is new or has no name.
        // Some temporary objects created by various readers have no name.
        if ( (GetElementState() != FdoSchemaElementState_Added) && (wcslen(GetName()) > 0) ) {
            FdoStringP myName = GetName();
            
            // Get all dependencies that this table participates in.

		    FdoSmPhDependencyReaderP rdr = new FdoSmPhDependencyReader( myName, myName, false, GetManager() );

            // Read each dependency from the database and add it to this database object.
            if ( rdr ) {
                while ( rdr->ReadNext() ) {
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
    }
}

void FdoSmPhDbObject::LoadDependenciesUp( FdoPtr<FdoSmPhTableDependencyReader> depRdr )
{
    FdoStringP myName = GetName();

    while ( depRdr->ReadNext() ) {
	    FdoSmPhDependencyP pDep = depRdr->GetDependencyReader()->GetDependency(this);
        mDependenciesUp->Add( pDep );
    }
}

FdoPtr<FdoSmPhTableComponentReader> FdoSmPhDbObject::NewTablePkeyReader( FdoSmPhRdPkeyReaderP rdr )
{
    return new FdoSmPhTableComponentReader(
        GetName(),
        L"",
        L"table_name",
        rdr->SmartCast<FdoSmPhReader>()
    );
}

FdoPtr<FdoSmPhTableIndexReader> FdoSmPhDbObject::NewTableIndexReader( FdoSmPhRdIndexReaderP rdr )
{
    return new FdoSmPhTableIndexReader(
        GetName(),
        rdr
    );
}

FdoPtr<FdoSmPhTableComponentReader> FdoSmPhDbObject::NewTableFkeyReader( FdoSmPhRdFkeyReaderP rdr )
{
    return new FdoSmPhTableComponentReader(
        GetName(),
        L"",
        L"table_name",
        rdr->SmartCast<FdoSmPhReader>()
    );
}

FdoStringsP FdoSmPhDbObject::_getRefColsSql( FdoSmPhColumnCollection* columns )
{
    FdoInt32        i;
    FdoStringsP     colClauses = FdoStringCollection::Create();

    for ( i = 0; i < columns->GetCount(); i++ ) {
        colClauses->Add( columns->RefItem(i)->GetDbName() );
    }

    return colClauses;
}

FdoSmPhBaseObjectsP FdoSmPhDbObject::Get_BaseObjects()
{
    return mBaseObjects;
}

void FdoSmPhDbObject::DiscardBaseObjects()
{
    mBaseObjects = NULL;
}


void FdoSmPhDbObject::SetLtMode( FdoLtLockModeType mode )
{
    mLtMode = mode;
}

void FdoSmPhDbObject::SetLockingMode( FdoLtLockModeType mode )
{
    mLockingMode = mode;
}

void FdoSmPhDbObject::SetRootObject( FdoSmPhDbObjectP rootObject )
{
    if ( mBaseObjects ) 
        mBaseObjects->Clear();
    else 
        mBaseObjects = new FdoSmPhBaseObjectCollection( this );

    if ( rootObject ) {
        FdoSmPhBaseObjectP baseObject = NewBaseObject( rootObject );
        mBaseObjects->Add( baseObject );
    }
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
        if ( newColumn ) 
            mColumns->Add( newColumn );
    }
}

void FdoSmPhDbObject::LoadBaseObjects()
{
    if ( !mBaseObjects ) {
        // Base objects not loaded so initialize the list.
        mBaseObjects = new FdoSmPhBaseObjectCollection( this );

        // Skip database access when not necessary: object is new or has no name.
        // Some temporary objects created by various readers have no name.
        if ( (GetElementState() != FdoSchemaElementState_Added) && (wcslen(GetName()) > 0) ) {
            FdoPtr<FdoSmPhRdBaseObjectReader> baseObjRdr = CreateBaseObjectReader();

            // Read each column from the database and add it to this database object.
            if ( baseObjRdr ) {
                FdoSmPhTableComponentReaderP groupReader = NewTableBaseReader(
                    baseObjRdr
                );

                LoadBaseObjects( groupReader );
            }
        }
    }
}

void FdoSmPhDbObject::LoadBaseObjects( FdoPtr<FdoSmPhTableComponentReader> baseObjRdr, bool isSkipAdd )
{
    while ( baseObjRdr->ReadNext() ) {
        if ( !isSkipAdd ) {
            FdoSmPhBaseObjectP newBaseObject = NewBaseObject( baseObjRdr );

            if ( newBaseObject ) {
                FdoSmPhBaseObjectP currBaseObject = mBaseObjects->FindItem( newBaseObject->GetName() );
                if ( currBaseObject ) 
                    // Base object already in collection, just add a base reference.
                    currBaseObject->AddBaseRef();
                else
                    // Not in collection, add it.
                    mBaseObjects->Add( newBaseObject );
            }
        }
    }
}

void FdoSmPhDbObject::LoadPkeys(void)
{
    // Do nothing if primary key already loaded
	if ( !mPkeyColumns ) {
        mPkeyColumns = new FdoSmPhColumnCollection();

        // Skip load if new table.
        if ( GetElementState() != FdoSchemaElementState_Added ) {
            FdoPtr<FdoSmPhRdPkeyReader> pkeyRdr = CreatePkeyReader();

            if ( pkeyRdr ) 
                LoadPkeys( pkeyRdr->SmartCast<FdoSmPhReader>(), false );
        }
    }
}

void FdoSmPhDbObject::LoadPkeys( FdoSmPhReaderP pkeyRdr, bool isSkipAdd )
{
    // read each primary key column.
    while (pkeyRdr->ReadNext() ) {
        mPkeyName = pkeyRdr->GetString(L"", L"constraint_name");

        if ( !isSkipAdd ) 
            LoadPkeyColumn( pkeyRdr, mPkeyColumns );
    }
}

void FdoSmPhDbObject::LoadPkeyColumn( FdoSmPhReaderP pkeyRdr, FdoSmPhColumnsP pkeyColumns )
{
    FdoStringP columnName = pkeyRdr->GetString(L"",L"column_name");

    FdoSmPhColumnP pkeyColumn = GetColumns()->FindItem( columnName );

    if ( pkeyColumn == NULL ) {
        // Primary Key column must be in this table.
        if ( GetElementState() != FdoSchemaElementState_Deleted )
	        AddPkeyColumnError( columnName );
    }
    else  {
        mPkeyColumns->Add(pkeyColumn);
    }
}

bool FdoSmPhDbObject::LoadIndexes(void)
{
    bool ret = false; 

    // Do nothing if already loaded

    // If not loaded, try bulk fetch of indexes for this table plus some other
    // candidates.
    if ( !IndexesLoaded() && (GetElementState() != FdoSchemaElementState_Added) ) {
        FdoSmPhOwner* pOwner = (FdoSmPhOwner*) GetParent();
        pOwner->CacheCandIndexes( GetName() );
    }

	if ( !IndexesLoaded() ) {
        // Not loaded by bulk fetch, just load indexes for this table.
        mIndexes = new FdoSmPhIndexCollection();

        // Skip load if table is new
        if ( GetElementState() != FdoSchemaElementState_Added ) {
            FdoPtr<FdoSmPhRdIndexReader> indexRdr = CreateIndexReader();

            ret = LoadIndexes( NewTableIndexReader(indexRdr), false );
        }
    }

    if ( !mIndexes ) 
        mIndexes = new FdoSmPhIndexCollection();

    return ret;
}

bool FdoSmPhDbObject::LoadIndexes( FdoSmPhTableIndexReaderP indexRdr, bool isSkipAdd )
{
    FdoStringP            nextIndex;
    FdoSmPhIndexP         index;
    bool                  ret = false;

    // Read each index and column
    while ( indexRdr->ReadNext() ) {
        ret = true;
        nextIndex = indexRdr->GetString(L"",L"index_name");

        if ( !index || (nextIndex != index->GetName()) ) {
            // hit the next index. Create an object for it
            index = CreateIndex( indexRdr ); 
                        
            if ( index && ! isSkipAdd )
                mIndexes->Add(index);
        }

        LoadIndexColumn( indexRdr, index );
    }

    return ret;
}

void FdoSmPhDbObject::LoadIndexColumn( FdoSmPhTableIndexReaderP indexRdr, FdoSmPhIndexP index )
{
    FdoStringP columnName = indexRdr->GetString(L"",L"column_name");
    FdoSmPhColumnP column = GetColumns()->FindItem(columnName);

    if ( column ) {
        // Add the column to the current index.
        index->AddColumn( column );
    }
    else {
        // Index column must be in this table.
        if ( GetElementState() != FdoSchemaElementState_Deleted )
	        AddIndexColumnError( columnName );
    }
}

bool FdoSmPhDbObject::CacheIndexes( FdoSmPhRdIndexReaderP rdr )
{
    bool ret = false;

    // Do nothing if indexes already loaded
	if ( !mIndexes ) {
        mIndexes = new FdoSmPhIndexCollection();

        ret = LoadIndexes( NewTableIndexReader(rdr), false );
    }
    else
        ret = LoadIndexes( NewTableIndexReader(rdr), true );

    return ret;
}

bool FdoSmPhDbObject::IndexesLoaded()
{
    return (mIndexes != NULL);
}

void FdoSmPhDbObject::LoadFkeys(void)
{
    // Do nothing if already loaded
	if ( !mFkeysUp ) {
        mFkeysUp = new FdoSmPhFkeyCollection();

        // Skip load for new tables
        if ( GetElementState() != FdoSchemaElementState_Added ) {
            FdoPtr<FdoSmPhRdFkeyReader> fkeyRdr = CreateFkeyReader();

            if ( fkeyRdr ) 
                LoadFkeys( fkeyRdr->SmartCast<FdoSmPhReader>(), false );
        }
    }
}

void FdoSmPhDbObject::LoadFkeys( FdoSmPhReaderP fkeyRdr, bool isSkipAdd  )
{
    FdoStringP                  nextFkey;
    FdoSmPhFkeyP                fkey;

    // Read each foreign key and column
    while ( fkeyRdr->ReadNext() ) {
        nextFkey = fkeyRdr->GetString(L"",L"constraint_name");

        if ( !fkey || (nextFkey != fkey->GetName()) ) {
            // hit the next foreign key. Create an object for it
            fkey = NewFkey(
                nextFkey, 
                fkeyRdr->GetString(L"", "r_table_name"),
                fkeyRdr->GetString(L"", "r_owner_name"),
                FdoSchemaElementState_Unchanged
            );

            if( fkey && ! isSkipAdd )
                mFkeysUp->Add(fkey);
        }

        LoadFkeyColumn(fkeyRdr, fkey);
    }
}

void FdoSmPhDbObject::LoadFkeyColumn( FdoSmPhReaderP fkeyRdr, FdoSmPhFkeyP fkey )
{
    // Add the column to the foreign key
    FdoStringP columnName = fkeyRdr->GetString(L"",L"column_name");
    FdoSmPhColumnP column = GetColumns()->FindItem(columnName);

    if ( fkey && column ) {
        fkey->AddFkeyColumn( 
            column,
            fkeyRdr->GetString(L"", "r_column_name")
        );
    }
    else {
        // Foreign Key column must be in this table.
        if ( GetElementState() != FdoSchemaElementState_Deleted )
	        AddFkeyColumnError( columnName );
    }
}


FdoPtr<FdoSmPhTableComponentReader> FdoSmPhDbObject::NewTableBaseReader( FdoSmPhRdBaseObjectReaderP rdr )
{
    return new FdoSmPhTableComponentReader(
        GetName(),
        L"",
        L"name",
        rdr->SmartCast<FdoSmPhReader>()
    );
}

FdoSmPhColumnP FdoSmPhDbObject::NewColumn(
    FdoPtr<FdoSmPhRdColumnReader> colRdr
)
{
    // Create column based on column type and reader fields.

    FdoPtr<FdoDataValue> defaultValue = GetManager()->ParseSQLVal( colRdr->GetString(L"", L"default_value") );

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
			defaultValue,
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
			defaultValue,
            colRdr
        );

    case FdoSmPhColType_Single:
        return NewColumnSingle(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            L"",
			defaultValue,
            colRdr
        );

    case FdoSmPhColType_Double:
        return NewColumnDouble(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            L"",
			defaultValue,
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
			defaultValue,
            colRdr
        );

    case FdoSmPhColType_Byte:
        return NewColumnByte(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            L"",
			defaultValue,
            colRdr
        );

    case FdoSmPhColType_Int16:
        return NewColumnInt16(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
			colRdr->GetBoolean(L"",L"is_autoincremented"),
            L"",
			defaultValue,
            colRdr
        );

    case FdoSmPhColType_Int32:
        return NewColumnInt32(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
			colRdr->GetBoolean(L"",L"is_autoincremented"),
            L"",
			defaultValue,
            colRdr
        );

    case FdoSmPhColType_Int64:
        return NewColumnInt64(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
			colRdr->GetBoolean(L"",L"is_autoincremented"),
            L"",
			defaultValue,
            colRdr
        );

    case FdoSmPhColType_String:
        return NewColumnChar(
            colRdr->GetString(L"",L"name"),
            FdoSchemaElementState_Unchanged,
            colRdr->GetBoolean(L"",L"nullable"),
            colRdr->GetLong(L"",L"size"),
            L"",
			defaultValue,
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

FdoSmPhIndexP FdoSmPhDbObject::CreateIndex(
    FdoPtr<FdoSmPhTableIndexReader> rdr
)
{
    FdoSmPhIndexP index;

    switch ( rdr->GetIndexType() ) {
    case FdoSmPhIndexType_Scalar:
        index = NewIndex(
            rdr->GetString(L"",L"index_name"), 
            (rdr->GetString(L"",L"uniqueness") == L"UNIQUE") ? true : false,
            FdoSchemaElementState_Unchanged
        );
        break;

    case FdoSmPhIndexType_Spatial:
        index = NewSpatialIndex(
            rdr->GetString(L"",L"index_name"), 
            (rdr->GetString(L"",L"uniqueness") == L"UNIQUE") ? true : false,
            FdoSchemaElementState_Unchanged
        );
        break;
    }

    return index;
}

FdoSmPhBaseObjectP FdoSmPhDbObject::NewBaseObject(
    FdoPtr<FdoSmPhTableComponentReader> baseObjRdr
)
{
    return NewBaseObject(
        baseObjRdr->GetString(L"",L"base_name"),
        baseObjRdr->GetString(L"",L"base_owner"),
        baseObjRdr->GetString(L"",L"base_database")
    );
}

FdoSmPhBaseObjectP FdoSmPhDbObject::NewBaseObject(
    FdoStringP name,
    FdoStringP ownerName,
    FdoStringP databaseName
)
{
    return new FdoSmPhBaseObject( name, FDO_SAFE_ADDREF(this), ownerName, databaseName );
}

FdoSmPhBaseObjectP FdoSmPhDbObject::NewBaseObject(
    FdoPtr<FdoSmPhDbObject> dbObject
)
{
    return new FdoSmPhBaseObject( dbObject, FDO_SAFE_ADDREF(this));
}

FdoStringP FdoSmPhDbObject::GenPkeyName()
{
    if ( mPkeyName == L"" ) {
        FdoSmPhOwner* pOwner = dynamic_cast<FdoSmPhOwner*>((FdoSmPhSchemaElement*) GetParent());
        FdoStringP PkeyName = GetName();
        if ( PkeyName.Contains(L".") ) 
            PkeyName = PkeyName.Replace( L".", L".pk_" );
        else
            PkeyName = FdoStringP(L"pk_") + PkeyName;

        mPkeyName = pOwner->UniqueDbObjectName( PkeyName );
    }

    return mPkeyName;
}

FdoSmPhFkeyP FdoSmPhDbObject::NewFkey(
    FdoStringP name, 
    FdoStringP pkeyTableName,
    FdoStringP pkeyTableOwner,
    FdoSchemaElementState elementState
)
{
    return (FdoSmPhFkey*) NULL;
}

FdoPtr<FdoSmPhRdBaseObjectReader> FdoSmPhDbObject::CreateBaseObjectReader() const
{
    return (FdoSmPhRdBaseObjectReader*) NULL;
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhDbObject::CreatePkeyReader() const
{
    return (FdoSmPhRdPkeyReader*) NULL;
}

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhDbObject::CreateFkeyReader() const
{
    return (FdoSmPhRdFkeyReader*) NULL;
}

void FdoSmPhDbObject::UpdRollbackCache()
{
    GetManager()->AddRollbackTable( GetQName(), GetCommitState() );
}

void FdoSmPhDbObject::Discard()
{
    ((FdoSmPhOwner*) GetParent())->DiscardDbObject( this );
}

void FdoSmPhDbObject::AddPkeyColumnError(FdoStringP columnName)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_217), 
				(FdoString*) columnName, 
				(FdoString*) GetQName()
            )
        )
	);
}

void FdoSmPhDbObject::AddFkeyColumnCountError(FdoStringP fkeyName)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_383), 
				(FdoString*) fkeyName 
            )
        )
	);
}

void FdoSmPhDbObject::AddIndexColumnError(FdoStringP columnName)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_3), 
				(FdoString*) columnName, 
				(FdoString*) GetQName()
            )
        )
	);
}

bool FdoSmPhDbObject::CheckRootObjectLoop( FdoInt32& level )
{
    bool ok = true;

    // When there are 100 or levels of object dependencies, there is likely something wrong.
    // Check for a dependency loop in this case. This is done by counting the total number
    // of DbObjects currently in the cache. If there are fewer objects than dependency levels
    // we must have visited an object more than once, meaning that there is a dependency loop.
    if ( (level % 100) == 0 ) {
        FdoInt32 totalObjectCount = 0;
        FdoInt32 ixD = 0;
        
        // Count objects for each database
        for ( ixD = 0; ; ixD++ ) {
            FdoSmPhDatabaseP database = GetManager()->GetCachedDatabase( ixD );
            if ( database == NULL ) 
                break;

            FdoInt32 ixO;

            // Count objects for each owner within database
            for ( ixO = 0; ; ixO++ ) {
                FdoSmPhOwnerP owner = database->GetCachedOwner( ixO );
                if ( owner == NULL ) 
                    break;

                FdoInt32 ixB;

                // Count each object for each owner.
                for ( ixB = 0; ; ixB++ ) {
                    FdoSmPhDbObjectP dbObject = owner->GetCachedDbObject( ixB );
                    if ( dbObject == NULL ) 
                        break;

                    totalObjectCount++;
                }
            }
        }
        
        // Add 1 to be on the safe side.
        if ( level > (totalObjectCount + 1) ) {
            ok = false;
        }
    }

    level++;

    return ok;
}

void FdoSmPhDbObject::AddFkeyColumnError(FdoStringP columnName)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_2), 
				(FdoString*) columnName, 
				(FdoString*) GetQName()
            )
        )
	);
}

void FdoSmPhDbObject::AddCreateFkeyError(FdoStringP fkeyName)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_30), 
				(FdoString*) fkeyName, 
				(FdoString*) GetQName()
            )
        )
	);
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

*/

bool FdoSmPhDbObject::ClassifyObjectType(FdoBoolean classifyDefaultTypes )
{
    FdoSmPhTableP pTable = SmartCast<FdoSmPhTable>();
    FdoSmPhViewP pView = SmartCast<FdoSmPhView>();

    return ( pTable || pView );
}

FdoStringP FdoSmPhDbObject::GetClassifiedObjectName( FdoStringP schemaName )
{
    FdoStringP classifiedObjectName;

    // Don't reverse-engineer the special spatial context referencer table.
    if ( GetManager()->GetRealDbObjectName(FdoSmPhMgr::ScInfoNoMetaTable) != GetName() ) {
        classifiedObjectName = GetBestClassName();

        if ( classifiedObjectName.GetLength() > 0 ) {
            if ( (schemaName != L"") && (GetBestSchemaName() != schemaName) )
            {
                // DbObject is for a different feature schema.
                classifiedObjectName = L"";
            }

        }
    }

    return classifiedObjectName;
}


