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

#ifndef FDOWMSSPATIALCONTEXTREADER_H
#define FDOWMSSPATIALCONTEXTREADER_H

#include "FdoWmsCapabilities.h"

class FdoWmsSpatialContextReader : public FdoISpatialContextReader
{
public:
	FdoWmsSpatialContextReader();
	FdoWmsSpatialContextReader(FdoWmsCapabilities* capa);
    virtual ~FdoWmsSpatialContextReader(void);

    // methods inherited from FdoISpatialContextReader:
    virtual FdoString* GetName();
    virtual FdoString* GetDescription();
    virtual FdoString* GetCoordinateSystem();
    virtual FdoString* GetCoordinateSystemWkt();
    virtual FdoSpatialContextExtentType GetExtentType();
    virtual FdoByteArray* GetExtent();
    virtual const double GetXYTolerance();
    virtual const double GetZTolerance();
    virtual const bool IsActive();
    virtual bool ReadNext();
    virtual void Dispose();

private:
	// Identify the current spatial context.
	FdoInt32 mIndex;	

	// cache the WMS capabilities
	FdoWmsCapabilitiesP mWMSCapabilities;
};

#endif//FDOWMSSPATIALCONTEXTREADER_H
