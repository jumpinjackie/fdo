#ifndef FDOSMPHSQSSCHEMACOLLECTION_H
#define FDOSMPHSQSSCHEMACOLLECTION_H		1
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

#include <Sm/NamedCollection.h>
#include "Schema.h"

// Represents a collection of physical Schema Objects.

class FdoSmPhSqsSchemaCollection : public FdoSmNamedCollection<FdoSmPhSqsSchema>
{
public:
	FdoSmPhSqsSchemaCollection(const FdoSmSchemaElement* pParent) :
		FdoSmNamedCollection<FdoSmPhSqsSchema>(pParent)
	{}
	~FdoSmPhSqsSchemaCollection(void) {}

protected:
    /// unused constructor needed only to build on Linux
    FdoSmPhSqsSchemaCollection() {}
};

typedef FdoPtr<FdoSmPhSqsSchemaCollection> FdoSmPhSqsSchemasP;

#endif


