#ifndef FdoSmPhRdOraOdbcColumnReader_H
#define FdoSmPhRdOraOdbcColumnReader_H      1
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

// Odbc column reader implementation
class FdoSmPhRdOraOdbcColumnReader : public FdoSmPhRdColumnReader
{
public:
    FdoSmPhRdOraOdbcColumnReader (FdoSmPhMgrP mgr, FdoSmPhDbObjectP dbObject);

	// Generates the Schema Manager type for each row retrieved, from the 
    // type retrieved from Odbc.
	virtual bool ReadNext();

    // Get the type enum for the current column
    virtual FdoSmPhColType GetType();

private:

    FdoSmPhColType String2Type( FdoString* colTypeString, int size, int scale );

    FdoSmPhColType      mColType;
};

#endif
