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

FdoSmPhBaseObject::FdoSmPhBaseObject(
    FdoStringP name, 
    FdoPtr<FdoSmPhDbObject> parent,
    FdoStringP ownerName,
    FdoStringP databaseName
) : 
    FdoSmPhDbElement(name, (FdoSmPhMgr*) NULL, parent, FdoSchemaElementState_Detached ),
    mOwnerName(ownerName),
    mDatabaseName(databaseName),
    mBaseRefCount(1)
{
    if ( ownerName == L"" ) 
        mOwnerName = parent->GetParent()->GetName();
}

FdoSmPhBaseObject::FdoSmPhBaseObject(
    FdoPtr<FdoSmPhDbObject> dbObject,
    FdoPtr<FdoSmPhDbObject> parent
) : 
    FdoSmPhDbElement(dbObject->GetName(), (FdoSmPhMgr*) NULL, parent, FdoSchemaElementState_Detached ),
    mOwnerName(dbObject->GetParent()->GetName()),
    mDatabaseName(dbObject->GetParent()->GetParent()->GetName()),
    mDbObject(dbObject),
    mBaseRefCount(1)
{
}

FdoSmPhBaseObject::FdoSmPhBaseObject(void)
{
}

FdoSmPhBaseObject::~FdoSmPhBaseObject(void)
{
}


const FdoSmPhDbObject* FdoSmPhBaseObject::RefDbObject() const
{
    FdoSmPhDbObjectP rootObject = ((FdoSmPhBaseObject*) this)->GetDbObject();

    return (FdoSmPhDbObject*) rootObject;
}

FdoSmPhDbObjectP FdoSmPhBaseObject::GetDbObject()
{
    const FdoSmSchemaElement* parent = GetParent();
    FdoSmPhOwnerP owner;

    if ( (wcslen(GetName()) > 0) && (!mDbObject) ) {
        while ( parent && !owner ) {
            owner = ((FdoSmSchemaElement*)parent)->SmartCast<FdoSmPhOwner>();
            parent = parent->GetParent();
        }

        if ( owner ) 
            // Use FindReferencedDbObject since it sets up base objects to be bulk loaded.
            mDbObject = owner->FindReferencedDbObject( FdoSmPhDbElement::GetName(), GetOwnerName(), GetDatabaseName() );
        else
            mDbObject = GetManager()->FindDbObject( FdoSmPhDbElement::GetName(), GetOwnerName(), GetDatabaseName() );
    }

    return mDbObject;
}

FdoString* FdoSmPhBaseObject::GetName() const
{
    if ( mQName == L"" ) {
        mQName = FdoStringP(L"\"") + GetOwnerName() + L"\".\"" + FdoSmPhDbElement::GetName() + L"\"";

        if ( GetDatabaseName() != L"" ) {
            mQName = FdoStringP(L"\"") + GetDatabaseName() + L"\"." + mQName;
        }
    }

    return mQName;
}

FdoStringP FdoSmPhBaseObject::GetObjectName() const
{
    return FdoSmPhDbElement::GetName();
}

FdoStringP FdoSmPhBaseObject::GetOwnerName() const
{
    return mOwnerName;
}

FdoStringP FdoSmPhBaseObject::GetDatabaseName() const
{
    return mDatabaseName;
}

FdoInt32 FdoSmPhBaseObject::GetBaseRefCount() const
{
    return mBaseRefCount;
}

void FdoSmPhBaseObject::AddBaseRef()
{
    mBaseRefCount++;
}
