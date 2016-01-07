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
 */

#include "stdafx.h"

// FdoRdbmsInsertCommand
#include "FdoRdbmsGetSchemaNamesCommand.h"
#include "FdoRdbmsSchemaUtil.h"

//////////////////////////////////////////////////////////////////////////
// Class FdoRdbmsGetSchemaNamesCommand

/*  Do not implement the copy constructor to avoid a lot of problems. BK
FdoRdbmsGetSchemaNamesCommand::FdoRdbmsGetSchemaNamesCommand(const FdoRdbmsGetSchemaNamesCommand &right)
{
}
*/

FdoRdbmsGetSchemaNamesCommand::FdoRdbmsGetSchemaNamesCommand() :
    mRdbmsConnection( NULL )
{
}

FdoRdbmsGetSchemaNamesCommand::FdoRdbmsGetSchemaNamesCommand(FdoIConnection *connection)
{
	mFdoConnection = connection;
	FDO_SAFE_ADDREF(mFdoConnection.p);

    FdoRdbmsConnection *conn  = static_cast<FdoRdbmsConnection*>(connection);
    if( conn )
        mRdbmsConnection = conn->GetDbiConnection();
}

FdoRdbmsGetSchemaNamesCommand::FdoRdbmsGetSchemaNamesCommand(DbiConnection *connection)
{
    mRdbmsConnection = connection;
}

FdoRdbmsGetSchemaNamesCommand::~FdoRdbmsGetSchemaNamesCommand()
{
}

FdoStringCollection* FdoRdbmsGetSchemaNamesCommand::Execute()
{
    if( NULL == mRdbmsConnection )
    {
        throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));
    }

    FdoSchemaManagerP pschemaManager = mRdbmsConnection->GetSchemaUtil()->GetSchemaManager();
    FdoPtr<FdoStringCollection> featureClasses = pschemaManager->GetSchemaNames();

    return FDO_SAFE_ADDREF(featureClasses.p);
}

