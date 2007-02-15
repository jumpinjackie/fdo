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
    mDatabaseName(databaseName)
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
    mDbObject(dbObject)
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
    if ( (wcslen(GetName()) > 0) && (!mDbObject) ) 
        mDbObject = GetManager()->FindDbObject( GetName(), GetOwnerName(), GetDatabaseName() );

    return mDbObject;
}

FdoStringP FdoSmPhBaseObject::GetOwnerName() const
{
    return mOwnerName;
}

FdoStringP FdoSmPhBaseObject::GetDatabaseName() const
{
    return mDatabaseName;
}

