#ifndef _FDORDBMSGETSPATIALCONTEXTS_H_
#define _FDORDBMSGETSPATIALCONTEXTS_H_
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
#include "Fdo/Commands/SpatialContext/ISpatialContextReader.h"
#include "Fdo/Commands/SpatialContext/IGetSpatialContexts.h"

class DbiConnection;

/// <summary>The IGetSpatialContexts interface defines the GetSpatialContexts command,
/// which enumerates the existing spatial contexts.</summary>
class FdoRdbmsGetSpatialContexts : public FdoRdbmsCommand<FdoIGetSpatialContexts>
{
    friend class FdoRdbmsConnection;
    friend class DbiConnection;

protected:
    DbiConnection* mConnection;

    //    Constructs an instance of a FdoRdbmsGetSpatialContexts using the
    //    specified arguments.
    FdoRdbmsGetSpatialContexts (FdoIConnection *connection);

public:
    // Gets a Boolean flag that indicates if the GetSpatialContexts command
    // will return only the active spatial context or all spatial contexts. The
    // default value of this flag is false, return all spatial contexts.
    FDORDBMS_API virtual const bool GetActiveOnly();

    // Sets a Boolean flag that indicates if the GetSpatialContexts command
    // will return only the active spatial context or all spatial contexts. The
    // default value of this flag is false, return all spatial contexts.
    FDORDBMS_API virtual void SetActiveOnly(const bool value);

    /// summary>Executes the GetSpatialContexts command returning an ISpatialContextReader.</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual FdoISpatialContextReader* Execute();

protected:
    virtual ~FdoRdbmsGetSpatialContexts();

private:
     bool    mActiveOnly;
};
#endif
