#ifndef _FGFMULTILINESTRING_H_
#define _FGFMULTILINESTRING_H_
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


#include <Geometry/IMultiLineString.h>
#include "GeometryImpl.h"


class FdoFgfMultiLineString : public FdoFgfGeometryImpl<FdoIMultiLineString>
{
public:

	/*
     * Constructors matching public factory methods.
     */

	FdoFgfMultiLineString(FdoFgfGeometryFactory * factory, FdoLineStringCollection* lineStrings);

    /*
     * Constructors supporting internal needs of this package.
     */

	FdoFgfMultiLineString(FdoFgfGeometryFactory * factory, FdoByteArray * byteArray, const FdoByte * data, FdoInt32 count);

    ~FdoFgfMultiLineString();

	// From FdoIGeometry
	virtual FdoIEnvelope* GetEnvelope() const;
	virtual FdoInt32 GetDimensionality() const;
	virtual FdoGeometryType GetDerivedType() const;

	// From FdoIGeometricAggregateAbstract
	virtual FdoInt32 GetCount() const;

	// From FdoIMultiLineString
	virtual FdoILineString* GetItem(FdoInt32 Index) const;


protected:

	void Dispose();

	FdoIEnvelope* ComputeEnvelope() const;
};
#endif

