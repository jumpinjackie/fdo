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
#include "..\Io\mgIoTextWriter.h"

class FdoXslTransformer;

BEGIN_NAMESPACE_OSGEO_COMMON_XML
public __gc class XmlReader;
public __gc class XmlWriter;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_COMMON
public __gc class DictionaryElementCollection;
END_NAMESPACE_OSGEO_COMMON

BEGIN_NAMESPACE_OSGEO_COMMON_XSL

/// \brief
/// XslTransformer provides a way to execute an XSL stylesheet
/// on an XML document.
public __sealed __gc class XslTransformer : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs an XSL Transformer object.
    /// 
    /// \param inDoc 
    /// Input the XML document to transform.
    /// \param stylesheet 
    /// Input the XSL stylesheet with transformation instructions.
    /// \param outDoc 
    /// Output the transformed XML document.
    /// <p><b>Note:</b> The transformed document is not completely written until 'outDoc'
    /// and this transformer are destroyed by releasing all references to them.
    /// Therefore, these objects must be destroyed before reading back the 
    /// transformed document.
    /// \param log 
    /// Input the logging object.
    /// 
	XslTransformer();

	XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader* inDoc);

	XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader* inDoc, NAMESPACE_OSGEO_COMMON_XML::XmlReader* stylesheet);

	XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader* inDoc, NAMESPACE_OSGEO_COMMON_XML::XmlReader* stylesheet, NAMESPACE_OSGEO_COMMON_XML::XmlWriter* outDoc);

    XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader* inDoc, NAMESPACE_OSGEO_COMMON_XML::XmlReader* stylesheet, NAMESPACE_OSGEO_COMMON_XML::XmlWriter* outDoc, NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* log);

    /// \brief
    /// Gets the document being transformed
    /// 
    /// \return
    /// Returns the input document.
    /// 
	__property NAMESPACE_OSGEO_COMMON_XML::XmlReader* get_InDoc();

    /// \brief
    /// Sets the document to transform.
    /// 
    /// \param inDoc 
    /// Input the XML document to transform.
    /// 
	__property System::Void set_InDoc(NAMESPACE_OSGEO_COMMON_XML::XmlReader* value );

    /// \brief
    /// Gets the XSL Stylesheet
    /// 
    /// \return
    /// Returns the stylesheet.
    /// 
	__property NAMESPACE_OSGEO_COMMON_XML::XmlReader* get_Stylesheet();

    /// \brief
    /// Sets the XSL stylesheet
    /// 
    /// \param stylesheet 
    /// Input the stylesheet
    /// 
	__property System::Void set_Stylesheet(NAMESPACE_OSGEO_COMMON_XML::XmlReader* value);

    /// \brief
    /// Gets the output document.
    /// 
    /// \return
    /// Returns the out document
    /// 
	__property NAMESPACE_OSGEO_COMMON_XML::XmlWriter* get_OutDoc();

    /// \brief
    /// Sets the output document
    /// 
    /// \param outDoc 
    /// Input the output document
    /// <p><b>Note:</b> The output document is not completely written until outDoc
    /// and this transformer are destroyed by releasing all references to them.
    /// 
	__property System::Void set_OutDoc(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* value);

    /// \brief
    /// Gets the input logging object.
    /// 
    /// \return
    /// Returns the input logging object
    /// 
	__property NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* get_Log();

    /// \brief
    /// Sets the input logging object
    /// 
    /// \param outDoc 
    /// Input the logging object
    /// 
	__property System::Void set_Log(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* value);

    /// \brief
    /// Gets the list of parameters that will be passed to the 
    /// stylesheet by Transform(). The caller can add parameters to this list
    /// or modify/delete existing parameters. The parameter list is empty
    /// when this class is constructed.
    /// Parameter values must be in XPath syntax. Therefore, literal values
    /// must be enclosed in single quotes.
    /// 
    /// \return
    /// Returns the parameter list.
    /// 
	__property NAMESPACE_OSGEO_COMMON::DictionaryElementCollection* get_Parameters();

    /// \brief
    /// Performs the transformation.
    /// 
	System::Void Transform();

	XslTransformer(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoXslTransformer* GetImpObj();

protected:
	System::Void Dispose(System::Boolean disposing);
};

END_NAMESPACE_OSGEO_COMMON_XSL


