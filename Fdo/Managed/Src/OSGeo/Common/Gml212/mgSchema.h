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

#pragma once

BEGIN_NAMESPACE_OSGEO_COMMON_XML
ref class XmlReader;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_COMMON_GML212

/// \ingroup (OSGeoFDOCommonGML212)
/// \brief
/// Schema is a utility class for handling the GML 2.1.2 
/// version of the GML Sschema.
public ref class Schema sealed
{
public:
    /// \brief
    /// Constructs an GML 2.1.2 schema reader. This reader reads
    /// an XML document containing the GML 2.1.2 and Xlinks schemas.
    /// 
    /// \return
    /// Returns a new XmlReader
    /// 
	static NAMESPACE_OSGEO_COMMON_XML::XmlReader^ CreateReader();

/// \cond DOXYGEN-IGNORE
private:
	Schema(){}
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON_GML212


