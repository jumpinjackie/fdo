// 
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
//  
#include <Std.h>
#include <Fdo/Xml/Serializable.h>

void FdoXmlSerializable::WriteXml(
    FdoString* fileName, 
      FdoXmlFlags* flags
)
{
    // The following also creates an XML document, with this object as its only member.
    WriteXml( FdoXmlWriterP(FdoXmlWriter::Create(fileName,false)), flags );
}

void FdoXmlSerializable::WriteXml(
       FdoIoStream* stream, 
       FdoXmlFlags* flags
)
{
    // The following also creates an XML document, with this object as its only member.
    WriteXml( FdoXmlWriterP(FdoXmlWriter::Create(stream,false)), flags );
}

void FdoXmlSerializable::WriteXml(
       FdoIoTextWriter* textWriter, 
       FdoXmlFlags* flags
)
{
    // The following also creates an XML document, with this object as its only member.
    WriteXml( FdoXmlWriterP(FdoXmlWriter::Create(textWriter,false)), flags );
}

void FdoXmlSerializable::WriteXml(
    FdoXmlWriter* xmlWriter, 
    FdoXmlFlags* flags
)
{
    // This function is different from the other 3 in that the object is just appended
    // to the currently open XML document.

    FdoIoMemoryStreamP internalStream;
    xmlWriter->AddRef();
    // By default, internal and external formats are the same.
    FdoXmlWriterP internalWriter = xmlWriter;
    FdoXmlReaderP internalReader;
    FdoXslTransformerP transformer;

    FdoXmlFlagsP lFlags;

    if ( flags ) {
        flags->AddRef();
        lFlags = flags;
    }
    else {
        // Create default flags since none passed in
        lFlags = FdoXmlFlags::Create();
    }

    FdoXmlReaderP stylesheet = GetFromInternalStylesheet();

    if ( stylesheet != NULL ) {
        // An internal format has been defined. Therefore, write to internal format
        // in a temporary in-memory stream.
        internalStream = FdoIoMemoryStream::Create();
        internalWriter = FdoXmlWriter::Create( internalStream, xmlWriter->GetDefaultRoot() );
    }

    // Serialize the object.
    _writeXml( internalWriter, lFlags );


    // If no internal format then we're done ...
    if ( stylesheet != NULL ) {
        // Otherwise convert from internal to external format

        // Need default wrapper element if intermediate XML document has one.
        if ( internalWriter->GetDefaultRoot() && !xmlWriter->GetDefaultRoot() ) 
            xmlWriter->SetDefaultRoot( internalWriter->GetDefaultRoot() );

        // Close off all open elements in the internal document.
        internalWriter = NULL;

        internalStream->Reset();

#ifdef XML_DEBUG
        FdoIoFileStreamP temp = FdoIoFileStream::Create( L"temp_out.xml", L"w+" );
        temp->Write(internalStream);
        temp = NULL;
        internalStream->Reset();
#endif
        internalReader = FdoXmlReader::Create( internalStream );

        // Transform from temporary stream to output writer.
        transformer = FdoXslTransformer::Create( internalReader, stylesheet, xmlWriter );
        
        // When writing a Feature Schema, transformer needs customer_url to generate
        // target namespace for the schema.
        FdoDictionaryP params = transformer->GetParameters();
        params->Add( 
            FdoDictionaryElementP( 
                FdoDictionaryElement::Create( 
                    L"customer_url", 
                    FdoStringP::Format( L"'%ls'", lFlags->GetUrl() )
                ) 
            ) 
        );

        // Do the transformation.
        transformer->Transform();
    }
}

FdoXmlReader* FdoXmlSerializable::GetFromInternalStylesheet() 
{
    return NULL;
}

