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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpQueryResult.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#ifndef FDORFPQUERYRESULT_H
#define FDORFPQUERYRESULT_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include "FdoRfpGeoRaster.h"
#include "FdoRfpRect.h"
#include <vector>

// FdoRfpQueryResult contains information about selected rows and requested columns
struct FdoRfpQueryResult : public FdoIDisposable
{
	// 1. information about rows

	// We can not combine FdoPtr with STL templates because FdoPtr overrides operator "&"
	// and we are not sure all the implementations of STL take it into account. 
	// so we have to create a class of collection's collection
	FdoPtr<FdoRfpGeoRasterCollectionCollection>     resultSet;
	// whether the result is aggregated
	bool											aggregated;

	// 2. information about requested columns

	// idenditiers for each column
	std::vector<std::vector<FdoStringP>* >			identifiers;
	// clipping bounds for each column
	std::vector<FdoRfpRect*>						clippingBounds;
	// property types for each column
	std::vector<PropertyType>						propertyTypes;
	// height/width in RESAMPLE function
	std::vector<FdoSize>							heightValues;
	std::vector<FdoSize>							widthValues;
public:
	FdoRfpQueryResult()	: aggregated(false)
	{
		resultSet = FdoRfpGeoRasterCollectionCollection::Create();
	}
protected:
	void Dispose() { delete this; }
	virtual ~FdoRfpQueryResult()
	{
		for (std::vector<std::vector<FdoStringP>* >::iterator it = identifiers.begin();
			it != identifiers.end(); it++)
			delete (*it);

		for (std::vector<FdoRfpRect*>::iterator it = clippingBounds.begin();
			it != clippingBounds.end(); it++)
			if ((*it) != NULL) delete (*it);
	}
};


#endif
