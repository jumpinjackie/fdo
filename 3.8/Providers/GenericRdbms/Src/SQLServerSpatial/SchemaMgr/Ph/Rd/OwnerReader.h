#ifndef FDOSMPHRDSQSOWNERREADER_H
#define FDOSMPHRDSQSOWNERREADER_H		1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include <Sm/Ph/Rd/OwnerReader.h>

// This class reads all owners (schemas) for the given database instance.

class FdoSmPhRdSqsOwnerReader : public FdoSmPhRdOwnerReader
{
public:
	// Create and execute the reader
	FdoSmPhRdSqsOwnerReader(
        FdoSmPhDatabaseP database,    // get the owners for this database instance
        FdoStringP ownerName = L""    // owner name. When set, this reader
                                      // retrieves a single row for this owner. When L""
                                      // all objects in the database instance are read.
    );

    // Returns true if current owner has MetaSchema tables.
    FdoBoolean GetHasMetaSchema();
 
	// Returns the description of the current owner from F_SCHEMAINFO
	FdoStringP GetDescription();

    // Deactivates the reader.
	~FdoSmPhRdSqsOwnerReader(void);

protected:
    FdoSmPhReaderP MakeQueryReader(
        FdoSmPhDatabaseP database,
        FdoStringP ownerName = L""
    ); 

    FdoSmPhReaderP MakeHasMetaSchemaReader(
        FdoSmPhDatabaseP database,
        FdoStringP ownerName 
    );

    FdoSmPhRowP MakeBinds( 
        FdoSmPhMgrP mgr, 
        FdoStringP ownerName 
    );
};

typedef FdoPtr<FdoSmPhRdSqsOwnerReader> FdoSmPhRdSqsOwnerReaderP;

#endif
