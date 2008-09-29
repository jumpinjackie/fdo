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
#include "FdoRdbmsDescribeSchemaMappingCommand.h"

#include "FdoRdbmsSchemaUtil.h"

#ifndef ASSERT
#ifdef _DEBUG
    #include <CrtDbg.h>
    #define ASSERT  _ASSERT
#else
    #define ASSERT(x) {}
#endif
#endif
//////////////////////////////////////////////////////////////////////////
// Class FdoRdbmsDescribeSchemaMappingCommand

/*  Do not implement the copy constructor to avoid a lot of problems. BK
FdoRdbmsDescribeSchemaMappingCommand::FdoRdbmsDescribeSchemaMappingCommand(const FdoRdbmsDescribeSchemaCommand &right)
{
}
*/

FdoRdbmsDescribeSchemaMappingCommand::FdoRdbmsDescribeSchemaMappingCommand() :
    mRdbmsConnection( NULL ),
    mIncludeDefaults( false )
{
}

FdoRdbmsDescribeSchemaMappingCommand::FdoRdbmsDescribeSchemaMappingCommand(FdoIConnection *connection) :
    mIncludeDefaults( false )
{
    FdoRdbmsConnection *conn  = static_cast<FdoRdbmsConnection*>(connection);
    if( conn )
        mRdbmsConnection = conn->GetDbiConnection();
}

FdoRdbmsDescribeSchemaMappingCommand::FdoRdbmsDescribeSchemaMappingCommand(DbiConnection *connection)
{
    mRdbmsConnection = connection;
}

FdoRdbmsDescribeSchemaMappingCommand::~FdoRdbmsDescribeSchemaMappingCommand()
{
}

FdoString* FdoRdbmsDescribeSchemaMappingCommand::GetSchemaName()
{
    return mSchemaName;
}


void FdoRdbmsDescribeSchemaMappingCommand::SetSchemaName(FdoString* value)
{
    mSchemaName = value;
}

FdoBoolean FdoRdbmsDescribeSchemaMappingCommand::GetIncludeDefaults()
{
    return mIncludeDefaults;
}

void FdoRdbmsDescribeSchemaMappingCommand::SetIncludeDefaults( FdoBoolean includeDefaults )
{
    mIncludeDefaults = includeDefaults;
}

FdoPhysicalSchemaMappingCollection* FdoRdbmsDescribeSchemaMappingCommand::Execute()
{
    if( NULL == mRdbmsConnection )
    {
        throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));
    }

    try
    {
        FdoSchemaManagerP  pSchemaManager = mRdbmsConnection->GetSchemaUtil()->GetSchemaManager();
        FdoSchemaMappingsP mappings = pSchemaManager->GetSchemaMappings( mSchemaName, mIncludeDefaults );

        return FDO_SAFE_ADDREF((FdoPhysicalSchemaMappingCollection*) mappings);
    }
    catch (FdoSchemaException *ex)
    {
        // Catch and rethrow FdoSchemaExceptions here so the following
        // catch for FdoExceptions doesn't get them.
        throw ex;
    }
    catch (FdoException *ex)
    {
        throw FdoSchemaException::Create(ex->GetExceptionMessage(), ex);
    }
}

