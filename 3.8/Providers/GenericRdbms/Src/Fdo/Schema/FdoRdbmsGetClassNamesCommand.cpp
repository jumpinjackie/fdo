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
#include "FdoRdbmsGetClassNamesCommand.h"
#include "FdoRdbmsSchemaUtil.h"

//////////////////////////////////////////////////////////////////////////
// Class FdoRdbmsGetClassNamesCommand

/*  Do not implement the copy constructor to avoid a lot of problems. BK
FdoRdbmsGetClassNamesCommand::FdoRdbmsGetClassNamesCommand(const FdoRdbmsGetClassNamesCommand &right)
{
}
*/

FdoRdbmsGetClassNamesCommand::FdoRdbmsGetClassNamesCommand() :
    mRdbmsConnection( NULL )
{
}

FdoRdbmsGetClassNamesCommand::FdoRdbmsGetClassNamesCommand(FdoIConnection *connection)
{
	mFdoConnection = connection;
	FDO_SAFE_ADDREF(mFdoConnection.p);

    FdoRdbmsConnection *conn  = static_cast<FdoRdbmsConnection*>(connection);
    if( conn )
        mRdbmsConnection = conn->GetDbiConnection();
}

FdoRdbmsGetClassNamesCommand::FdoRdbmsGetClassNamesCommand(DbiConnection *connection)
{
    mRdbmsConnection = connection;
}

FdoRdbmsGetClassNamesCommand::~FdoRdbmsGetClassNamesCommand()
{
}

const wchar_t* FdoRdbmsGetClassNamesCommand::GetSchemaName()
{
    return mSchemaName;
}


void FdoRdbmsGetClassNamesCommand::SetSchemaName(const wchar_t* value)
{
    mSchemaName = value;
}

FdoStringCollection* FdoRdbmsGetClassNamesCommand::Execute()
{
    if( NULL == mRdbmsConnection )
    {
        throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));
    }

    FdoSchemaManagerP pschemaManager = mRdbmsConnection->GetSchemaUtil()->GetSchemaManager();
    FdoPtr<FdoStringCollection> featureClasses = pschemaManager->GetClassNames(mSchemaName);

    return FDO_SAFE_ADDREF(featureClasses.p);
}

