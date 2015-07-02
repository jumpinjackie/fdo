#ifndef FDOSMPHCOMMANDWRITER_H
#define FDOSMPHCOMMANDWRITER_H		1
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

#include <Sm/Ph/Writer.h>

// This class is used to write data to the MetaSchema tables.
//
// To Add a row:
//      - call the various setter functions to set the fields for the row.
//          - if the setter for a field is not called, the field's default
//            value is used.
//      - call Add() to add the row.
//
// To modify a row
//      - call the various setter functions. to set the fields for the row.
//      - call Modify() to modify the row, only fields whose setters have
//        been called are modified
//
// To delete a row
//      - call Delete().

class FdoSmPhCommandWriter : public FdoSmPhWriter
{
public:
	
	~FdoSmPhCommandWriter(void);

protected:
    /// unused constructor needed only to build on Linux
    FdoSmPhCommandWriter() {}

    /// Create the writer based on an inner writter
	//
    /// Parameters:
    /// 	subWriter: the inner writer.
    FdoSmPhCommandWriter(FdoSmPhWriterP subWriter);
    
    /// Create the writer based on a table specification.
	//
    /// Parameters:
    /// 	row: the table that is written to.
    /// 	mgr: holds the connection for the datastore to query.
	FdoSmPhCommandWriter(FdoSmPhRowP row , FdoSmPhMgrP mgr);

private:
};

typedef FdoPtr<FdoSmPhCommandWriter> FdoSmPhCommandWriterP;

#endif


