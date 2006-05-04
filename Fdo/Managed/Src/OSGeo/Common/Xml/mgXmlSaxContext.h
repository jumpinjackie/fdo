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

public __gc class XmlReader;

/// \brief
/// XmlSaxContext provides contextual information to the 
/// SaxHandler callback implementations when an XML document parse
/// is in progress. This class provides very rudimentary functionality such as
/// error reporting and access to the XmlReader doing the parse. Applications
/// can pass extra information by sub-classing from this class.
public __gc class XmlSaxContext :  public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs the default Sax Context
    /// 
    /// \param reader 
    /// Input the FdoXmlReader that is parsing the XML document.
    /// 
    /// \return
    /// Returns XmlSaxContext
    /// 
	XmlSaxContext(NAMESPACE_OSGEO_COMMON_XML::XmlReader* reader);

    /// \brief
    /// Gets the FdoXmlReader that is parsing the XML document.
    /// 
    /// \return
    /// Returns XmlReader
    /// 
	__property NAMESPACE_OSGEO_COMMON_XML::XmlReader* get_Reader();

	XmlSaxContext(System::IntPtr unmanaged, System::Boolean autoDelete);

protected:
	System::Void Dispose(System::Boolean disposing);

public private:
	inline FdoXmlSaxContext* GetImpObj();
};

END_NAMESPACE_OSGEO_COMMON_XML


