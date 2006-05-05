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

void FdoSmPhView::SetRootObject( FdoSmPhDbObjectP rootObject )
{
    mRootObject = rootObject;
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

FdoStringP FdoSmPhView::GetAddSql()
{
    return FdoStringP::Format( 
        L"create view %ls", 
        (FdoString*) GetQName() 
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
        colClauses->Add( columns->GetItem(i)->GetRootName() );
    }

    FdoStringP selClause = FdoStringP::Format(
        L"select %ls from %ls",
        (FdoString*) colClauses->ToString(),
        (FdoString*) GetRootNameSql()
    );

    return selClause;
}

