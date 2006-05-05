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

#include "FdoRdbmsException.h"
#include "FdoRdbmsGetSpatialContexts.h"
#include "FdoRdbmsSpatialContextReader.h"

FdoRdbmsGetSpatialContexts::FdoRdbmsGetSpatialContexts(FdoIConnection *connection):
FdoRdbmsCommand<FdoIGetSpatialContexts>(connection)
{
    FdoRdbmsConnection *conn = static_cast<FdoRdbmsConnection*>(connection);
    if( conn )
        mConnection = conn->GetDbiConnection();

    mActiveOnly = false;
}

FdoRdbmsGetSpatialContexts::~FdoRdbmsGetSpatialContexts()
{
}

const bool FdoRdbmsGetSpatialContexts::GetActiveOnly()
{
    return mActiveOnly;
}

void FdoRdbmsGetSpatialContexts::SetActiveOnly(const bool value)
{
    mActiveOnly = value;
}

FdoISpatialContextReader* FdoRdbmsGetSpatialContexts::Execute()
{
    FdoRdbmsSpatialContextReader *scReader = 0;

    int                found = false;

    try
    {
        FdoSchemaManagerP smgr = mFdoConnection->GetSchemaManager();

        if ( mActiveOnly )
        {
            FdoString * activeScName = mFdoConnection->GetActiveSpatialContextName();
            if (NULL != activeScName && wcslen(activeScName) > 0)
                found = true;

            if ( !found )
                throw FdoRdbmsException::Create( FdoStringP( NlsMsgGet( FDORDBMS_325,
                                                "Active Spatial Context not found") )
                                                );
        }
        
        scReader = new FdoRdbmsSpatialContextReader(mFdoConnection, mActiveOnly);
    }
    catch (FdoRdbmsException *ex)
    {
       throw ex;
    }

    return scReader;
}
