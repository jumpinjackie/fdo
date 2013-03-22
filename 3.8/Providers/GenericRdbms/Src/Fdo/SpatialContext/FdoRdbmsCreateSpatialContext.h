#ifndef _FDORDBMSCREATESPATIALCONTEXT_H_
#define _FDORDBMSCREATESPATIALCONTEXT_H_
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
#include "Fdo/Connections/IConnection.h"
#include "Fdo/Expression/GeometryValue.h"
#include "Fdo/Commands/SpatialContext/ICreateSpatialContext.h"

/// <summary>The ICreateSpatialContext interface defines the CreateSpatialContext
/// command, which creates a new spatial context. Input to the command includes
/// the name, description, coordinate system, extent type, and extent for the
/// new context. Command execution will fail if the context already exists, or
/// if the coordinate system or extent type is not supported by the provider.
/// Note that the geographic extent of a coordinate system should generally be
/// defined as an envelope (or bounding box). However any geometry type that
/// falls into the surface geometric types can be used (Polygons, etc.). If a
/// non-rectangular area is passed in and the provider only supports a
/// rectangular extent then the provider will compute the minimum box that
/// encloses the given geometry and use that for the extent value.</summary>
class FdoRdbmsCreateSpatialContext : public FdoRdbmsCommand<FdoICreateSpatialContext>
{
    friend class FdoRdbmsConnection;

    //    Constructs an instance of a FdoRdbmsCreateSpatialContext using the
    //    specified arguments.
    FdoRdbmsCreateSpatialContext (FdoIConnection *connection);

public:

    /// <summary>Gets the name of the context to create as a String.</summary>
    /// <returns>Returns the name of the spatial context</returns>
    FDORDBMS_API virtual FdoString* GetName();

    /// <summary>Sets the name of the context to create as a String.</summary>
    /// <param name="value">Input the name of the spatial context</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetName(FdoString* value);

    /// <summary>Gets the description of the context to create as a String.</summary>
    /// <returns>Returns the description of the spatial context</returns>
    FDORDBMS_API virtual FdoString* GetDescription();

    /// <summary>Sets the description of the context to create as a String.</summary>
    /// <param name="value">Input the description of the spatial context</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetDescription(FdoString* value);

    /// <summary>Gets the coordinate system of the context to create as a String
    /// in OpenGIS SRS WKT format.</summary>
    /// <returns>Returns the coordinate system</returns>
    FDORDBMS_API virtual FdoString* GetCoordinateSystem();

    /// <summary>Sets the coordinate system of the context to create as a String in
    /// OpenGIS SRS WKT format.</summary>
    /// <param name="value">Input the coordinate system</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetCoordinateSystem(FdoString* value);

    /// <summary>Gets the coordinate system definition in OGC SRS WKT format for the 
    /// spatial context currently being created. If the result is an empty string, 
    /// the coordinate system WKT description has not been set.</summary>
    /// <returns>Returns the coordinate system description in WKT of of the spatial context.</returns> 
    FDORDBMS_API virtual FdoString* GetCoordinateSystemWkt();

    /// <summary>Sets the coordinate system definition in OGC SRS WKT 
    /// format for the spatial context currently being created.</summary>
    /// <param name="value">Input the coordinate system WKT</param> 
    /// <returns>Returns nothing</returns> 
    FDORDBMS_API virtual void SetCoordinateSystemWkt(FdoString* value);

    /// <summary>Gets the desired extent type of the context to create, either static or dynamic.</summary>
    /// <returns>Returns the extent type</returns>
    FDORDBMS_API virtual FdoSpatialContextExtentType GetExtentType();

    /// <summary>Sets the desired extent type of the context to create, either static or dynamic.</summary>
    /// <param name="value">Input the extent type</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetExtentType(FdoSpatialContextExtentType value);

    /// <summary>Gets the extent of the context to create as a byte array in FGF format.
    /// If the extent type is dynamic this is optional and any value specified is ignored.</summary>
    /// <returns>Returns the extent of the spatial context</returns>
    FDORDBMS_API virtual FdoByteArray* GetExtent();

    /// <summary>Sets the extent of the context to create as a byte array in FGF format.
    /// If the extent type is dynamic this is optional and any value specified is ignored.</summary>
    /// <param name="value">Input the extent of the spatial context</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetExtent(FdoByteArray* value);

    /// <summary>Gets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs a lot in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    FDORDBMS_API virtual const double GetXYTolerance();

    /// <summary>Sets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs a lot in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.</summary>
    /// <param name="value">Input the tolerance</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetXYTolerance(const double value);

    /// <summary>Gets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs a lot in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    FDORDBMS_API virtual const double GetZTolerance();

    /// <summary>Sets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs a lot in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.</summary>
    /// <param name="value">Input the tolerance</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetZTolerance(const double value);

    /// <summary>Gets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists then command execution
    /// will fail.</summary>
    /// <returns>Returns true if command should update existing contexts</returns>
    FDORDBMS_API virtual const bool GetUpdateExisting();

    /// <summary>Sets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists then command execution
    /// will fail.</summary>
    /// <param name="value">Input true if existing contexts should be updated</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetUpdateExisting(const bool value);

    /// <summary>Executes the CreateSpatialContext command. An exception is thrown if
    /// the spatial context already exists, or if the coordinate system or
    /// extent type is not supported by the provider.</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void Execute();

protected:
	FdoRdbmsCreateSpatialContext() {};	
    virtual ~FdoRdbmsCreateSpatialContext();

private:

    FdoStringP                  mSCName;
    FdoStringP                  mDescription;
    FdoStringP                  mCoordSysName;
    FdoStringP                  mCoordSysWkt;

    FdoSpatialContextExtentType mExtentType;
    FdoPtr<FdoByteArray>        mExtent;

    double                      mXYTolerance;
    double                      mZTolerance;

    bool                        mUpdateExisting;
    long                        mSRID;
};
#endif
