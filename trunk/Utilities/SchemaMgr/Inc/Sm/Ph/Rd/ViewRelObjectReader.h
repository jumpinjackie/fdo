#ifndef FDOSMPHRDVIEWRELOBJECTREADER_H
#define FDOSMPHRDVIEWRELOBJECTREADER_H	1
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
#include <Sm/Ph/Rd/Reader.h>
#include <Sm/Ph/RowCollection.h>
#include <Sm/Ph/DbObjType.h>
#include <Sm/Ph/Mgr.h>

// This abstract class reads base objects. Each provider must 
// provide its own implementing class.
// A base object is a database object that another database object
// depends on. Typically a view can be based on a table or another view.

class FdoSmPhRdViewRelationsObjectReader : public FdoSmPhRdReader
{
public:

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdViewRelationsObjectReader() {}

    // Creates a reader for all base object references in the given owner
	//
	FdoSmPhRdViewRelationsObjectReader(
        FdoSmPhReaderP reader,      // inner reader created by implementing class
        FdoSmPhOwnerP owner        // get the base objects for this owner
    );

    /// Deactivates the reader.
	virtual ~FdoSmPhRdViewRelationsObjectReader(void);

    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per base object attribute.
    virtual FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

    FdoSmPhOwnerP mOwner;
};

typedef FdoPtr<FdoSmPhRdViewRelationsObjectReader> FdoSmPhRdViewRelationsObjectReaderP;

#endif


