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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpSpatialContext.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#include "FDORFP.h"
#include "FdoRfpSpatialContext.h"
#include <Fdo/Xml/SpatialContextReader.h>

FdoRfpSpatialContext::FdoRfpSpatialContext(void)
{
}

FdoRfpSpatialContext::~FdoRfpSpatialContext(void)
{
}

void FdoRfpSpatialContextCollection::ReadXml(FdoXmlReader* reader)
{
	FdoXmlSpatialContextReaderP spreader = FdoXmlSpatialContextReader::Create(reader);

	// Read the one and ond supported spatial context
	while (spreader->ReadNext())
	{
		FdoPtr<FdoRfpSpatialContext> context = new FdoRfpSpatialContext();
		context->SetName(spreader->GetName());
		context->SetDescription(spreader->GetDescription());
		context->SetCoordinateSystem(spreader->GetCoordinateSystem());
		context->SetCoordinateSystemWkt(spreader->GetCoordinateSystemWkt());
		context->SetExtent(spreader->GetExtent());
		context->SetExtentType(spreader->GetExtentType());
		context->SetXYTolerance(spreader->GetXYTolerance());
		context->SetZTolerance(spreader->GetZTolerance());
		Add(context);
    }
}

FdoRfpSpatialContextCollection::~FdoRfpSpatialContextCollection()
{
	FdoInt32 count = GetCount();
	for (FdoInt32 i = count - 1; i >= 0; i--)
	{
		FdoRfpSpatialContext* sc = GetItem(0);
		RemoveAt(0);
		FDO_SAFE_RELEASE(sc);
	}
}
