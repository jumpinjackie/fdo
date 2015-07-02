#ifndef _FDORDBMSSPATIALINDEXREADER_H_
#define _FDORDBMSSPATIALINDEXREADER_H_
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
#include "Rdbms/ISpatialIndexReader.h"
#include "FdoRdbmsConnection.h"

#define DBI_PLAN_NAME_SIZE  256
#define DBI_INDEX_NAME_TYPE	3
#define DBI_INDEX_NAME_SIZE 32
/// <summary>Implementation class for ISpatialIndexReader</summary>

class FdoRdbmsSpatialIndexReader : public FdoISpatialIndexReader
{
public:
    FdoRdbmsSpatialIndexReader(FdoIConnection *connection, int qid, const wchar_t *scName);

    /// <summary>Gets the name of the spatial index currently being read.</summary>
    /// <returns>Returns the name of the spatial context.</returns>
    FDORDBMS_API virtual const wchar_t* GetName();

    /// <summary>Gets the name of the current spatial context as a String.</summary>
    /// <returns>Returns the description of the spatial context </returns>
    FDORDBMS_API virtual const wchar_t* GetSpatialContextName();

    /// <summary>Gets the type of the current spatial Index, either Quad-Tree or R-Tree.</summary>
    /// <returns>Returns the spatial index type</returns>
    FDORDBMS_API virtual SpatialIndexType GetSpatialIndexType();

    /// <summary>Gets the number of dimensions of the current spatial Index.</summary>
    /// <returns>Returns the extent type</returns>
    FDORDBMS_API virtual SpatialIndexDimensionType GetNumDimensions();

    /// <summary>Advances the reader to the next item. The default position of the reader
    /// is prior to the first item. Therefore you must call ReadNext to begin
    /// accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns>
    FDORDBMS_API virtual bool ReadNext();

    /// <summary>Closes the ISpatialIndexReader object freeing any resources it may
    /// be holding.</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void Dispose();

protected:

    virtual ~FdoRdbmsSpatialIndexReader();

private:
    bool ReadNextSI();

    DbiConnection*              mConnection;
    FdoRdbmsConnection*         mFdoConnection;
    int                         mQid;

    wchar_t                     mName[DBI_INDEX_NAME_SIZE+1];
    wchar_t                     mSCName[DBI_PLAN_NAME_SIZE+1];
    SpatialIndexType            mIndexType;
    SpatialIndexDimensionType   mDimType;

    bool                        mSpatialIndexFound;
    bool                        mFirstRead;
    long                        mSpatialContextId;
    int                         mGeomColIndex;
    int                         mEof;
//TODO    dbi_plan_geom_info_def      mDbiPlanGeom;
    char                        mDbName[GDBI_DB_NAME_SIZE];
};
#endif

