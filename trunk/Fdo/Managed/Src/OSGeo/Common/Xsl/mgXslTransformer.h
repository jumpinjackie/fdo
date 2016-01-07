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
ref class XmlReader;
ref class XmlWriter;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_COMMON
ref class DictionaryElementCollection;
END_NAMESPACE_OSGEO_COMMON

BEGIN_NAMESPACE_OSGEO_COMMON_XSL

/// \ingroup (OSGeoFDOCommonXsl)
/// \brief
/// XslTransformer provides a way to execute an XSL stylesheet
/// on an XML document.
public ref class XslTransformer sealed : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs an XSL Transformer object.
    /// 
	XslTransformer();

    /// \brief
    /// Constructs an XSL Transformer object.
    /// 
    /// \param inDoc 
    /// Input the XML document to transform.
    /// 
    XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ inDoc);

    /// \brief
    /// Constructs an XSL Transformer object.
    /// 
    /// \param inDoc 
    /// Input the XML document to transform.
    /// \param stylesheet 
    /// Input the XSL stylesheet with transformation instructions.
    /// 
    XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ inDoc, NAMESPACE_OSGEO_COMMON_XML::XmlReader^ stylesheet);

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
    /// 
	XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ inDoc, NAMESPACE_OSGEO_COMMON_XML::XmlReader^ stylesheet, NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ outDoc);

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
    XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ inDoc, NAMESPACE_OSGEO_COMMON_XML::XmlReader^ stylesheet, NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ outDoc, NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ log);

    /// \brief
    /// Gets the document being transformed
    /// 
    /// \return
    /// Returns the input document.
    /// 
    /// \brief
    /// Sets the document to transform.
    /// 
    /// \param inDoc 
    /// Input the XML document to transform.
    /// 
    property NAMESPACE_OSGEO_COMMON_XML::XmlReader^ InDoc
    {
        NAMESPACE_OSGEO_COMMON_XML::XmlReader^ get();
        System::Void set(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ inDoc);
    }

    /// \brief
    /// Gets the XSL Stylesheet
    /// 
    /// \return
    /// Returns the stylesheet.
    /// 
    /// \brief
    /// Sets the XSL stylesheet
    /// 
    /// \param stylesheet 
    /// Input the stylesheet
    /// 
    property NAMESPACE_OSGEO_COMMON_XML::XmlReader^ Stylesheet
    {
        NAMESPACE_OSGEO_COMMON_XML::XmlReader^ get();
        System::Void set(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ stylesheet);
    }

    /// \brief
    /// Gets the output document.
    /// 
    /// \return
    /// Returns the out document
    /// 
    /// \brief
    /// Sets the output document
    /// 
    /// \param outDoc 
    /// Input the output document
    /// <p><b>Note:</b> The output document is not completely written until outDoc
    /// and this transformer are destroyed by releasing all references to them.
    /// 
    property NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ OutDoc
    {
        NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ get();
        System::Void set(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ outDoc);
    }

    /// \brief
    /// Gets the input logging object.
    /// 
    /// \return
    /// Returns the input logging object
    /// 
    /// \brief
    /// Sets the input logging object
    /// 
    /// \param outDoc 
    /// Input the logging object
    /// 
    property NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ Log
    {
        NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ get();
        System::Void set(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ outDoc);
    }

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
    property NAMESPACE_OSGEO_COMMON::DictionaryElementCollection^ Parameters
    {
        NAMESPACE_OSGEO_COMMON::DictionaryElementCollection^ get();
    }

    /// \brief
    /// Performs the transformation.
    /// 
	System::Void Transform();

    /// \brief
    /// Constructs an XslTransformer based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	XslTransformer(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

/// \cond DOXYGEN-IGNORE
internal:
	inline FdoXslTransformer* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON_XSL


