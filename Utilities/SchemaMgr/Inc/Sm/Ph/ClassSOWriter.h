#ifndef FDOSMPHCLASSSOWRITER_H
#define FDOSMPHCLASSSOWRITER_H		1
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

#include <Sm/Ph/SOWriter.h>

// writes Schema Options rows for a given class.

class FdoSmPhClassSOWriter : public FdoSmPhElementSOWriter
{
public:
	FdoSmPhClassSOWriter(FdoSmPhMgrP mgr);

    /// Writer methods:
	void Add(FdoStringP schemaName, FdoStringP className);
	void Modify(FdoStringP schemaName, FdoStringP className);
	void Delete(FdoStringP schemaName, FdoStringP className);

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhClassSOWriter() {}

	virtual ~FdoSmPhClassSOWriter(void) {};
};

typedef FdoPtr<FdoSmPhClassSOWriter> FdoSmPhClassSOWriterP;

#endif


