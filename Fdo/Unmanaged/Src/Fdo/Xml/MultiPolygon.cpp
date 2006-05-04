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

#include "MultiPolygon.h"
#include <Geometry/Fgf/Factory.h>

#include <vector>
using namespace std;

FdoXmlMultiPolygon* FdoXmlMultiPolygon::Create()
{
	return new FdoXmlMultiPolygon();
}

FdoIGeometry* FdoXmlMultiPolygon::GetFdoGeometry()
{
	//create polygon collection
 	FdoPtr<FdoPolygonCollection> polygonCollection = FdoPolygonCollection::Create();
	
	for(vector<FdoXmlGeometry*>::iterator it = m_geometryCollection.begin();
		it != m_geometryCollection.end();
		it++)
	{
		FdoPtr<FdoIPolygon> fdoPolygon = dynamic_cast<FdoIPolygon*>((*it)->GetFdoGeometry());
		if(fdoPolygon != NULL)
			polygonCollection->Add(fdoPolygon);
	}

	//convert to FdoIMultiPoint
	FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoIGeometry> rv;
    if (polygonCollection->GetCount() != 0)
	    rv = factory->CreateMultiPolygon(polygonCollection);
    return FDO_SAFE_ADDREF(rv.p);
}


