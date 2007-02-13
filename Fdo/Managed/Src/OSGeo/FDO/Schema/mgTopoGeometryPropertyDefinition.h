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
*/

#pragma  once

#include "FDO\Schema\mgGeometricPropertyDefinition.h"

class FdoTopoGeometryPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \brief
/// TopoGeometryPropertyDefinition derives from
/// GeometricPropertyDefinition, but it has behavior of both geometry and
/// fixed association. It can be used to relate an application-domain feature
/// class (other than a topological primitive class) to a topology. One can use
/// the short form TopoGeometry to refer to these properties.  Values of this
/// property can be used:
/// <ul>
/// <li> as spatial geometries; use FdoIFeatureReader::GetGeometry().
/// <li> to gain access to relationships to underlying features, including
/// topology primitives, similar to Association Properties; e.g. use
/// IFeatureReader::GetObject() to retrieve, and associated feature
/// identifiers to insert/update.
/// </ul>
/// \note
/// This Help topic is provided for informational use only. There is
/// no interface or support provided. OSGeo reserves the right to change
/// the software related to the content herein.
public __gc class TopoGeometryPropertyDefinition : public NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition
{
public:
    /// \brief
    /// Constructs a TopoGeometryPropertyDefinition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	TopoGeometryPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoTopoGeometryPropertyDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


