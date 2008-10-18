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

#include "Common\Xml\mgXmlSaxHandler.h"

class FdoXmlCharDataHandler;

BEGIN_NAMESPACE_OSGEO_COMMON_XML

/// \ingroup (OSGeoFDOCommonXml)
/// \brief
/// XmlCharDataHandler is convenience class for reading XML element content.
/// When XmlReader encounters the start of an element with simple content, an object
/// of this class can be constructed and pushed onto the XmlReader's SAX handler 
/// stack. When the end of the element is reached, the object will contain all of
/// the element's content.
public __gc __sealed class XmlCharDataHandler : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler
{
public:
    /// \brief
    /// Constructs a Char Data Handler.
    /// 
	XmlCharDataHandler();

    /// \brief
    /// Gets the element content that has been read sofar. When the end of the
    /// element is reached, this will contain all of the element content.
    /// 
    /// \return
    /// Returns the element content.
    /// 
	__property System::String* get_RetString();
	
    /// \brief
    /// Constructs a DataHandler based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	XmlCharDataHandler(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
public private:
	inline FdoXmlCharDataHandler* GetImpObj();
/// \endcond
};
END_NAMESPACE_OSGEO_COMMON_XML


