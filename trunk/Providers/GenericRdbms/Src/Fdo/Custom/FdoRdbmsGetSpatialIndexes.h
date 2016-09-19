#ifndef _FDORDBMSGETSPATIALINDEXES_H_
#define _FDORDBMSGETSPATIALINDEXES_H_
//
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifdef _WIN32
#pragma once
#endif

#include "FdoRdbms.h"
#include "FdoRdbmsCommand.h"
#include "FdoRdbmsConnection.h"
#include "Rdbms/IGetSpatialIndexes.h"

/// <summary>Implementation class for IGetSpatialIndexes interface.</summary>
class FdoRdbmsGetSpatialIndexes : public FdoRdbmsCommand<FdoIGetSpatialIndexes>
{
    friend class FdoIConnection;

public:
    FdoRdbmsGetSpatialIndexes(void);

    FdoRdbmsGetSpatialIndexes(FdoIConnection *connection);

    ///<summary> Gets a Boolean flag that indicates if the GetSpatialIndexes command
    /// will return only the spatial indexes of the active spatial contexts or all spatial indexes.
    /// The default value of this flag is false, return all spatial indexes. </summary>
    /// <returns>Returns the value of the Boolean flag </returns>
    FDORDBMS_API virtual const bool GetActiveOnly();

    /// <summary> Gets a Boolean flag that indicates if the GetSpatialIndexes command
    /// will return only the spatial indexes of the active spatial contexts or all spatial indexes. </summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetActiveOnly(const bool value);

    /// <summary>Executes the GetSpatialIndexes command returning an ISpatialIndexReader.</summary>
    /// <returns>Returns FdoISpatialIndexReader</returns>
    FDORDBMS_API virtual FdoISpatialIndexReader* Execute();

protected:
    DbiConnection*  mConnection;

    virtual ~FdoRdbmsGetSpatialIndexes();

private:
    bool    mActiveOnly;
};
#endif

