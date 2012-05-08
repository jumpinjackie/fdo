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

#include "MultiPoint.h"
#include <Geometry/Fgf/Factory.h>

#include <vector>
using namespace std;

FdoXmlMultiPoint* FdoXmlMultiPoint::Create()
{
	return new FdoXmlMultiPoint();
}

FdoIGeometry* FdoXmlMultiPoint::GetFdoGeometry()
{
	//create point collection
	FdoPtr<FdoPointCollection> pointCollection = FdoPointCollection::Create();
	
	for(vector<FdoXmlGeometry*>::iterator it = m_geometryCollection.begin();
		it != m_geometryCollection.end();
		it++)
	{
		FdoPtr<FdoIPoint> fdoPoint = dynamic_cast<FdoIPoint*>((*it)->GetFdoGeometry());
		
		if(fdoPoint != NULL)
			pointCollection->Add(fdoPoint);
	}

	//convert to FdoIMultiPoint
	FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
	
    FdoPtr<FdoIGeometry> rv;
    if (pointCollection->GetCount() != 0)
        rv = factory->CreateMultiPoint(pointCollection);
    return FDO_SAFE_ADDREF(rv.p);

}


