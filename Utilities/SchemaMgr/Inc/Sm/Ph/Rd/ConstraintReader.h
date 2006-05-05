#ifndef FDOSMPHRDCONSTRAINTREADER_H
#define FDOSMPHRDCONSTRAINTREADER_H		1
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

// This abstract class reads database constraints. Each provider must 
// provide its own implementing class.

class FdoSmPhRdConstraintReader : public FdoSmPhReader
{
protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdConstraintReader() {}

    /// Create the constraint reader
	//
	FdoSmPhRdConstraintReader(
        FdoSmPhReaderP reader    // inner reader created by implementing class
    );

    /// Deactivates the reader.
	~FdoSmPhRdConstraintReader(void);

    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per constraint attribute.
    FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );
};

typedef FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhRdConstraintReaderP;

#endif


