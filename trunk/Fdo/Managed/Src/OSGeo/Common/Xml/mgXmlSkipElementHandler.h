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

class FdoXmlSkipElementHandler;

BEGIN_NAMESPACE_OSGEO_COMMON_XML

/// \ingroup (OSGeoFDOCommonXml)
/// \brief
/// XmlSkipHandler can be used to skip an XML element, when reading an 
/// XML document via XmlReader. It is a XmlSaxHandler with no callback 
/// implementations. Therefore, if another XmlSaxHandler's XmlStartElement
/// callback returns a XmlSkipElementHandler then there are no more SAX events
/// until the end of the current element is reached.
public __gc __sealed class XmlSkipElementHandler : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler
{
public:
    /// \brief
    /// Constructs a SAX Handler for skipping an element.
    /// 
    /// \return
    /// Returns XmlSkipElementHandler
    /// 
	XmlSkipElementHandler();

    /// \brief
    /// Constructs a XmlSkipElementHandler based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	XmlSkipElementHandler(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
public private:
	inline FdoXmlSkipElementHandler* GetImpObj();
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON_XML


