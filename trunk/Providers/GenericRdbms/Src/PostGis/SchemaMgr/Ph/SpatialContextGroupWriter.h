#ifndef FDOSMPHPOSTGISSPATIALCONTEXTGROUPWRITER_H
#define FDOSMPHPOSTGISSPATIALCONTEXTGROUPWRITER_H		1
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

#include "../../../SchemaMgr/Ph/SpatialContextGroupWriter.h"
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Row.h>

// This class writes spatial context group definitions 
// to the f_spatialcontextgroup table.
class FdoSmPhPostGisSpatialContextGroupWriter : public FdoSmPhGrdSpatialContextGroupWriter
{
public:

    /// Create a spatial context group writer.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the datastore to use.
	FdoSmPhPostGisSpatialContextGroupWriter(FdoSmPhMgrP mgr);

	~FdoSmPhPostGisSpatialContextGroupWriter(void);
    
    virtual FdoStringP ColNameXMin();
    virtual FdoStringP ColNameYMin();
    virtual FdoStringP ColNameZMin();
    virtual FdoStringP ColNameXMax();
    virtual FdoStringP ColNameYMax();
    virtual FdoStringP ColNameZMax();


protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhPostGisSpatialContextGroupWriter() {}
};

typedef FdoPtr<FdoSmPhPostGisSpatialContextGroupWriter> FdoSmPhPostGisSpatialContextGroupWriterP;

#endif


