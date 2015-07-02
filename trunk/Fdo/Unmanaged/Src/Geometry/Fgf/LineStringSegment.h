#ifndef _FGFLINESTRINGSEGMENT_H_
#define _FGFLINESTRINGSEGMENT_H_
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

#include <Geometry/ILineStringSegment.h>
#include <Geometry/DirectPositionImpl.h>
#include <Geometry/EnvelopeImpl.h>
#include <Geometry/ILineString.h>

class FdoFgfGeometryFactory;   // Forward declaration


class FdoFgfLineStringSegment : public FdoILineStringSegment
{
public:

	/*
     * Constructors matching public factory methods.
     */

	FdoFgfLineStringSegment(FdoFgfGeometryFactory * factory, FdoDirectPositionCollection * positions);
	FdoFgfLineStringSegment(FdoFgfGeometryFactory * factory, FdoInt32 dimensionType, FdoInt32 numOrdinates, double* ordinates);

	virtual ~FdoFgfLineStringSegment();

	//From FdoICurveSegmentAbstract
	virtual FdoIEnvelope* GetEnvelope() const;
	virtual FdoIDirectPosition* GetStartPosition() const;
	virtual FdoIDirectPosition* GetEndPosition() const;
	virtual bool GetIsClosed() const;
	virtual FdoGeometryComponentType GetDerivedType() const;
	virtual FdoInt32 GetDimensionality() const;

	//From FdoILineStringSegment
	virtual FdoInt32 GetCount() const;
	virtual FdoIDirectPosition* GetItem(FdoInt32 index) const;
	virtual FdoDirectPositionCollection* GetPositions();
    virtual const double * GetOrdinates();

protected:


	FdoPtr<FdoILineString>	m_lineString;

	void Dispose();



};
#endif

