#ifndef FDOSMPHMTASSOCIATIONREADER_H
#define FDOSMPHMTASSOCIATIONREADER_H		1
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

#include <Sm/Ph/Rd/QueryReader.h>

// This reader retrieves all the association definition maintained 
// in the f_associationdefinition table.
class FdoSmPhMtAssociationReader :
	public FdoSmPhReader
{
public:
    /// Create and execute a query to list the association definition
    /// for a property.
    /// Call ReadNext() to read each row.
	//
    /// Parameters:
    /// 	classId: id of the property's class.
    /// 	fkTableName: the associate dclass table.
	FdoSmPhMtAssociationReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, long classId, FdoStringP fkTableName );

    /// Create and execute a query to retrieve the association definition for the given tables.
    /// The query depends on the parameters below.
    /// 
    /// When pkTableName and fkTableName are both L"" then all definitions are
    /// retrieved.
	//
    /// When only fkTableName is L"" then all definitions referenced by 
    /// pkTableName are selected.
	//
    /// When only pkTableName is L"" then all definitions referencing 
    /// fkTableName are selected.
	//
    /// When neither is L"" then the select depends on bAnd:
    /// 	- if true then definitions that match the pkTableName and the
    /// 	fkTableName are selected.
    /// 	- if false then definitions that match the pkTableName or the
    /// 	fkTableName are selected.
	//
    /// Parameters:
    /// 	mgr: the current physical schema manager.
    /// 	pkTableName: primary key table name
    /// 	fkTableName: foreign key table name.
    /// 	bAnd: 
    /// 		True: select definitions for the given pkTable and fkTable.
    /// 		False: select definitions for the given pkTable or fkTable.
	FdoSmPhMtAssociationReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd );

    /// Deactivate the query
	~FdoSmPhMtAssociationReader(void);

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhMtAssociationReader() {}

    /// Helpers for creating inner query reader
    FdoSmPhReaderP MakeReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, long classId, FdoStringP fkTableName );
    FdoSmPhReaderP MakeReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd );
};

#endif


