 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdafx.h"

#include "FdoRdbmsConnection.h"
#include "FdoRdbmsException.h"
#include "FdoRdbmsDestroySpatialContext.h"

FdoRdbmsDestroySpatialContext::FdoRdbmsDestroySpatialContext(FdoIConnection *connection)
: FdoRdbmsCommand<FdoIDestroySpatialContext>(connection)
{
}

FdoRdbmsDestroySpatialContext::~FdoRdbmsDestroySpatialContext()
{
}

FdoString* FdoRdbmsDestroySpatialContext::GetName()
{
    return mSCName;
}

void FdoRdbmsDestroySpatialContext::SetName(FdoString* value)
{
    mSCName = value;
}

void FdoRdbmsDestroySpatialContext::Execute()
{
    try
    {
        FdoSchemaManagerP smgr = mFdoConnection->GetSchemaManager();

        // Make sure that we do not leave an improper active spatial context.
        bool deletingActiveSc = false;
        FdoString * activeScName = mFdoConnection->GetActiveSpatialContextName();
        if (NULL != activeScName && wcscmp(activeScName, mSCName) == 0)
            deletingActiveSc = true;

        smgr->DestroySpatialContext(mSCName);

        if (deletingActiveSc)
            mFdoConnection->SetDefaultActiveSpatialContextName();
    }
    catch (FdoRdbmsException *ex)
    {
       throw ex;
    }
}
