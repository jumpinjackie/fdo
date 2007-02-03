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

namespace fdo { namespace postgis {


/// Implementation of forward-only, read-only iterator for spatial contexts.
///
class SpatialContextReader : public FdoISpatialContextReader
{
public:

    /// Default constructor.
    SpatialContextReader();

protected:

    /// Destructor.
    virtual ~SpatialContextReader();

    //
    // FdoDisposable interface
    //

    void Dispose();

private:
	
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_SPATIALCONTEXTREADER_H_INCLUDED
