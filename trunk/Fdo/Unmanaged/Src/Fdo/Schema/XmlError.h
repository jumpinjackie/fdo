#ifndef FDO_SCHEMA_XML_ERROR_H
#define FDO_SCHEMA_XML_ERROR_H
// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#include "XmlContext.h"

///<summary>FdoSchemaXmlError is a SAX Handler for parsing error elements
///from an XML document. The error elements are created by the XSLT step
///when feature schema errors are encountered.</summary>
class FdoSchemaXmlError : 
    public FdoDisposable,
    public FdoXmlSaxHandler
{
public:
    ///<summary>Constructs an error element handler</summary>
    /// <param name="attrs">Input the error element's attributes</param> 
    /// <returns>Returns FdoSchemaXmlError</returns> 
	static FdoSchemaXmlError* Create( FdoXmlAttributeCollection* attrs );

    ///<summary>Applies the error by passing it to the given XML context.
    /// The error is only applied if it's error level is lower than the 
    /// current level.</summary>
    /// <param name="pContext">Input the XML context.</param> 
	void Apply( FdoSchemaXmlContext* pContext );

    // The following handle the start and end of the parameter sub-elements.
    // 
    virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );
    virtual FdoBoolean XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname
    );

protected:
    // Default constructor to keep linux from complaining
    FdoSchemaXmlError();

	FdoSchemaXmlError( FdoXmlAttributeCollection* attrs );
    virtual ~FdoSchemaXmlError() {}



private:
    // The error level of this error
    FdoXmlFlags::ErrorLevel     mErrorLevel;
    // The error number. The number of the FDO error message
    FdoInt32                    mErrorNum;
    // Substitution parameters for the message
    FdoStringsP                 mParms;
    // SAX Handler for reading each substitution parameter value.
    FdoXmlCharDataHandlerP      mXmlContentHandler;

};

///<summary>FdoSchemaXmlErrorP is a FdoPtr on FdoSchemaXmlError, provided for convenience.</summary>
typedef FdoPtr<FdoSchemaXmlError> FdoSchemaXmlErrorP;

#endif

