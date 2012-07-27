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
 * Revision Control Modification History
 *
 *         $Id: //providers/GenericRdbms/src/Odbc/SchemaMgr/Ph/Rd/ConstraintReader.cpp#2 $
 *     $Author: br $
 *   $DateTime: 2005/08/24 11:32:56 $
 *     $Change: 8166 $
 *
 */

#include "stdafx.h"
#include "ConstraintReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdOdbcConstraintReader::FdoSmPhRdOdbcConstraintReader(
    FdoSmPhOwnerP owner,
    FdoStringP constraintName
) :
FdoSmPhRdConstraintReader(owner->GetManager(), (FdoSmPhRowCollection*) NULL )
{
    SetRows(MakeRows(owner->GetManager()));
}

FdoSmPhRdOdbcConstraintReader::FdoSmPhRdOdbcConstraintReader(
    FdoSmPhOwnerP owner,
	FdoStringP tableName,
    FdoStringP constraintType
) :
    FdoSmPhRdConstraintReader(owner->GetManager(), (FdoSmPhRowCollection*) NULL)
{
    SetRows(MakeRows(owner->GetManager()));
}

FdoSmPhRdOdbcConstraintReader::~FdoSmPhRdOdbcConstraintReader(void)
{
}


bool FdoSmPhRdOdbcConstraintReader::ReadNext()
{
    // No constraint support in ODBC Provider.
    return false;
}
