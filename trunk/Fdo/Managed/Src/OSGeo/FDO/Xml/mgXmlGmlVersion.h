/*
* Copyright (C) 2009-2010  Autodesk, Inc.
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

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_XML
/// \endcond

/// \ingroup (OSGeoFDOXml)
/// \brief
/// The GmlVersion enumeration defines the GML version.
/// 
public enum class GmlVersion
{
    /// Represents GML 2.1.2 version.
    GmlVersion_212 = FdoGmlVersion_212,

    /// Represents GML 3.1.1 version.
    GmlVersion_311 = FdoGmlVersion_311,
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_XML
/// \endcond
