#ifndef _FGFCIRCULARARCSEGMENT_H_
#define _FGFCIRCULARARCSEGMENT_H_
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

#include <Geometry/ICircularArcSegment.h>
#include "GeometryFactory2.h"


class FdoFgfCircularArcSegment : public FdoICircularArcSegment
{
public:

	/*
     * Constructors matching public factory methods.
     */

	FdoFgfCircularArcSegment(FdoFgfGeometryFactory * factory, FdoIDirectPosition* startPoint, FdoIDirectPosition* midPoint, FdoIDirectPosition* endPoint);

    virtual ~FdoFgfCircularArcSegment();

	//From FdoICurveSegmentAbstract
	virtual FdoIEnvelope* GetEnvelope() const;
	virtual FdoIDirectPosition* GetStartPosition() const;
	virtual FdoIDirectPosition* GetEndPosition() const;
	virtual bool GetIsClosed() const;
	virtual FdoGeometryComponentType GetDerivedType() const;
	virtual FdoInt32 GetDimensionality() const;

	// From FdoIArcSegmentAbstract
	virtual FdoIDirectPosition* GetMidPoint() const;

private:

	// LineString is used internally to store/manage
	// arc data. This LineString has 3 DirectPositions:
	// start, middle, end positions. FdoDimensionality
	// is stored usually as part of LineString.
	FdoPtr<FdoILineString>	m_lineString;

	void Dispose();

};
#endif

