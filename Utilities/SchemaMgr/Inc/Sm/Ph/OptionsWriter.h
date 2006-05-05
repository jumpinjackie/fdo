#ifndef FDOSMPHOPTIONSWRITER_H
#define FDOSMPHOPTIONSWRITER_H		1
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

#include <Sm/Ph/CommandWriter.h>

// This class writes options 
// to the f_options table.
class FdoSmPhOptionsWriter : public FdoSmPhWriter
{
public:

    /// Create an options writer.
	//
    /// Parameters:
    /// 	mgr: physical schema manager
	FdoSmPhOptionsWriter(FdoSmPhMgrP mgr, FdoStringP owner = L"" );

	~FdoSmPhOptionsWriter(void);
    
    /// Various field value getters

    /// Option name
	FdoStringP GetName();

    /// Option value
	FdoStringP GetValue();

    /// Corresponding setter functions. 

	void SetName( FdoStringP sValue );

    void SetValue( FdoStringP sValue );


    /// Get a field list for reading from or writing to 
    /// f_options.
	//
    /// Parameters:
    /// 	mgr: Physical Schema Manager
    static FdoSmPhRowP MakeRow( FdoSmPhMgrP mgr, FdoStringP owner = L"" );

protected:
    /// unused constructor only for building on Linux
    FdoSmPhOptionsWriter() {}

    FdoSmPhWriterP MakeWriter( FdoSmPhMgrP mgr, FdoStringP owner = L"" );
};

typedef FdoPtr<FdoSmPhOptionsWriter> FdoSmPhOptionsWriterP;

#endif


