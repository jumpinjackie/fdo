// 
//  
//  Copyright (C) 2004-2014  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include "stdafx.h"

ShpGetClassNamesCommand::ShpGetClassNamesCommand(FdoIConnection *connection) :
    FdoCommonCommand<FdoIGetClassNames, ShpConnection>(connection), mSchemaName((wchar_t*)NULL)
{
}

ShpGetClassNamesCommand::~ShpGetClassNamesCommand(void)
{
}

FdoString* ShpGetClassNamesCommand::GetSchemaName()
{
    return mSchemaName;
}

void ShpGetClassNamesCommand::SetSchemaName(FdoString* value)
{
    mSchemaName = value;
}

FdoStringCollection* ShpGetClassNamesCommand::Execute()
{
    //NULL or "Default" is acceptable
    if (mSchemaName == FdoStringP::mNullString || mSchemaName == FdoStringP::mEmptyString || mSchemaName == L"Default")
    {
        return mConnection->GetClassNames();
    }
    else
    {
        throw FdoException::Create(NlsMsgGet(SHP_SCHEMA_NOT_FOUND, "Schema '%1$ls' not found.", (FdoString*)mSchemaName));
    }
}