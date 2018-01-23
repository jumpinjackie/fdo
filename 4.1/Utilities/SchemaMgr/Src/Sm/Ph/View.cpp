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
#include <Sm/Ph/TableComponentReader.h>
#include <Sm/Ph/Rd/ViewReader.h>

FdoSmPhView::FdoSmPhView(
    FdoStringP viewName, 
    FdoStringP rootDatabase, 
    FdoStringP rootOwner, 
    FdoStringP rootObjectName, 
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState
) : 
    FdoSmPhDbObject(viewName, pOwner, elementState ),
    mViewLoaded(false)
{
    if ( rootObjectName != L"" ) {
        FdoSmPhBaseObjectP baseObject = NewBaseObject( rootObjectName, rootOwner, rootDatabase );
        GetBaseObjects()->Add( baseObject );
    }
}

FdoSmPhView::~FdoSmPhView(void)
{
}

void FdoSmPhView::SetRootObject( FdoSmPhDbObjectP rootObject )
{
    FdoSmPhDbObject::SetRootObject( rootObject );
}

void FdoSmPhView::CacheView( FdoSmPhRdViewReaderP rdr )
{
    // Do nothing if view already loaded
	if ( !mViewLoaded ) {
        LoadView( NewViewReader(rdr), false );
        mViewLoaded = true;
    }
    else
        LoadView( NewViewReader(rdr), true );

}

void FdoSmPhView::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<view name=\"%s\" description=\"%s\"\n rootObject=\"%s.%s.%s\" >\n",
			(const char*) FdoStringP(GetName()), 
            (const char*) FdoStringP(GetDescription()),
            (const char*) GetRootDatabase(),
            (const char*) GetRootOwner(),
            (const char*) GetRootObjectName()
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

    // After view commit, just commit the columns to update their element states.
    if ( !isBeforeParent ) {
        FdoSmPhColumnsP columns = GetColumns();

        for ( i = (columns->GetCount() - 1); i >= 0; i-- ) {
            FdoSmPhColumnP column = columns->GetItem(i);

            column->Commit( true, isBeforeParent );
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
/*
void FdoSmPhDbObject::LoadView()
{
    if ( !mViewLoaded ) {
        TODO: implement.
    }
}
*/
void FdoSmPhView::LoadView( FdoPtr<FdoSmPhTableComponentReader> viewRdr, bool isSkipAdd )
{
    if ( viewRdr->ReadNext() ) {
        if ( !isSkipAdd ) 
            mSql = viewRdr->GetString( L"", L"sql" );
    }
}



FdoPtr<FdoSmPhTableComponentReader> FdoSmPhView::NewViewReader( FdoSmPhRdViewReaderP rdr )
{
    return new FdoSmPhTableComponentReader(
        GetName(),
        L"",
        L"name",
        rdr->SmartCast<FdoSmPhReader>()
    );
}

