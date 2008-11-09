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
#include "NullIndicator.h"


FdoSmPhGrdNullIndicator::FdoSmPhGrdNullIndicator(
	GdbiConnection* connection 
): 
	FdoSmPhNullIndicator(), 
    mConnection(connection)
{
    connection->GetCommands()->alcnullind(1, &mNullInd);
    SetNull();
}

FdoSmPhGrdNullIndicator::~FdoSmPhGrdNullIndicator(void)
{
    free(mNullInd);
}

_NI_TYPE* FdoSmPhGrdNullIndicator::GetDbIndicator()
{
    return mNullInd;
}

void FdoSmPhGrdNullIndicator::SetNull()
{
    mConnection->GetCommands()->set_null( mNullInd, 0, 0);
}

void FdoSmPhGrdNullIndicator::SetNNull()
{
    mConnection->GetCommands()->set_nnull( mNullInd, 0, 0);
}

bool FdoSmPhGrdNullIndicator::IsNull()
{
    return ( mConnection->GetCommands()->is_null(mNullInd, 0) != 0 );
}
