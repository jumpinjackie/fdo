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
#include "FdoRdbmsDestroySchemaCommand.h"
#include "../../SchemaMgr/Ph/Mgr.h"

// Class FdoRdbmsDestroySchemaCommand
/*  Do not implement the copy constructor to avoid alot of problem. BK
FdoRdbmsDestroySchemaCommand::FdoRdbmsDestroySchemaCommand(const FdoRdbmsDestroySchemaCommand &right)
{
}
*/

FdoRdbmsDestroySchemaCommand::FdoRdbmsDestroySchemaCommand()
{
}

FdoRdbmsDestroySchemaCommand::FdoRdbmsDestroySchemaCommand(FdoIConnection *connection)
{
    mRdbmsConnection = FDO_SAFE_ADDREF(static_cast<FdoRdbmsConnection*>(connection));
}

FdoRdbmsDestroySchemaCommand::~FdoRdbmsDestroySchemaCommand()
{
}

FdoString* FdoRdbmsDestroySchemaCommand::GetSchemaName()
{
    return mSchemaName;
}

void FdoRdbmsDestroySchemaCommand::SetSchemaName(FdoString* value)
{
    mSchemaName = value;
}

void FdoRdbmsDestroySchemaCommand::Execute()
{

    if( ((FdoRdbmsConnection*) NULL) == mRdbmsConnection )
    {   
        throw FdoSchemaException::Create();
        //TODO: proper message: throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));
    }

    if ( L"" == mSchemaName )
    {
        throw FdoSchemaException::Create();
        //TODO: proper message: throw FdoSchemaException::Create( NlsMsgGet( FDORDBMS_163, "Cannot destroy schema; no schema name specified") );
    }

    FdoSchemaManagerP      pSchemaManager = mRdbmsConnection->CreateSchemaManager();

    pSchemaManager->DestroySchema( mSchemaName );
}
