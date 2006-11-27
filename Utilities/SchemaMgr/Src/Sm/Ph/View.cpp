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
#include <Sm/Ph/View.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhView::FdoSmPhView(
    FdoStringP viewName, 
    FdoStringP rootDatabase, 
    FdoStringP rootOwner, 
    FdoStringP rootObjectName, 
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState
) : 
    FdoSmPhDbObject(viewName, pOwner, elementState ),
    mRootDatabase(rootDatabase),
    mRootOwner(rootOwner),
    mRootObjectName(rootObjectName)
{
}

FdoSmPhView::~FdoSmPhView(void)
{
}

const FdoSmPhDbObject* FdoSmPhView::RefRootObject() const
{
    FdoSmPhDbObjectP rootObject = ((FdoSmPhView*) this)->GetRootObject();

    return (FdoSmPhDbObject*) rootObject;
}

FdoSmPhDbObjectP FdoSmPhView::GetRootObject()
{
    if ( (mRootObjectName.GetLength() > 0) && (!mRootObject) ) 
        mRootObject = GetManager()->FindDbObject( mRootObjectName, mRootDatabase, mRootOwner );

    return mRootObject;
}

FdoSmPhDbObjectP FdoSmPhView::GetLowestRootObject()
{
    FdoSmPhDbObjectP rootObject = GetRootObject();

    if ( rootObject ) 
        rootObject = rootObject->GetLowestRootObject();

    return rootObject;
}

void FdoSmPhView::SetRootObject( FdoSmPhDbObjectP rootObject )
{
    mRootObject = rootObject;
    mRootObjectName = rootObject ? rootObject->GetName() : L"";
    mRootOwner = rootObject ? rootObject->GetParent()->GetName() : L"";
    mRootDatabase = rootObject ? rootObject->GetParent()->GetParent()->GetName() : L"";
}

void FdoSmPhView::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<view name=\"%ls\" description=\"%ls\"\n rootObject=\"%ls.%ls.%ls\" >\n",
			GetName(), GetDescription(),
            (FdoString*) GetRootDatabase(),
            (FdoString*) GetRootOwner(),
            (FdoString*) GetRootObjectName()
	);

	if ( ref == 0 ) {
		for ( int i = 0; i < RefColumns()->GetCount(); i++ ) 
			RefColumns()->RefItem(i)->XMLSerialize(xmlFp, ref);
	}

    FdoSmPhDbObject::XMLSerialize(xmlFp, ref);

	fprintf( xmlFp, "</view>\n" );

}

void FdoSmPhView::CommitChildren( bool isBeforeParent )
{
    int                     i;

    if ( isBeforeParent && 
         ((GetElementState() == FdoSchemaElementState_Unchanged) ||
          (GetElementState() == FdoSchemaElementState_Modified))
    ) {
        FdoSmPhColumnsP columns = GetColumns();

        for ( i = (columns->GetCount() - 1); i >= 0; i-- ) {
            FdoSmPhColumnP column = columns->GetItem(i);

            FdoSchemaElementState colState = column->GetElementState();

            if ( colState != FdoSchemaElementState_Unchanged ) {
                SetElementState( FdoSchemaElementState_Modified );
                column->Commit( true, isBeforeParent );
                if ( colState == FdoSchemaElementState_Deleted ) {
                    column->SetElementState( FdoSchemaElementState_Detached );
                    // Remove deleted columns from the cache.
                    columns->Remove( (FdoSmPhColumn*) column );
                }
            }
        }
    }
}

FdoStringP FdoSmPhView::GetAddSql()
{
    return FdoStringP::Format( 
        L"create view %ls", 
        (FdoString*) GetDbQName() 
    );
}

FdoStringP FdoSmPhView::GetDeleteSql()
{
    return FdoStringP::Format( 
        L"drop view %ls", 
        (FdoString*) GetDbQName() 
    );
}

FdoStringsP FdoSmPhView::GetAddColsSql()
{
    return GetRefColsSql();
}

FdoStringP FdoSmPhView::GetAddRootSql()
{
    FdoInt32        i;
    FdoSmPhColumnsP columns = GetColumns();
    FdoStringsP     colClauses = FdoStringCollection::Create();

    for ( i = 0; i < columns->GetCount(); i++ ) {
        colClauses->Add( FdoSmPhColumnP(columns->GetItem(i))->GetDbRootName() );
    }

    FdoStringP selClause = FdoStringP::Format(
        L"select %ls from %ls",
        (FdoString*) colClauses->ToString(),
        (FdoString*) GetRootNameSql()
    );

    return selClause;
}

