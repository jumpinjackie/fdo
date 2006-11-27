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

#include "Polygon.h"

#include <Geometry/Fgf/Factory.h>
#include "LinearRing.h"

FdoXmlPolygon* FdoXmlPolygon::Create()
{
	return new FdoXmlPolygon();
}

FdoIGeometry* FdoXmlPolygon::GetFdoGeometry()
{
	//at least have one outer boundary
	if(m_geometryCollection.size() > 0){
		//get outer bound
		FdoXmlLinearRing* outerBound = dynamic_cast<FdoXmlLinearRing*>(m_geometryCollection.at(0));
		FdoPtr<FdoILinearRing> outerBoundRing = outerBound->GetFdoLinearRing();


		//get inner bound collection
		FdoPtr<FdoLinearRingCollection> innerBounds = FdoLinearRingCollection::Create();
		for(int i = 1; i < (int)m_geometryCollection.size(); i++){
			FdoXmlLinearRing* innerBound = dynamic_cast<FdoXmlLinearRing*>(m_geometryCollection.at(i));
			innerBounds->Add(innerBound->GetFdoLinearRing());
		}

		//create polygon
		FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
		return factory->CreatePolygon(outerBoundRing, innerBounds);
	}

	return NULL;
}


