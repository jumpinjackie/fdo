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

#include "FDO\Commands\SpatialContext\mgSpatialContextExtentType.h"

class FdoXmlSpatialContextWriter;

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlSpatialContextFlags;

/// \brief
/// FdoXmlSpatialContextWriter writes Spatial Contexts to an 
/// XML document. This is done by calling the various setter functions 
/// to set the spatial context attributes and then calling 
/// WriteSpatialContext() to write the spatial context using the current 
/// attributes. 
public __gc class XmlSpatialContextWriter : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// creates a Spatial Context Writer.
    /// 
    /// \param writer 
    /// Input XML document writer. Specifies the XML
    /// document that the Spatial Contexts will be written to.
    /// \param flags 
    /// Input options for controlling the 
    /// deserializing of the Spatial Contexts. If NULL then the 
    /// default flags are used.
    /// 
    /// \return
    /// Returns FdoXmlSpatialContextWriter
    /// 
	XmlSpatialContextWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* writer);

	XmlSpatialContextWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* writer, NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags* flags);

    /// \brief
    /// Gets the name of the context to create as a string.
    /// 
    /// \return
    /// Returns the name of the spatial context
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Sets the name of the context to create as a string.
    /// 
    /// \param value 
    /// Input the name of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Name(System::String* value);

    /// \brief
    /// Gets the description of the context to create as a string.
    /// 
    /// \return
    /// Returns the description of the spatial context
    /// 
	__property System::String* get_Description();

    /// \brief
    /// Sets the description of the context to create as a string.
    /// 
    /// \param value 
    /// Input the description of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Description(System::String* value);

    /// \brief
    /// Gets the name of the coordinate system of the context to create.
    /// 
    /// \return
    /// Returns the coordinate system name
    /// 
	__property System::String* get_CoordinateSystem();

    /// \brief
    /// Gets the name of the coordinate system of the context to create.
    /// 
    /// \param value 
    /// Input the coordinate system name
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_CoordinateSystem(System::String* value);

    /// \brief
    /// Gets the coordinate system of the context to create as a string
    /// in OpenGIS SRS WKT format.
    /// 
    /// \return
    /// Returns the coordinate system
    /// 
	__property System::String* get_CoordinateSystemWkt();

    /// \brief
    /// Sets the coordinate system of the context to create as a string in
    /// OpenGIS SRS WKT format.
    /// 
    /// \param value 
    /// Input the coordinate system
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_CoordinateSystemWkt(System::String* value);

    /// \brief
    /// Gets the desired extent type of the context to create, either static or dynamic.
    /// 
    /// \return
    /// Returns the extent type
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType get_ExtentType();

    /// \brief
    /// Sets the desired extent type of the context to create, either static or dynamic.
    /// 
    /// \param value 
    /// Input the extent type
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ExtentType(NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType value);

    /// \brief
    /// Gets the extent of the context to create as a byte array in FGF format.
    /// If the extent type is dynamic, any value specified via this function is ignored.
    /// 
    /// \return
    /// Returns the extent of the spatial context
    /// 
	__property System::Byte get_Extent() [];

    /// \brief
    /// Sets the extent of the context to create as a byte array in FGF format.
    /// If the extent type is dynamic this is optional and any value specified is ignored.
    /// 
    /// \param value 
    /// Input the extent of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Extent(System::Byte buffer []);

    /// \brief
    /// Gets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
	__property System::Double get_XYTolerance();

    /// \brief
    /// Sets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \param value 
    /// Input the tolerance
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_XYTolerance(System::Double value);

    /// \brief
    /// Gets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
	__property System::Double get_ZTolerance();

    /// \brief
    /// Sets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \param value 
    /// Input the tolerance
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ZTolerance(System::Double value);

    /// \brief
    /// Writes the current Spatial Context to the XML document.
    /// 
	System::Void WriteSpatialContext();

public private:
	XmlSpatialContextWriter(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoXmlSpatialContextWriter* GetImpObj();

protected:
	System::Void Dispose(System::Boolean disposing);
};

END_NAMESPACE_OSGEO_FDO_XML


