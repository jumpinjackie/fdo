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
#include "Collation.h"
#include <Sm/Ph/Rd/CollationReader.h>

FdoSmPhMySqlCollation::FdoSmPhMySqlCollation(
    FdoStringP name,
    const FdoSmPhDatabase* pDatabase,
    FdoSmPhRdCollationReader* reader
) :
    FdoSmPhCollation(name, pDatabase, reader)
{
}

FdoSmPhMySqlCollation::~FdoSmPhMySqlCollation(void)
{
}

bool FdoSmPhMySqlCollation::Add()
{
    return false;
}

bool FdoSmPhMySqlCollation::Modify()
{
    return false;
}

bool FdoSmPhMySqlCollation::Delete()
{
    return false;
}
