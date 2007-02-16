#ifndef _FGFMULTIPOINT_H_
#define _FGFMULTIPOINT_H_
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

#include <Geometry/IMultiPoint.h>
#include "GeometryImpl.h"


class FdoFgfMultiPoint : public FdoFgfGeometryImpl<FdoIMultiPoint>
{
public:

	/*
     * Constructors matching public factory methods.
     */

	FdoFgfMultiPoint(FdoFgfGeometryFactory * factory, FdoFgfGeometryPools * pools, FdoInt32 dimensionality, FdoInt32 numOrdinates, double* ordinates);
	FdoFgfMultiPoint(FdoFgfGeometryFactory * factory, FdoFgfGeometryPools * pools, FdoPointCollection* points);

    /*
     * Constructors supporting internal needs of this package.
     */

	FdoFgfMultiPoint(FdoFgfGeometryFactory * factory, FdoFgfGeometryPools * pools, FdoByteArray * byteArray, const FdoByte * data, FdoInt32 count);

    // Support object re-use, matching the constructor.
	void Reset(FdoByteArray * byteArray, const FdoByte * data, FdoInt32 count);

	~FdoFgfMultiPoint();

	// From FdoIGeometry
	virtual FdoIEnvelope* GetEnvelope() const;
	virtual FdoInt32 GetDimensionality() const;
	virtual FdoGeometryType GetDerivedType() const;

	// From FdoIGeometricAggregateAbstract
	virtual FdoInt32 GetCount() const;

	// From FdoIMultiPoint
	virtual FdoIPoint* GetItem(FdoInt32 Index) const;
    virtual const double * GetOrdinates();


protected:

	void Dispose();

    // The following is just to retain ownership of the ordinate array in
    // case GetOrdinates() is called.  Unlike most other types descended
    // from FdoIGeometry with ordinate-array factories, this type does not
    // actually store ordinates an array of undiluted ordinates.  Being an
    // aggregate type, it interleaves sub-geometries with typing information.
    // m_ordinates stores a homogenous array of ordinates if GetOrdinates()
    // is called.
    double *    m_ordinates;

    FdoIEnvelope* ComputeEnvelope() const;
};
#endif

