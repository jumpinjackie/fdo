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
#include <FdoStd.h>
#include <Fdo/Xml/Deserializable.h>


FdoXmlDeserializable::FdoXmlDeserializable()
{
}
 
FdoXmlDeserializable::~FdoXmlDeserializable()
{
}


void FdoXmlDeserializable::ReadXml(
    FdoString* fileName, 
      FdoXmlFlags* flags
)
{
    ReadXml( FdoXmlReaderP(FdoXmlReader::Create(fileName)), flags );
}

void FdoXmlDeserializable::ReadXml(
       FdoIoStream* stream, 
       FdoXmlFlags* flags
)
{
    ReadXml( FdoXmlReaderP(FdoXmlReader::Create(stream)), flags );
}

void FdoXmlDeserializable::ReadXml(
       FdoIoTextReader* textReader, 
       FdoXmlFlags* flags
)
{
    ReadXml( FdoXmlReaderP(FdoXmlReader::Create(textReader)), flags );
}

void FdoXmlDeserializable::ReadXml(
    FdoXmlReader* xmlReader, 
    FdoXmlFlags* flags
)
{
    FDO_SAFE_ADDREF(xmlReader);
    mInternalReader = xmlReader;

    if ( flags ) {
		FDO_SAFE_ADDREF(flags);
        mFlags = flags;
    }
    else {
        // Create default flags since none were passed in.
        mFlags = FdoXmlFlags::Create();
    }

    FdoIoMemoryStreamP internalStream;
    FdoXmlWriterP internalWriter;
    FdoXslTransformerP transformer;

    FdoXmlReaderP stylesheet = GetFromExternalStylesheet();

    if ( stylesheet.p ) {
        // A stylesheet has been defined so convert the XML document to 
        // internal format

        // Create a memory stream to hold the internal format
        internalStream = FdoIoMemoryStream::Create();
        internalWriter = FdoXmlWriter::Create( internalStream );

        // Transform to internal format
        transformer = FdoXslTransformer::Create( xmlReader, stylesheet, internalWriter );

        // Pass down various flags to the XSL Stylesheet
        FdoDictionaryP params = transformer->GetParameters();
        params->Add( 
            FdoDictionaryElementP( 
                FdoDictionaryElement::Create( 
                    L"customer_url", 
                    FdoStringP::Format( L"'%ls'", mFlags->GetUrl() )
                ) 
            ) 
        );

        params->Add( 
            FdoDictionaryElementP( 
                FdoDictionaryElement::Create( 
                    L"schema_name_as_prefix", 
                    FdoStringP::Format( L"'%ls'", mFlags->GetSchemaNameAsPrefix() ? L"yes" : L"no" )
                ) 
            ) 
        );

        params->Add( 
            FdoDictionaryElementP( 
                FdoDictionaryElement::Create( 
                    L"element_default_nullability", 
                    FdoStringP::Format( L"'%ls'", mFlags->GetElementDefaultNullability() ? L"yes" : L"no" )
                ) 
            ) 
        );

        params->Add( 
            FdoDictionaryElementP( 
                FdoDictionaryElement::Create( 
                    L"use_gml_id", 
                    FdoStringP::Format( L"'%ls'", mFlags->GetUseGmlId() ? L"yes" : L"no" )
                ) 
            ) 
        );

        transformer->Transform();
        transformer = NULL;
        internalWriter = NULL;

        // Rewind the internal format stream
        internalStream->Reset();
#ifdef XML_DEBUG
        FdoIoFileStreamP temp = FdoIoFileStream::Create( L"temp_in.xml", L"w+" );
        temp->Write(internalStream);
        temp = NULL;
        internalStream->Reset();
#endif
        // read from the internal format stream instead
        mInternalReader = FdoXmlReader::Create( internalStream );
    }

    // perform the XML read.
    mInternalReader->Parse( (FdoXmlSaxHandler*) this, FdoXmlSaxContextP(GetSaxContext()) );
}

FdoXmlFlags* FdoXmlDeserializable::GetDeserializationFlags()
{
    FdoXmlFlags* flags = mFlags;
    FDO_SAFE_ADDREF( flags );

    return flags;
}

FdoXmlReader* FdoXmlDeserializable::GetXmlReader()
{
    FdoXmlReader* pReader = mInternalReader;
    FDO_SAFE_ADDREF( pReader );

    return(pReader);
}

FdoXmlReader* FdoXmlDeserializable::GetFromExternalStylesheet() 
{
    return NULL;
}

FdoXmlSaxContext* FdoXmlDeserializable::GetSaxContext() 
{
    return NULL;
}



