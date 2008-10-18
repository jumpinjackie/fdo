#ifndef FDORDBMSGETSCHEMANAMES_H
#define FDORDBMSGETSCHEMANAMES_H        1
#ifdef _WIN32
#pragma once
#endif
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

#include <map>
#include "Inc/Util/string.h"
#include "FdoRdbmsCommand.h"
#include "FdoRdbmsConnection.h"

// The GetSchemaNames command retrieves a list of the available schemas
// from the connection. The Execute operation returns a FdoStringCollection object.
class FdoRdbmsGetSchemaNamesCommand : public FdoRdbmsCommand<FdoIGetSchemaNames>
{
    friend class FdoRdbmsConnection;
    friend class FdoRdbmsFeatureReader;

private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsGetSchemaNamesCommand(const FdoRdbmsGetSchemaNamesCommand &right);

    // Constructs a default instance of a GetSchemaNames command.
    FdoRdbmsGetSchemaNamesCommand();

    // Constructs an instance of a GetSchemaNames command using
    // the specified arguments.
    FdoRdbmsGetSchemaNamesCommand(FdoIConnection* connection);

    // Constructs an instance of a GetSchemaNames command using
    // the specified arguments.
    FdoRdbmsGetSchemaNamesCommand(DbiConnection* connection);

protected:
    // Default destructor for GetSchemaNames command.
    virtual ~FdoRdbmsGetSchemaNamesCommand();

public:
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsGetSchemaNamesCommand & operator=(const FdoRdbmsGetSchemaNamesCommand &right);

    // Executes the get schema names command and returns a
    // string collection. 
    virtual FdoStringCollection* Execute();

protected:
    DbiConnection*                        mRdbmsConnection;


	FdoPtr<FdoIConnection>			mFdoConnection;
};

#endif  // FDORDBMSGETSCHEMANAMES_H
