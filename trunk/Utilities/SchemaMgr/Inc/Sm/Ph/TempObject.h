#ifndef FDOSMPHTEMPOBJECT_H
#define FDOSMPHTEMPOBJECT_H		1
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

#include <Sm/Ph/DbObject.h>

// This class represents a temporary database object, one that is 
// not in any of the databases for the current connection.
//
// The main purpose of this class is to support readers that do not 
// read from a permanent database object.
class FdoSmPhTempObject : virtual public FdoSmPhDbObject
{
public:
    /// Create an instance of a temporary database object..
	//
    /// Parameters:
    /// 	mgr: Physical Schema Manager
    FdoSmPhTempObject(FdoPtr<FdoSmPhMgr> mgr);

	~FdoSmPhTempObject(void);

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhTempObject() {}

    virtual bool Add();
    virtual bool Modify();
    virtual bool Delete();

};

typedef FdoPtr<FdoSmPhTempObject> FdoSmPhTempObjectP;

#endif


