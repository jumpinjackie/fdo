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

#include "Runtime\Disposable.h"

class FdoXmlSaxContext;

BEGIN_NAMESPACE_OSGEO_COMMON_XML

ref class XmlReader;

/// \ingroup (OSGeoFDOCommonXml)
/// \brief
/// XmlSaxContext provides contextual information to the 
/// SaxHandler callback implementations when an XML document parse
/// is in progress. This class provides very rudimentary functionality such as
/// error reporting and access to the XmlReader doing the parse. Applications
/// can pass extra information by sub-classing from this class.
public ref class XmlSaxContext :  public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs the default Sax Context
    /// 
    /// \param reader 
    /// Input the XmlReader that is parsing the XML document.
    /// 
    /// \return
    /// Returns XmlSaxContext
    /// 
	XmlSaxContext(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ reader);

    /// \brief
    /// Gets the XmlReader that is parsing the XML document.
    /// 
    /// \return
    /// Returns XmlReader
    /// 
    property NAMESPACE_OSGEO_COMMON_XML::XmlReader^ Reader
    {
        NAMESPACE_OSGEO_COMMON_XML::XmlReader^ get();
    }

    /// \brief
    /// Constructs an XmlSaxContext object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	XmlSaxContext(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	inline FdoXmlSaxContext* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
/// \endcond

END_NAMESPACE_OSGEO_COMMON_XML


