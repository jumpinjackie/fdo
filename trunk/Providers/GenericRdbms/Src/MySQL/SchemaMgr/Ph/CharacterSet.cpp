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
#include "Database.h"
#include "CharacterSet.h"
#include <FdoCommonStringUtil.h>
#include <Sm/Ph/Rd/CharacterSetReader.h>

FdoSmPhMySqlCharacterSet::FdoSmPhMySqlCharacterSet(
    FdoStringP name,
    const FdoSmPhDatabase* pDatabase,
    FdoSmPhRdCharacterSetReader* reader
) :
    FdoSmPhCharacterSet(name, pDatabase, reader)
{
    mCharLen = FdoCommonStringUtil::StringToInt64( reader->GetString(L"", L"maxlen") );
}

FdoSmPhMySqlCharacterSet::~FdoSmPhMySqlCharacterSet(void)
{
}

FdoInt64 FdoSmPhMySqlCharacterSet::GetCharLen() const
{
    return mCharLen;
}

bool FdoSmPhMySqlCharacterSet::Add()
{
    return false;
}

bool FdoSmPhMySqlCharacterSet::Modify()
{
    return false;
}

bool FdoSmPhMySqlCharacterSet::Delete()
{
    return false;
}
