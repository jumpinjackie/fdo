#ifndef FDOSMPHRDCOLLATIONEADER_H
#define FDOSMPHRDCOLLATIONREADER_H		1
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
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/RowCollection.h>

// This abstract class reads Collations for a given database.
// Providers that support character sets and collations can provide an implementing 
// class.

class FdoSmPhRdCollationReader : public FdoSmPhReader
{
public:

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdCollationReader() {}

    /// Create and execute the Collation reader:

    FdoSmPhRdCollationReader(
        FdoSmPhReaderP      reader,
        FdoStringP CollationName
    );

    /// Deactivates the reader.
	~FdoSmPhRdCollationReader(void);

protected:
    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per Collation attribute.
    FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

private:

   FdoStringP mCollationName;
};

typedef FdoPtr<FdoSmPhRdCollationReader> FdoSmPhRdCollationReaderP;

#endif


