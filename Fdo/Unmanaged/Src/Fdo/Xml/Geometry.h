#ifndef FDO_XML_GEOMETRY_H_
#define FDO_XML_GEOMETRY_H_
//

//
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
//

#ifdef _WIN32
#pragma once
#endif
#include <Std.h>
#include "CoordinateGroup.h"

#include <vector>

class FdoXmlGeometricProperty;
class FdoIGeometry;

//Geometry Base class
class FdoXmlGeometry : public FdoDisposable {

public:

	static FdoXmlGeometry* Create(FdoByteArray* fgfByteArray);

	virtual FdoByteArray* GetFgf();
	virtual FdoIGeometry* GetFdoGeometry();

	virtual void SetCoordinates(FdoXmlCoordinateGroup* coordinates);

	virtual void AddGeometricProperty(FdoString* name, FdoXmlGeometry* geometry);
	virtual void AddGeometryMember(FdoXmlGeometry* geometry);

	FdoXmlGeometry* GetGeometricProperty(FdoString* name);

protected:
	FdoXmlGeometry();
	FdoXmlGeometry(FdoByteArray* fgfByteArray);
	virtual ~FdoXmlGeometry();

    virtual void Dispose ()
    {
        delete this;
    }
	

	//std::vector<double> m_coordinates;
	FdoPtr<FdoXmlCoordinateGroup> m_coordinates;

	std::vector<FdoXmlGeometricProperty*> m_geometricProperties;
	std::vector<FdoXmlGeometry*> m_geometryCollection;

	FdoPtr<FdoByteArray> m_fgf;
private:
};

typedef FdoPtr<FdoXmlGeometry> FdoXmlGeometryP;

#endif

