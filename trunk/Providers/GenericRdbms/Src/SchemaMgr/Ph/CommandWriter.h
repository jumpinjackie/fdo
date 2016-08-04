#ifndef FDOSMPHGRDCOMMANDWRITER_H
#define FDOSMPHGRDCOMMANDWRITER_H		1
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

#include <Sm/Ph/CommandWriter.h>
#include "Mgr.h"

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

class FdoSmPhGrdCommandWriter : public FdoSmPhCommandWriter
{
public:
	// Create the writer.
	//
	// Parameters:
	//		row: the table that is written to.
	//		mgr: holds the connection for the database to query.
	FdoSmPhGrdCommandWriter(FdoSmPhRowP row , FdoSmPhMgrP mgr);
	
	~FdoSmPhGrdCommandWriter(void);

    // Add the current row to the table.
	virtual void Add();

    // Modify a row or set of rows.
    //
    // Parameters:
    //      sClauses: a where clause that identifies the row(s) to modify.
    virtual void Modify( FdoStringP sClauses );

    // Delete a row or set of rows.
    //
    // Parameters:
    //      sClauses: a where clause that identifies the row(s) to delete.
    virtual void Delete( FdoStringP sClauses );

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhGrdCommandWriter() {}

    // Binds the given statement to the given fields.
    void Bind( GdbiStatement* stmt, FdoSmPhFieldsP binds, bool rebind = false );

private:
    GdbiStatement* mInsertStmt;
};

#endif
