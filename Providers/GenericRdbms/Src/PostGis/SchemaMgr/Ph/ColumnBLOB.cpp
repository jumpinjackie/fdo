//
// Copyright (C) 2006 Refractions Research, Inc.
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
//
#include "stdafx.h"
#include "ColumnBLOB.h"

FdoSmPhPostGisColumnBLOB::FdoSmPhPostGisColumnBLOB(FdoStringP columnName,
    FdoSchemaElementState state,
    FdoSmPhDbObject* parentObject,
    bool isNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* reader)
    : FdoSmPhPostGisColumn (reader),
      FdoSmPhColumn(columnName, L"BYTEA", state, parentObject,
        isNullable, rootColumnName)
{
    // TODO: mloskot - Verify type name "BLOB".

    // idle
}

int FdoSmPhPostGisColumnBLOB::GetRdbType()
{
    return RDBI_BLOB_REF;
}

FdoInt64 FdoSmPhPostGisColumnBLOB::GetDbBinarySize()
{
    // TODO: mloskot - PostgreSQL does not specify exact size limit.
    // Temporarily, let's use MySQL's limit: 2^32 bytes

    return 4294967296LL;
}

FdoSmPhPostGisColumnBLOB::~FdoSmPhPostGisColumnBLOB()
{
    // idle
}
