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
#include "Fdo/Pvc/FdoRdbmsPvcOperation.h"

FdoRdbmsPvcOperation::FdoRdbmsPvcOperation()
{
}
FdoRdbmsPvcOperation::FdoRdbmsPvcOperation(FdoSmLpClassDefinition *currentClass)
{
    mProperties = FdoPropertyValueCollection::Create();
    mCurrentClass = currentClass;
    mScope = L"";
    mIsLinked = false;
}

FdoRdbmsPvcOperation::FdoRdbmsPvcOperation(FdoSmLpClassDefinition *currentClass, FdoPropertyValueCollection *props, const wchar_t* scope )
{
    props->AddRef();
    mProperties = props;
    mCurrentClass = currentClass;
    mScope = scope;
    mIsLinked = true;
}

FdoPropertyValueCollection *FdoRdbmsPvcOperation::GetProperties()
{
    mProperties->AddRef();
    return mProperties;
}
FdoSmLpClassDefinition *FdoRdbmsPvcOperation::GetClass()

{
    return mCurrentClass;
}

const wchar_t* FdoRdbmsPvcOperation::GetScope()
{
    return (const wchar_t*)mScope;
}

bool FdoRdbmsPvcOperation::IsLinked() { return mIsLinked; }

void FdoRdbmsPvcOperation::Dispose()
{
    delete this;
}

FdoRdbmsPvcOperation::~FdoRdbmsPvcOperation()
{
    mProperties->Release();
}

