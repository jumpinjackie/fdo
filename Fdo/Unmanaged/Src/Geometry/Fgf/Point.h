#ifndef _FGFPOINT_H_
#define _FGFPOINT_H_
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
*/

#ifdef _WIN32
#pragma once
#endif

#include <Geometry/IPoint.h>
#include "GeometryImpl.h"


class FdoFgfPoint : public FdoFgfGeometryImpl<FdoIPoint>
{
public:

	/*
     * Constructors matching public factory methods.
     */

	// Copy the ordinates.
	FdoFgfPoint(FdoFgfGeometryFactory * factory, FdoInt32 dimensionType, double* ordinates);

	//	Copy from a DirectPosition
	FdoFgfPoint(FdoFgfGeometryFactory * factory, FdoIDirectPosition* position);

    /*
     * Constructors supporting internal needs of this package.
     */

	// Just latch onto the data stream.
	FdoFgfPoint(FdoFgfGeometryFactory * factory, FdoByteArray * byteArray, const FdoByte * data, FdoInt32 count);

    // Support object re-use, matching the constructor.
	void Reset(FdoByteArray * byteArray, const FdoByte * data, FdoInt32 count);

    virtual ~FdoFgfPoint();


	// From FdoIGeometry
	virtual FdoIEnvelope* GetEnvelope() const;
	virtual FdoInt32 GetDimensionality() const;
	virtual FdoGeometryType GetDerivedType() const;


	// From FdoIPoint
	virtual FdoIDirectPosition* GetPosition() const;
    virtual void GetPositionByMembers(double *x, double *y, double *z, double *m, FdoInt32 *dimensionality) const;
    virtual const double * GetOrdinates();


protected:

	void Dispose();
};
#endif

