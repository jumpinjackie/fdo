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

class FdoXmlFeatureContext;

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlFeaturePropertyReader;

/// \brief
/// FdoXmlFeatureContext provides contextual information to the FdoXmlFeatureHandler 
/// callback implementations when an XML document parse is in progress. This class 
/// provides very rudimentary functionality such as error reporting and access to the 
/// FdoXmlFeaturePropertyReader doing the parse. Applications can pass extra information
/// by sub-classing from this class.
public __gc class XmlFeatureContext : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext
{
public:
    /// \brief
    /// Constructs an XML  feature context
    /// 
    /// \param reader 
    /// Input the FdoXmlFeaturePropertyReader that is parsing the XML document.
    /// 
	XmlFeatureContext(NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader* value);

    /// \brief
    /// Gets the FdoXmlFeaturePropertyReader that is parsing the XML document.
    /// 
    /// \return
    /// Returns FdoXmlFeaturePropertyReader
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader* GetFeaturePropertyReader();

public private:
	XmlFeatureContext(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext(unmanaged, autoDelete)
	{
		
	}

	inline FdoXmlFeatureContext* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_XML


