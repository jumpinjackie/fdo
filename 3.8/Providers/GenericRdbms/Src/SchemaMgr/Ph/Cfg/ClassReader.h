#ifndef FDOSMPHCFGGRDCLASSREADER_H
#define FDOSMPHCFGGRDCLASSREADER_H		1
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

#include <Sm/Ph/Cfg/ClassReader.h>
#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>

class FdoSmPhCfgGrdClassReader : public FdoSmPhCfgClassReader
{
public:
	// Create a reader to autogenerate classes based on config document directives.
	//
	// Parameters:
	//		rows: rows and fields to retrieve.
	//		schemaName: generate classes for this schema
    //      mgr: Physical Schema Manager
	FdoSmPhCfgGrdClassReader(FdoSmPhRowsP rows, FdoStringP schemaName, FdoSmPhMgrP mgr);

    // Deactivates the query.
	~FdoSmPhCfgGrdClassReader(void);

	// Get the next row and make it current.
	virtual bool ReadNext();


protected:
    // unused constructor needed only to build on Linux
    FdoSmPhCfgGrdClassReader() {}

private:
	FdoSmPhReaderP MakeReader(FdoSmPhRowsP rows, FdoStringP schemaName, FdoSmPhMgrP mgr);

    FdoRdbmsOvSchemaMappingP       mConfigMapping;

    FdoStringP                     mDatabase;
    FdoStringP                     mOwner;
};

#endif
