// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "LinearRing.h"
#include <Geometry/Fgf/Factory.h>

FdoXmlLinearRing* FdoXmlLinearRing::Create()
{
	return new FdoXmlLinearRing();
}


FdoIGeometry* FdoXmlLinearRing::GetFdoGeometry()
{
	//use Line String to simulate Linear Ring
	//this is necessary because FdoILinearRing is not FdoIGeometry

	FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoILineString> lineString = factory->CreateLineString(m_coordinates->GetDimensionality(), 
																m_coordinates->GetSize(), 
																m_coordinates->GetDoubleArray()
																);
	return FDO_SAFE_ADDREF(lineString.p);
}

FdoILinearRing* FdoXmlLinearRing::GetFdoLinearRing()
{

	FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoILinearRing> ring = factory->CreateLinearRing(m_coordinates->GetDimensionality(), 
																m_coordinates->GetSize(), 
																m_coordinates->GetDoubleArray()
																);

	return FDO_SAFE_ADDREF(ring.p);
}


