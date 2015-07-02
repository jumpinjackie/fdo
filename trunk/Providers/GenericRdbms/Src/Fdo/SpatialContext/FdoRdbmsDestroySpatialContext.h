#ifndef _FDORDBMSDESTROYSPATIALCONEXT_H_
#define _FDORDBMSDESTROYSPATIALCONEXT_H_
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

#include "FdoRdbms.h"
#include "FdoRdbmsCommand.h"
#include "Fdo/Commands/SpatialContext/IDestroySpatialContext.h"

/// <summary>The IDestroySpatialContext interface defines the DestroySpatialContext
/// command, which destroys an existing spatial context. Input to the command is
/// the name of the context to destroy. Destroying a spatial context destroys
/// all data stored in that context.</summary>
class FdoRdbmsDestroySpatialContext : public FdoRdbmsCommand<FdoIDestroySpatialContext>
{
    friend class FdoRdbmsConnection;

protected:

    //    Constructs an instance of a FdoRdbmsDestroySpatialContext using the
    //    specified arguments.
    FdoRdbmsDestroySpatialContext (FdoIConnection *connection);

public:

    /// <summary>Gets the name of the context to destroy as a String.</summary>
    /// <returns>Returns the name of the spatial context to be deleted</returns>
    FDORDBMS_API virtual FdoString* GetName();

    /// <summary>Sets the name of the context to destroy as a String.</summary>
    /// <param name="value">Input the name of the spatial context to be deleted</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetName(FdoString* value);

    /// <summary>Executes the DestroySpatialContext command removing all data stored
    /// in that context. An exception is thrown if the spatial context does not exist.</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void Execute();

protected:
	FdoRdbmsDestroySpatialContext() {};
    virtual ~FdoRdbmsDestroySpatialContext();

private:
    FdoStringP                    mSCName;

};
#endif
