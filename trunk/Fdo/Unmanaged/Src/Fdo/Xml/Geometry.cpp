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

#include "Geometry.h"

#include <Geometry/Fgf/Factory.h>
#include "GeometricProperty.h"

#include <iostream>
using namespace std;

FdoXmlGeometry* FdoXmlGeometry::Create(FdoByteArray* fgfByteArray)
{
	return new FdoXmlGeometry(fgfByteArray);
}

FdoXmlGeometry::FdoXmlGeometry()
{
	m_fgf = NULL;
}

FdoXmlGeometry::FdoXmlGeometry(FdoByteArray* fgfByteArray)
{
	m_fgf = FDO_SAFE_ADDREF(fgfByteArray);
}


FdoXmlGeometry::~FdoXmlGeometry()
{
	while(!m_geometricProperties.empty()){
		FDO_SAFE_RELEASE(m_geometricProperties.back());
		m_geometricProperties.pop_back();
	}

	while(!m_geometryCollection.empty()){
		FDO_SAFE_RELEASE(m_geometryCollection.back());
		m_geometryCollection.pop_back();
	}

}

FdoByteArray* FdoXmlGeometry::GetFgf()
{
	if(m_fgf == NULL){

		FdoPtr<FdoIGeometry> geometry = GetFdoGeometry();
		
		if(geometry != NULL){
			FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
			m_fgf = factory->GetFgf(geometry);
		}
	}

	return FDO_SAFE_ADDREF(m_fgf.p);
}


FdoIGeometry* FdoXmlGeometry::GetFdoGeometry()
{
	return NULL;
}

void FdoXmlGeometry::AddGeometricProperty(FdoString* name, FdoXmlGeometry* geometry)
{
	FdoPtr<FdoXmlGeometricProperty> geoProperty = new FdoXmlGeometricProperty(name, geometry);

	m_geometricProperties.push_back(FDO_SAFE_ADDREF(geoProperty.p));
}

void FdoXmlGeometry::AddGeometryMember(FdoXmlGeometry* geometry)
{
	m_geometryCollection.push_back(FDO_SAFE_ADDREF(geometry));
}


void FdoXmlGeometry::SetCoordinates(FdoXmlCoordinateGroup* coordinates)
{
	//m_coordinates.insert(m_coordinates.end(), coordinates.begin(), coordinates.end());
	m_coordinates = FDO_SAFE_ADDREF(coordinates);
}

FdoXmlGeometry* FdoXmlGeometry::GetGeometricProperty(FdoString* name)
{
	for(vector<FdoXmlGeometricProperty*>::iterator it = m_geometricProperties.begin(); it != m_geometricProperties.end(); it++){
		FdoXmlGeometricProperty* p = *it;
		if(wcscmp( name, p->GetName()) == 0)
			return p->GetGeometry();
	}
	return NULL;

}


