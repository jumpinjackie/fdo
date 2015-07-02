#ifndef FDOSMPHRDCHARACTERSETREADER_H
#define FDOSMPHRDCHARACTERSETREADER_H		1
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

// This abstract class reads CharacterSets for a given database.
// Providers that support character sets can provide an implementing class.

class FdoSmPhRdCharacterSetReader : public FdoSmPhReader
{
public:

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdCharacterSetReader() {}

    /// Create and execute the CharacterSet reader:

    FdoSmPhRdCharacterSetReader(
        FdoSmPhReaderP      reader,
        FdoStringP characterSetName
    );

    /// Deactivates the reader.
	~FdoSmPhRdCharacterSetReader(void);

protected:
    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per CharacterSet attribute.
    FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

private:
   FdoStringP mCharacterSetName;
};

typedef FdoPtr<FdoSmPhRdCharacterSetReader> FdoSmPhRdCharacterSetReaderP;

#endif


