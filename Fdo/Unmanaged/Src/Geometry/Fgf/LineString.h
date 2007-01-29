#ifndef _FGFLINESTRING_H_
#define _FGFLINESTRING_H_
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

#include <Geometry/ILineString.h>
#include <Common/Array.h>
#include <Geometry/GeometryStream/GeometryStreamReader.h>
#include <Geometry/GeometryStream/GeometryStreamWriter.h>
#include <Geometry/EnvelopeImpl.h>
#include "GeometryImpl.h"


class FdoFgfLineString : public FdoFgfGeometryImpl<FdoILineString>
{
public:

	/*
     * Constructors matching public factory methods.
     */

	// Copy the positions.  Dimensionality is taken from the first position.
	FdoFgfLineString(FdoFgfGeometryFactory * factory, FdoDirectPositionCollection* positions);

	// Copy the ordinates.
	FdoFgfLineString(FdoFgfGeometryFactory * factory, FdoInt32 dimensionType, FdoInt32 numOrdinates, double* ordinates);

	// (internal use) Just latch onto the data stream.
	FdoFgfLineString(FdoFgfGeometryFactory * factory, FdoByteArray * byteArray, const FdoByte * data, FdoInt32 count);

    // Support object re-use, matching the constructors.
	void Reset(FdoDirectPositionCollection* positions);
	void Reset(FdoInt32 dimensionType, FdoInt32 numOrdinates, double* ordinates);
	void Reset(FdoByteArray * byteArray, const FdoByte * data, FdoInt32 count);

	// From FdoIGeometry
	virtual FdoIEnvelope* GetEnvelope() const;
	virtual FdoInt32 GetDimensionality() const;
	virtual FdoGeometryType GetDerivedType() const;

	// From FdoICurveAbstract
	virtual FdoIDirectPosition* GetStartPosition() const;
	virtual FdoIDirectPosition* GetEndPosition() const;
	virtual bool GetIsClosed() const;

	// From FdoILineString
	virtual FdoInt32 GetCount() const;
	virtual FdoIDirectPosition* GetItem(FdoInt32 index) const;
    virtual void GetItemByMembers(FdoInt32 index, double *x, double *y, double *z, double *m, FdoInt32 *dim) const;
	virtual FdoDirectPositionCollection* GetPositions();
    virtual const double * GetOrdinates();

    // From FdoFgfGeometryImpl
    virtual void SetFgf(FdoByteArray * fgf, const FdoByte * fgfData, FdoInt32 count);

protected:

	FdoFgfLineString() {}; // Default constructor to satisfy FdoPtr on Linux

	virtual ~FdoFgfLineString();

	mutable FdoInt32		 m_previousPositionIndex;

	FdoEnvelopeImpl* ComputeEnvelope() const;

	void Dispose();

};
#endif

