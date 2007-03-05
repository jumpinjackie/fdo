//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_SPATIALCONTEXTREADER_H_INCLUDED
#define FDOPOSTGIS_SPATIALCONTEXTREADER_H_INCLUDED

#include <Fdo/Commands/SpatialContext/SpatialContextExtentType.h>
#include <Fdo/Commands/SpatialContext/ISpatialContextReader.h>
#include "SpatialContext.h"
#include "SpatialContextCollection.h"


namespace fdo { namespace postgis {

/// Implementation of forward-only, read-only iterator for spatial contexts.
///
class SpatialContextReader : public FdoISpatialContextReader
{
public:

    /// Default constructor.
    SpatialContextReader(SpatialContextCollection*  scc);

    //
    // FdoISpatialContextReader interface
    //
    
    /// Get name of the spatial context currently being read.
    virtual FdoString* GetName();
    
    /// Get description of the spatial context currently being read.
    virtual FdoString* GetDescription();
    
    /// Get name of coordinate system of spatial context currently being read.
    virtual FdoString* GetCoordinateSystem();
    
    /// Get name of coordinate system in OpenGIS SRS WKT format of spatial context
    /// currently being read.
    virtual FdoString* GetCoordinateSystemWkt();
    
    /// Get extent type of spatial context currently being read.
    virtual FdoSpatialContextExtentType GetExtentType();
    
    /// Get extent of spatial context currently being read as a byte array in FGF format.
    virtual FdoByteArray* GetExtent();
    
    /// Get tolerance value for XY ordinates of spatial context currently being read.
    virtual const double GetXYTolerance();
    
    /// Get tolerance value for Z ordinates of spatial context currently being read.
    virtual const double GetZTolerance();
    
    /// Check if spatial context currently being read is an active spatial context.
    virtual const bool IsActive();
    
    /// Moves contexts iterator of the reader to next item.
    virtual bool ReadNext();

protected:

    /// Destructor.
    virtual ~SpatialContextReader();

    //
    // FdoDisposable interface
    //

    /// Close the reader instance freeing any resources it may be holding.
    void Dispose();

private:
	
    SpatialContextCollection::Ptr mSpatialContexts;
    SpatialContext::Ptr mCurrentContext;
    FdoInt32 mCurrentIndex;
    
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_SPATIALCONTEXTREADER_H_INCLUDED
