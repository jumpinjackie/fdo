#ifndef _FDORDBMSACTIVATESPATIALCONTEXT_H_
#define _FDORDBMSACTIVATESPATIALCONTEXT_H_
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
#include "Fdo/Commands/SpatialContext/IActivateSpatialContext.h"

/// <summary>The IActivateSpatialContext interface defines the ActivateSpatialContext
/// command, which activates a specified spatial context. Input to the command
/// is simply the name of the context to activate.</summary>
class FdoRdbmsActivateSpatialContext : public FdoRdbmsCommand<FdoIActivateSpatialContext>
{
    friend class FdoRdbmsConnection;

protected:

    //    Constructs an instance of a FdoRdbmsActivateSpatialContext using the
    //    specified arguments.
    FdoRdbmsActivateSpatialContext (FdoIConnection *connection);

public:
    /// <summary>Gets the name of the context to activate as a String.</summary>
    /// <returns>Returns the name of the spatial context</returns>
    FDORDBMS_API virtual FdoString* GetName();

    /// <summary>Sets the name of the context to activate as a String.</summary>
    /// <param name="value">Input the name of the spatial context</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetName(FdoString* value);

    /// <summary>Executes the ActivateSpatialContext command. An exception is thrown
    /// if the spatial context does not exist.</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void Execute();

protected:
	FdoRdbmsActivateSpatialContext() {};
    virtual ~FdoRdbmsActivateSpatialContext();

private:
    FdoStringP                    mSCName;
};
#endif
