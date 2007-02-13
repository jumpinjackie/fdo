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

class FdoXmlNameCollectionHandler;

BEGIN_NAMESPACE_OSGEO_FDO_XML

/// \brief
/// XmlNameCollectionHandler is a class for serializing and 
/// deserializing fdo:NameCollectionType elements.
public __gc class XmlNameCollectionHandler : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler
{
public:
    /// \brief
    /// Constructs an Fdo XML Name Collection Handler.
    /// 
	XmlNameCollectionHandler();

    /// \brief
    /// Constructs an Fdo XML Name Collection Handler.
    /// 
    /// \param names 
    /// Input the collection of names. Any names deserialized
    /// will be added to this collection. If NULL then an internal empty
    /// name collection is created.
    /// 
	XmlNameCollectionHandler(NAMESPACE_OSGEO_COMMON::StringCollection* names);

    /// \brief
    /// Gets the collection of names
    /// 
    /// \return
    /// Returns the element content.
    /// 
	NAMESPACE_OSGEO_COMMON::StringCollection* GetNames();

public private:
	XmlNameCollectionHandler(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(unmanaged, autoDelete)
	{
		
	}

	inline FdoXmlNameCollectionHandler* GetImpObj();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond
};

END_NAMESPACE_OSGEO_FDO_XML


