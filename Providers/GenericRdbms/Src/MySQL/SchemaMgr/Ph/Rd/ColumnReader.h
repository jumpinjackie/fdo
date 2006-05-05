#ifndef FDOSMPHRDMYSQLCOLUMNREADER_H
#define FDOSMPHRDMYSQLCOLUMNREADER_H      1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/Rd/ColumnReader.h>
#include <Inc/Rdbi/context.h>

// MySql column reader implementation
class FdoSmPhRdMySqlColumnReader : public FdoSmPhRdColumnReader
{
public:
    FdoSmPhRdMySqlColumnReader (FdoSmPhMgrP mgr, FdoSmPhDbObjectP dbObject);

    // Deactivates the query.
    ~FdoSmPhRdMySqlColumnReader(void);

	// Generates the Schema Manager type for each row retrieved, from the 
    // type retrieved from MySql.
	virtual bool ReadNext();

    // Get the type enum for the current column
    virtual FdoSmPhColType GetType();

private:
    // Create the column reader
    FdoSmPhReaderP MakeQueryReader (FdoSmPhMgrP mgr, FdoSmPhDbObjectP dbObject);

    FdoSmPhRowP MakeBinds (FdoSmPhMgrP mgr, FdoStringP table_owner, FdoStringP table_name);

    FdoSmPhColType      mColType;
};

#endif
