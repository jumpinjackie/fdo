#ifndef _FGFLINEARRING_H_
#define _FGFLINEARRING_H_
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

#include <Geometry/ILinearRing.h>
#include <Geometry/ILineString.h>

class FdoFgfGeometryFactory;   // Forward declaration


class FdoFgfLinearRing : public FdoILinearRing
{
public:

	/*
     * Constructors matching public factory methods.
     */

	FdoFgfLinearRing(FdoFgfGeometryFactory * factory, FdoInt32 dimensionType, FdoInt32 numOrdinates, double* ordinates);
	FdoFgfLinearRing(FdoFgfGeometryFactory * factory, FdoDirectPositionCollection* positions);

	virtual ~FdoFgfLinearRing();

    // Support object re-use, matching the constructors.
	virtual void Reset(FdoDirectPositionCollection* positions);
	virtual void Reset(FdoInt32 dimensionType, FdoInt32 numOrdinates, double* ordinates);

	// From FdoIRingAbstract
	virtual FdoIEnvelope* GetEnvelope() const;
	virtual FdoInt32 GetDimensionality() const;

	// From FdoILinearRing
	virtual FdoInt32 GetCount() const;
	virtual FdoIDirectPosition* GetItem(FdoInt32 index) const;
    virtual void GetItemByMembers(FdoInt32 index, double *x, double *y, double *z, double *m, FdoInt32 *dimensionality) const;
	virtual FdoDirectPositionCollection* GetPositions();
    virtual const double * GetOrdinates();


protected:

	FdoFgfLinearRing() {}; // Default constructor to satisfy FdoPtr on Linux

	FdoPtr<FdoILineString>	m_lineString;

	bool IsClosed(FdoInt32 dimensionality, FdoInt32 numOrdinates, double* ordinates) const;

	void Dispose();


};
#endif

