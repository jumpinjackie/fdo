#ifndef FDOSMPHRDVIEWREADER_H
#define FDOSMPHRDVIEWREADER_H		1
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
#include <Sm/Ph/Reader.h>
#include <Sm/Ph/RowCollection.h>
#include <Sm/Ph/Mgr.h>

// This abstract class reads views. In particular, it reads
// the creation syntax for each view (select statement that 
// the view is based on).

class FdoSmPhRdViewReader : public FdoSmPhReader
{
public:

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdViewReader() {}

    // Creates a reader for all views in the given owner
	//
	FdoSmPhRdViewReader(
        FdoSmPhReaderP reader,      // inner reader created by implementing class
        FdoSmPhOwnerP owner        // get the views for this owner
    );

    // Creates a reader for a particular view.
	//
	FdoSmPhRdViewReader(
        FdoSmPhReaderP reader,      // inner reader created by implementing class
        FdoSmPhDbObjectP dbObject   // the view to read. Retrieves no rows if this
                                    // dbObject is not a view.
    );

    // TODO: read based on a join.

    /// Deactivates the reader.
	virtual ~FdoSmPhRdViewReader(void);

    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per view attribute.
    virtual FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

    FdoSmPhOwnerP mOwner;
    FdoSmPhDbObjectP mDbObject;
};

typedef FdoPtr<FdoSmPhRdViewReader> FdoSmPhRdViewReaderP;

#endif


