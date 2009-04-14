#ifndef FDOSMPHOPTIONSREADER_H
#define FDOSMPHOPTIONSREADER_H		1
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

#include <Sm/Ph/Reader.h>

#include <Sm/Ph/ClassPropertyReader.h>
/*
#include <Sm/Ph/IndexPropertyReader.h>
#include <Sm/Ph/ClassSADReader.h>
#include <Sm/Ph/TableCollection.h>
*/
// Retrieves all classes for a given Feature schema. 

class FdoSmPhOptionsReader :
	public FdoSmPhReader
{
public:
    /// Create and execute a query to get the f_options contents.
    /// Call ReadNext() to read each option.
	//
    /// Parameters:
    /// 	mgr: physical schema manager
	FdoSmPhOptionsReader(FdoSmPhMgrP mgr, FdoStringP owner = L"" );

    /// Deactivate the options reader
	~FdoSmPhOptionsReader(void);

    /// Various getters

    /// Option name
	FdoStringP GetName();

    /// Option value
	FdoStringP GetValue();

protected:
    /// unused constructor only for building on Linux
    FdoSmPhOptionsReader() {}

    /// function for creating inner reader
    FdoSmPhReaderP MakeReader( FdoSmPhMgrP mgr, FdoStringP owner = L"" );
};

typedef FdoPtr<FdoSmPhOptionsReader> FdoSmPhOptionsReaderP;

#endif


