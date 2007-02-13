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

#include "FDO\Commands\SpatialContext\mgISpatialContextReader.h"
#include "FDO\Commands\SpatialContext\mgSpatialContextExtentType.h"

class FdoXmlSpatialContextReader;

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlSpatialContextFlags;

/// \brief
/// XmlSpatialContextReader reads Spatial Contexts from an XML 
/// document. Unlike other XML Readers, the reading is done procedurally, 
/// rather than through events. XmlSpatialContextReader implements 
/// ISpatialContextReader to provide the functions for retrieving the 
/// spatial contexts that were read.
public __gc class XmlSpatialContextReader : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler, public NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReader
{
public:
    /// \brief
    /// creates a Spatial Context Reader.
    /// 
    /// \param reader 
    /// Input XML document reader. Specifies the XML
    /// document that the Spatial Contexts will be read from.
    /// 
	XmlSpatialContextReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader* reader);

    /// \brief
    /// creates a Spatial Context Reader.
    /// 
    /// \param reader 
    /// Input XML document reader. Specifies the XML
    /// document that the Spatial Contexts will be read from.
    /// \param flags 
    /// Input options for controlling the 
    /// deserializing of the Spatial Contexts. If NULL then the 
    /// default flags are used.
    /// 
	XmlSpatialContextReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader* reader, NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags* flags);

    /// \brief
    /// Gets the Xml document reader that was passed to this object.
    /// 
    /// \return
    /// Returns the XML document reader.
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlReader* GetXmlReader();

    /// ISpatialContextReader implementation

    /// \brief
    /// Gets the name of the spatial context currently being read.
    /// 
    /// \return
    /// Returns the name of the spatial context.
    /// 
	System::String* GetName();

    /// \brief
    /// Gets the description of the spatial context currently being read.
    /// 
    /// \return
    /// Returns the description of the spatial context.
    /// 
	System::String* GetDescription();

    /// \brief
    /// Gets the name of the coordinate system of the spatial context currently
    /// being read. If the result is a null or empty string, the coordinate system is
    /// unknown to the spatial context.
    /// 
    /// \return
    /// Returns the coordinate system name of the spatial context.
    /// 
	System::String* GetCoordinateSystem();

    /// \brief
    /// Gets the coordinate system in OpenGIS SRS WKT format of the spatial context currently
    /// being read. If the result is an empty string, the coordinate system WKT description is
    /// not available.
    /// 
    /// \return
    /// Returns the coordinate system description in WKT of of the spatial context.
    /// 
	System::String* GetCoordinateSystemWkt();

    /// \brief
    /// Gets the extent type of the spatial context currently being read.
    /// 
    /// \return
    /// Returns the extent type.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType GetExtentType();

    /// \brief
    /// Gets the extent of the spatial context currently being read as a byte
    /// array in FGF format.
    /// 
    /// \return
    /// Returns the extent as a byte array in FGF format.
    /// 
	System::Byte GetExtent() [];

    /// \brief
    /// Gets the tolerance value for XY ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs frequently in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
	System::Double GetXYTolerance();

    /// \brief
    /// Gets the tolerance value for Z ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs a frequently in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
	System::Double GetZTolerance();

    /// \brief
    /// Returns true if the spatial context currently being read is the active
    /// spatial context.
    /// 
    /// \return
    /// Returns true if the current spatial context is the active one.
    /// 
	System::Boolean IsActive();

    /// \brief
    /// Advances the reader to the next item. The default position of the reader
    /// is prior to the first item. Thus, you must call ReadNext to begin
    /// accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
	System::Boolean ReadNext();

public private:
	XmlSpatialContextReader(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(unmanaged, autoDelete)
	{
		
	}

	inline FdoXmlSpatialContextReader* GetImpObj();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond
};

END_NAMESPACE_OSGEO_FDO_XML


