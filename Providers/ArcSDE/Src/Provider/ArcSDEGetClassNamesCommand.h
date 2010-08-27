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

#ifndef ARCSDEGETCLASSNAMESCOMMAND_H
#define ARCSDEGETCLASSNAMESCOMMAND_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

// The GetClassNames command retrieves a list of the available feature classes
// from the connection. The Execute operation returns a FdoStringCollection object.
class ArcSDEGetClassNamesCommand : public ArcSDECommand<FdoIGetClassNames>
{
    friend class ArcSDEConnection;

private:
    //
    // Prevent the use of the copy constructor by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ArcSDEGetClassNamesCommand(const ArcSDEGetClassNamesCommand &right);

	// Prevent the use of the Assignment Operation by defining it and not implementing it.
	// DO NOT IMPLEMENT
	ArcSDEGetClassNamesCommand & operator=(const ArcSDEGetClassNamesCommand &right);

    // Constructs an instance of a GetClassNames command using
    // the specified arguments.
    ArcSDEGetClassNamesCommand(FdoIConnection* connection);

protected:
    // Default destructor for GetClassNames command.
    virtual ~ArcSDEGetClassNamesCommand();

public:
    //
    // Gets the name of the schema from which to get class names. This is optional,
    // if not specified execution of the command will get class names from all schemas.
    virtual const wchar_t* GetSchemaName();

    // Sets the name of the schema from which to get class names. This is optional, if not
    // specified execution of the command will get class names from all schemas.
    virtual void SetSchemaName(const wchar_t* value);

    // Executes the get feature class names command and returns a
    // FdoStringCollection. If the specified schema name does not exist
    // the Execute method will throw an exception.
    virtual FdoStringCollection* Execute();

protected:
    FdoStringP                            mSchemaName;
};

#endif  // ARCSDEGETCLASSNAMESCOMMAND_H

