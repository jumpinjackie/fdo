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

#include "GeometricProperty.h"


using namespace std;


FdoXmlGeometricProperty::FdoXmlGeometricProperty(FdoString* name, FdoXmlGeometry* geometry)
{
	m_name = name;
	m_geometry = FDO_SAFE_ADDREF(geometry);
}

FdoXmlGeometricProperty::FdoXmlGeometricProperty(FdoString* name, FdoByteArray* fgfByteArray)
{
	m_name = name;
	m_geometry = FdoXmlGeometry::Create(fgfByteArray);
}

FdoXmlGeometricProperty::~FdoXmlGeometricProperty()
{
}

FdoString* FdoXmlGeometricProperty::GetName() const
{
	return m_name;
}

FdoXmlGeometry* FdoXmlGeometricProperty::GetGeometry() const
{
	return FDO_SAFE_ADDREF((FdoXmlGeometry*)m_geometry);
}

FdoByteArray* FdoXmlGeometricProperty::GetFgf() const
{
	return m_geometry->GetFgf();
}

