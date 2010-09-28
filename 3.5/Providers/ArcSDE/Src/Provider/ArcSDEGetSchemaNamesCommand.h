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

#ifndef ARCSDEGETSCHEMANAMESCOMMAND_H
#define ARCSDEGETSCHEMANAMESCOMMAND_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

// The GetSchemaNames command retrieves a list of the available schemas
// from the connection. The Execute operation returns a FdoStringCollection object.
class ArcSDEGetSchemaNamesCommand : public ArcSDECommand<FdoIGetSchemaNames>
{
    friend class ArcSDEConnection;

private:
    //
    // Prevent the use of the copy constructor by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ArcSDEGetSchemaNamesCommand(const ArcSDEGetSchemaNamesCommand &right);

	// Prevent the use of the Assignment Operation by defining it and not implementing it.
	// DO NOT IMPLEMENT
	ArcSDEGetSchemaNamesCommand & operator=(const ArcSDEGetSchemaNamesCommand &right);

    // Constructs an instance of a GetSchemaNames command using
    // the specified arguments.
    ArcSDEGetSchemaNamesCommand(FdoIConnection* connection = NULL);

protected:
    // Default destructor for GetSchemaNames command.
    virtual ~ArcSDEGetSchemaNamesCommand();

public:
    //
    // Executes the get schema names command and returns a
    // string collection. 
    virtual FdoStringCollection* Execute();
};

#endif  // ARCSDEGETSCHEMANAMESCOMMAND_H

