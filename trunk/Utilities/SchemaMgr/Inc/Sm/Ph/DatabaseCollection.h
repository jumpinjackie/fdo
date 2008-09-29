#ifndef FDOSMPHDATABASECOLLECTION_H
#define FDOSMPHDATABASECOLLECTION_H		1
//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/../NamedCollection.h>
#include <Sm/Ph/Database.h>

// Represents a collection of Database Objects.

class FdoSmPhDatabaseCollection : public FdoSmNamedCollection<FdoSmPhDatabase>
{
public:
	FdoSmPhDatabaseCollection(void) :
		FdoSmNamedCollection<FdoSmPhDatabase>(NULL)
	{}
	~FdoSmPhDatabaseCollection(void) {}
};

typedef FdoPtr<FdoSmPhDatabaseCollection> FdoSmPhDatabasesP;

#endif


