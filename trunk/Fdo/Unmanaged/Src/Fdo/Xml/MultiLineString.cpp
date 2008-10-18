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

#include "MultiLineString.h"
#include <Geometry/Fgf/Factory.h>

#include <vector>
using namespace std;


FdoXmlMultiLineString* FdoXmlMultiLineString::Create()
{
	return new FdoXmlMultiLineString();

}

FdoIGeometry* FdoXmlMultiLineString::GetFdoGeometry()
{
	//create point collection
	FdoPtr<FdoLineStringCollection> lineStringCollection = FdoLineStringCollection::Create();
	
	for(vector<FdoXmlGeometry*>::iterator it = m_geometryCollection.begin();
		it != m_geometryCollection.end();
		it++)
	{
		FdoPtr<FdoILineString> fdoLineString = dynamic_cast<FdoILineString*>((*it)->GetFdoGeometry());
		if(fdoLineString != NULL)
			lineStringCollection->Add(fdoLineString);
	}

	//create FdoIMultiLineString
	FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
	

    FdoPtr<FdoIGeometry> rv;
    if (lineStringCollection->GetCount() != 0)
        rv = factory->CreateMultiLineString(lineStringCollection);
    return FDO_SAFE_ADDREF(rv.p);

}


