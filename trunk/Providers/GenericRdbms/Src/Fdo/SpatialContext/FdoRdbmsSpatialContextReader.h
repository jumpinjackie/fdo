#ifndef _FDORDBMSSPATIALCONTEXTREADER_H_
#define _FDORDBMSSPATIALCONTEXTREADER_H_
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
#include "FdoRdbmsConnection.h"
#include "Fdo/Commands/SpatialContext/ISpatialContextReader.h"
//#include "inc/dbi/plan_info.h"

#define DBI_PLAN_NAME_SIZE      256
#define DBI_PLAN_DESC_SIZE      80
#define CSM_NAME_LEN				31

/// <summary>The ISpatialContextReader interface provides forward only, readonly
/// functionality for enumerating spatial contexts.  A reference to an
/// ISpatialContextReader is returned from the GetSpatialContexts command.
/// The initial position of the ISpatialContextReader is prior to the
/// first item. Therefore you must call ReadNext to begin accessing any data.</summary>
class FdoRdbmsSpatialContextReader : public FdoISpatialContextReader
{

public:

    //    Constructs an instance of a FdoRdbmsSpatialContextReader using the
    //    specified arguments.
    FdoRdbmsSpatialContextReader ( FdoRdbmsConnection * connection,
                                   bool act_sc_only );

public:

    /// <summary>Gets the name of the spatial context currently being read.</summary>
    /// <returns>Returns the name of the spatial context.</returns>
    FDORDBMS_API virtual FdoString* GetName();

    /// <summary>Gets the description of the spatial context currently being read.</summary>
    /// <returns>Returns the description of the spatial context.</returns>
    FDORDBMS_API virtual FdoString* GetDescription();

    /// <summary>Gets the name of the coordinate system of the spatial context currently
    /// being read. If null or empty string then the coordinate system is
    /// unknown to the spatial context.</summary>
    /// <returns>Returns the coordinate system name of the spatial context.</returns>
    FDORDBMS_API virtual FdoString* GetCoordinateSystem();

    /// <summary>Gets the name of the coordinate system in OpenGIS SRS WKT format of the spatial context currently
    /// being read. If the result is an empty string, the coordinate system WKT description is
    /// not available.</summary>
    /// <returns>Returns the coordinate system description in WKT of of the spatial context.</returns>
    FDORDBMS_API virtual FdoString* GetCoordinateSystemWkt();

    /// <summary>Gets the extent type of the spatial context currently being read.</summary>
    /// <returns>Returns the extent type.</returns>
    FDORDBMS_API virtual FdoSpatialContextExtentType GetExtentType();

    /// <summary>Gets the extent of the spatial context currently being read as a byte
    /// array in FGF format.</summary>
    /// <param name="length">Output the length of the returned byte array.</param>
    /// <returns>Returns the extent as a byte array in FGF format.</returns>
    FDORDBMS_API virtual FdoByteArray* GetExtent();

    /// <summary>Gets the tolerance value for XY ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs a lot in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    FDORDBMS_API virtual const double GetXYTolerance();

    /// <summary>Gets the tolerance value for Z ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs a lot in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    FDORDBMS_API virtual const double GetZTolerance();

    /// <summary>Returns true if the spatial context currently being read is the active
    /// spatial context.</summary>
    /// <returns>Returns true if the current spatial context is the active one.</returns>
    FDORDBMS_API virtual const bool IsActive();

    /// <summary>Advances the reader to the next item. The default position of the reader
    /// is prior to the first item. Therefore you must call ReadNext to begin
    /// accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns>
    FDORDBMS_API virtual bool ReadNext();

    /// <summary>Closes the ISpatialContextReader object freeing any resources it may
    /// be holding.</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void Dispose();

protected:
    virtual ~FdoRdbmsSpatialContextReader();

private:

    FdoRdbmsConnection* mFdoConnection; // Lightweight reference

    FdoInt32            mScIndex;   // Index into schema manager's spatial contexts
    FdoSmLpSpatialContextP mSc;     // Spatial Context at mScIndex

    bool                mActiveOnly;
    bool                mFirstRead;
    bool                mIsActiveSC;
    bool                mAreMoreSC;
};
#endif
