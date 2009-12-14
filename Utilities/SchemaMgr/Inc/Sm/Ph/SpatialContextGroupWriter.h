#ifndef FDOSMPHSPATIALCONTEXTGROUPWRITER_H
#define FDOSMPHSPATIALCONTEXTGROUPWRITER_H		1
/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/Writer.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Row.h>

// This class writes spatial context group definitions 
// to the f_spatialcontextgroup table.
class FdoSmPhSpatialContextGroupWriter : public FdoSmPhWriter
{
public:

    /// Create a spatial context group writer.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the datastore to use.
	FdoSmPhSpatialContextGroupWriter(FdoSmPhMgrP mgr);

	~FdoSmPhSpatialContextGroupWriter(void);
    
    /// Various field value getters

    /// Spatial Context Group ID - internal to Provider
	FdoInt64 GetId();

    /// Coordinate Reference System name
	FdoStringP GetCrsName();

    /// Coordinate Reference System definition in OpenGIS WKT format
	FdoStringP GetCrsWkt();

    /// Spatial Reference ID (if not zero, must agree with CrsName).
	FdoInt64 GetSrid();

    /// Tolerances and extents
    double GetXTolerance();
    double GetZTolerance();
    double GetXMin();
    double GetYMin();
    double GetZMin();
    double GetXMax();
    double GetYMax();
    double GetZMax();

    /// Extent Type -- immutable 'S' (static) or updatable 'D' (dynamic)
    FdoStringP GetExtentType();

    /// Corresponding setter functions. There's none for scgid 
    /// since it is autogenerated when a new spatial context group is written.

    /// Coordinate Reference System name
	void SetCrsName(FdoStringP sValue);

    /// Coordinate Reference System definition in OpenGIS WKT format
	void SetCrsWkt(FdoStringP sValue);

    /// Spatial Reference ID (if not zero, must agree with CrsName).
	void SetSrid(FdoInt64 lValue);

    /// Tolerances and extents
    void SetXTolerance(double dValue);
    void SetZTolerance(double dValue);
    void SetXMin(double dValue);
    void SetYMin(double dValue);
    void SetZMin(double dValue);
    void SetXMax(double dValue);
    void SetYMax(double dValue);
    void SetZMax(double dValue);

    /// Extent Type -- immutable 'S' (static) or updatable 'D' (dynamic)
    void SetExtentType(FdoStringP sValue);

    /// Add the current row as a new spatial context group
	virtual void Add();

    /// Modify a spatial context group using the current field values.
    //
    /// Parameters
    ///      scId: primary key of the spatial context group to modify
    virtual void Modify( FdoInt64 scgId );

    /// Delete a spatial context group using the current field values.
    //
    /// Parameters
    ///      scgId: primary key of the spatial context group to delete
    virtual void Delete( FdoInt64 scgId );

    /// Get a field list for reading from or writing to 
    /// f_spatialcontextgroup.
	//
    /// Parameters:
    /// 	physicalSchema: holds the connection for the datastore to query.
    virtual FdoSmPhRowP MakeRow( FdoSmPhMgrP mgr );

    // Various functions that specific column names for f_spatialcontextgroup
    // attributes. Providers that need to can override these.
    virtual FdoStringP ColNameXMin();
    virtual FdoStringP ColNameYMin();
    virtual FdoStringP ColNameZMin();
    virtual FdoStringP ColNameXMax();
    virtual FdoStringP ColNameYMax();
    virtual FdoStringP ColNameZMax();

    // Initialize this object. Set up its sub-writer.
    virtual void Initialize();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSpatialContextGroupWriter() {}


    FdoSmPhWriterP MakeWriter( FdoSmPhMgrP mgr );
    
    void SetId( FdoInt64 lValue);

private:
    FdoSmPhMgr* mMgr;
};

typedef FdoPtr<FdoSmPhSpatialContextGroupWriter> FdoSmPhSpatialContextGroupWriterP;

#endif


