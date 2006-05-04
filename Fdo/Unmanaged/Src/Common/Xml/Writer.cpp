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
#include <Common.h>
#include "WriterXrcs.h"
#include <stdio.h>

FdoXmlWriter::FdoXmlWriter(
    FdoIoTextWriter* writer, 
    FdoBoolean defaultRoot, 
    LineFormat lineFormat,
    FdoSize    lineLength
) :
    mbDefaultRoot(defaultRoot),
    mbTagOpen(false),
    mbPrologueWritten(false),
    mbElementWritten(false),
    mbCharsWritten(false),
    mLineFormat(lineFormat),
    mLineLength(lineLength),
    mIndentLevel(0),
    mIndent(L"   "),
	mCharWritten(0)
{
    FDO_SAFE_ADDREF(writer);
    mTextWriter = writer;

    mElementStack = ElementStack::Create();
}

FdoXmlWriter::~FdoXmlWriter(void)
{
}


FdoXmlWriter* FdoXmlWriter::Create( 
    FdoString* fileName, 
    FdoBoolean defaultRoot, 
    LineFormat lineFormat,
    FdoSize    lineLength
)
{
    FdoIoTextWriterP pWriter = FdoIoTextWriter::Create(fileName);

    return( FdoXmlWriter::Create(pWriter, defaultRoot, lineFormat, lineLength ) );
}

FdoXmlWriter* FdoXmlWriter::Create( 
    FdoIoStream* stream, 
    FdoBoolean defaultRoot, 
    LineFormat lineFormat,
    FdoSize    lineLength
)
{
    FdoIoTextWriterP pWriter = FdoIoTextWriter::Create(stream);

    return( FdoXmlWriter::Create(pWriter, defaultRoot, lineFormat, lineLength) );
}

FdoXmlWriter* FdoXmlWriter::Create(
    FdoIoTextWriter* writer, 
    FdoBoolean defaultRoot, 
    LineFormat lineFormat,
    FdoSize    lineLength
)
{
    if ( !writer ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"writer",
                L"(NULL)",
                L"FdoXmlWriter::Create"
            )
        );

    // Write using Xerces utility functions.
    return( FdoXmlWriterXrcs::Create(writer, defaultRoot, lineFormat, lineLength) );
}

FdoIoTextWriter* FdoXmlWriter::GetTextWriter()
{
    FdoIoTextWriter* pWriter = mTextWriter;
    FDO_SAFE_ADDREF(pWriter);

    return pWriter;
}

void FdoXmlWriter::Close()
{
    // When defaultRoot is true, the following ensures that the default root
    // element is written if nothing else was written to the document.
    WritePrologue();

    // Close any open elements
    while( !mElementStack->IsEmpty() ) 
        WriteEndElement();

    mbElementWritten = true;
}

void FdoXmlWriter::SetDefaultRoot( FdoBoolean defaultRoot )
{
    if ( defaultRoot != mbDefaultRoot ) {
        // Once the prologue is written it is too late to change the default
        // root element setting, since the root element has already been written.
        if ( mbPrologueWritten ) 
            throw FdoXmlException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(FDO_44_BADDEFAULTROOT)
                )
            );

        mbDefaultRoot = defaultRoot;
    }
}

void FdoXmlWriter::WriteStartElement( FdoString* elementName )
{
    if ( !elementName ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"elementName",
                L"(NULL)",
                L"FdoXmlWriter::WriteStartElement"
            )
        );

    if ( !IsValidName(elementName) ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_40_BADXMLELEMENT),
                elementName
            )
        );

    // The following ensures that we don't write 2 root elements,
    // XML allows only one root.
    if ( mbElementWritten && (mElementStack->IsEmpty()) ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_49_MULTIROOTELEMENT),
                L"FdoXmlWriter::WriteStartElement",
                elementName
            )
        );

    FdoIoTextWriterP pWriter = GetTextWriter();

    WritePrologue();

    // Close any element start tag that is still open.
    CloseStartElementTag();

    // This is now the current element so put it on the stack.
    mElementStack->Push( StackElementP(StackElement::Create(elementName, this)) );

    WriteIndent();
    mIndentLevel++;

    // Write the element start tag.
	if (mLineLength != 0) {
		FdoStringP str =  FdoStringP::Format(L"<%ls", elementName); 
		pWriter->Write(str);
		mCharWritten += str.GetLength();
		if (mCharWritten >= mLineLength)
			WriteIndent();
	} else
		pWriter->Write( FdoStringP::Format(L"<%ls", elementName));

    mbTagOpen = true;
    mbElementWritten = true;
}

void FdoXmlWriter::WriteEndElement()
{
    // Error if no element to end.
    if ( mElementStack->IsEmpty() ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(FDO_NLSID(FDO_31_ENDELEMENTERROR))
        );
    
    
    // Get the document writer.
    FdoIoTextWriterP pWriter = GetTextWriter();


    if ( mbTagOpen ) {
		// An element start tag is open, so flush its attributes and
		// end it. Pop the element now that it is complete.
        StackElementP(mElementStack->Pop())->FlushAttributes( pWriter );
        pWriter->Write( L"/>" );
        mbTagOpen = false;
        mIndentLevel--;
    }
    else {
        mIndentLevel--;
        // Otherwise, write an end tag
        // Pop the current element.
        StackElementP topElement = mElementStack->Pop();

        WriteIndent();
        pWriter->Write( FdoStringP::Format(L"</%ls>", (FdoString*) (topElement->GetName())) );
    }

}

void FdoXmlWriter::WriteAttribute( FdoString* attributeName, FdoString* attributeValue )
{
    if ( !attributeName ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"attributeName",
                L"(NULL)",
                L"FdoXmlWriter::WriteAttribute"
            )
        );

    if ( !IsValidName(attributeName) ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_41_BADXMLATTRIBUTE),
                attributeName
            )
        );

    // Error if no current element.
    if ( mElementStack->IsEmpty() ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_123_NOATTRIBUTEELEMENT),
                L"FdoXmlWriter::WriteAttribute",
                attributeName
            )
        );

    StackElementP topElement = mElementStack->Peek();

    if ( !mbTagOpen ) {
        // Error if current element start tag is no longer open.

        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_32_XMLATTRIBUTEERROR),
                attributeName,
                (FdoString*) (topElement->GetName())
            )
        );
    }

	// Put the attribute on the currently open element
	topElement->SetAttribute( attributeName, attributeValue );
}
    
void FdoXmlWriter::WriteCharacters( FdoString* characters )
{

    if ( characters ) {
        // Error if no current element.
        if ( mElementStack->IsEmpty() ) 
            throw FdoXmlException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(FDO_48_NOCONTENTELEMENT),
                    L"FdoXmlWriter::WriteCharacters",
                    characters
                )
            );

        FdoIoTextWriterP pWriter = GetTextWriter();
    
        // Close any open start tag
        CloseStartElementTag();
		pWriter->Write( StackElement::EncodeValue(characters) );
        if ( wcslen(characters) > 0 ) 
            mbCharsWritten = true;
    }
}

void FdoXmlWriter::WriteBytes( FdoByte* bytes, FdoSize count )
{
    // The following ensures that we don't write bytes after the
    // root element.
    if ( mbElementWritten && (mElementStack->IsEmpty()) ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_124_BYTESAFTERCLOSE),
                L"FdoXmlWriter::WriteBytes"
            )
        );

    FdoIoStreamP pStream = GetStream();

    WritePrologue();
    // Close any open start tag
    CloseStartElementTag();

    pStream->Write( bytes, count );
}

FdoStringP FdoXmlWriter::UriToQName( FdoString* uri, FdoString* localName, FdoBoolean isElement )
{
	WritePrologue();

    return( mElementStack->UriToQName(uri, localName, isElement) );
}

void FdoXmlWriter::WritePrologue()
{

    if ( !mbPrologueWritten) {

        FdoIoTextWriterP pWriter = GetTextWriter();
        
        // First element so write the prologue
        pWriter->Write( L"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" );
        mbPrologueWritten = true;
        
        if ( mbDefaultRoot ) {
            // Using default root element so write it first.
			WriteStartElement( FdoXml::mDefaultRoot );

            // Declare the namespaces that will most likely be used in the document.
            WriteAttribute( FdoXml::mXmlnsPref + L":" + FdoXml::mXsNs, FdoXml::mXsUri );
            WriteAttribute( FdoXml::mXmlnsPref + L":" + FdoXml::mXsiNs, FdoXml::mXsiUri );
            WriteAttribute( FdoXml::mXmlnsPref + L":" + FdoXml::mXlinkNs, FdoXml::mXlinkUri );
            WriteAttribute( FdoXml::mXmlnsPref + L":" + FdoXml::mGmlNs, FdoXml::mGmlUri );
            WriteAttribute( FdoXml::mXmlnsPref + L":" + FdoXml::mFdoNs, FdoXml::mFdoUri );
            WriteAttribute( FdoXml::mXmlnsPref + L":" + FdoXml::mFdsNs, FdoXml::mFdsUri );
        }
    }
}

void FdoXmlWriter::CloseStartElementTag()
{
    if ( mbTagOpen ) {
        FdoIoTextWriterP pWriter = GetTextWriter();
        
		// Write the current element's attributes.
        StackElementP(mElementStack->Peek())->FlushAttributes( pWriter );

		// Close the start tag but leave the element open.
		pWriter->Write( L">" );
        mbTagOpen = false;
    }
}

FdoXmlWriter::StackElement::StackElement( FdoStringP elementName, FdoXmlWriter* writer ) : mWriter(writer)
{
    mName = elementName;
    mAtts = FdoXmlAttributeCollection::Create();
    mHasNsDecl = false;
}

bool FdoXmlWriter::StackElement::GetHasNsDecl()
{
    return mHasNsDecl;
}

void FdoXmlWriter::StackElement::SetAttribute( FdoString* attributeName, FdoString* attributeValue )
{
    FdoXmlAttributeP att = mAtts->FindItem( attributeName );
    FdoStringP attPrefix;
    FdoStringP attLocalName;

    if ( att ) {
		// Attribute already set, remove and re-add.
		// This is similar to Xalan, where an attribute ends up with the
		// last value it is set to. This also ensures that we don't write
		// attributes with duplicate names.
        mAtts->Remove((FdoXmlAttribute*) att);
    }

    // The following separates prefixed attribute names into their prefix and 
    // local name parts

    FdoStringsP strings = FdoStringCollection::Create( attributeName, L":" );
    if ( strings->GetCount() == 2 ) {
        attPrefix = strings->GetString(0);
        attLocalName = strings->GetString(1);

        // If this attribute is a namespace declaration, flag this 
        // element as declaring a namespace.
        if ( attPrefix == FdoXml::mXmlnsPref )
            mHasNsDecl = true;
    }
    else if ( strings->GetCount() == 1 ) {
        // If attribute has no prefix, check if it is a default namespace declaration
        if ( FdoXml::mXmlnsPref == attributeName ) {
            mHasNsDecl = true;
            attPrefix = attributeName;
        }
    }

	// Add the attribute
    att = FdoXmlAttribute::Create( attributeName, attributeValue, attLocalName, NULL, attPrefix );
    mAtts->Add( att );
}

void FdoXmlWriter::StackElement::FlushAttributes( FdoIoTextWriterP pWriter )
{
    FdoInt32            idx;

    // Write each attribute to the current element.
    for ( idx = 0; idx < mAtts->GetCount(); idx++ ) {
        FdoXmlAttributeP att = mAtts->GetItem( idx );
        FdoStringP attVal = EncodeValue(att->GetValue());
		if (mWriter->mLineLength != 0) {
            FdoStringP str = FdoStringP::Format(L" %ls=\"%ls\"",att->GetName(), (FdoString*) attVal);
            if (mWriter->mCharWritten != 0 &&
                mWriter->mCharWritten + str.GetLength() >= mWriter->mLineLength)
                mWriter->WriteIndent();
            pWriter->Write(str);
            mWriter->mCharWritten += str.GetLength();
			
		} else
			pWriter->Write(FdoStringP::Format(L" %ls=\"%ls\"",att->GetName(), (FdoString*) attVal));
    }
}

FdoStringP FdoXmlWriter::StackElement::UriToQName( FdoString* uri, FdoString* localName, FdoBoolean isElement )
{
    FdoInt32 xa;
    FdoStringP qName;

	// Search the attributes for a matching declaration.
    for ( xa = 0; xa < mAtts->GetCount(); xa++ ) {
        FdoXmlAttributeP att = mAtts->GetItem(xa);
        FdoStringP attPrefix = att->GetPrefix();

		// Check if attribute has the form "xmls=<uri>" or 
		// "xmlns:<namespace>=<uri>
        if ( (attPrefix == FdoXml::mXmlnsPref) &&
             (wcscmp(att->GetValue(), uri) == 0)
        ) {
            FdoStringP attLocalName = att->GetLocalName();

			if ( attLocalName != L"" ) {
				// This is a non-default declaration: "xmlns:<namespace>=<uri>"
				// if qName not already set, generate it as "<namespace>:<localName>"
				if ( qName.GetLength() == 0 ) {
					qName = attLocalName + L":";
					qName += localName;
				}
            }
			else {
                // default namespace does not apply to attributes (XML spec).
                if ( isElement ) 
				    // This is a default declaration: "xmls=<uri>"
				    // Let the default declaration take precedence.
				    qName = localName;
			}
        }
    }

    return( qName );
}

FdoStringP FdoXmlWriter::ElementStack::UriToQName( FdoString* uri, FdoString* localName, FdoBoolean isElement )
{
    FdoInt32 xe;
    FdoStringP qName;

	// The following looks for the in-scope namespace declaration that
	// references the given uri. The in-scope declaration is the topmost
	// one so start at the top element and search downward until the 
	// first declaration is found.
    for ( xe = 0; StackElementP(Peek(xe)) && (qName.GetLength() == 0); xe++ ) {
        StackElementP stackElem = Peek(xe);
        // For performance, skip elements that don't declare namespaces.
        if ( stackElem->GetHasNsDecl() )
            qName = stackElem->UriToQName( uri, localName, isElement );
    }

    return( qName );
}

void FdoXmlWriter::WriteIndent()
{
    int i;

    if ( (mLineFormat != LineFormat_None) && (!mbCharsWritten) ) {
        FdoIoTextWriterP pWriter = GetTextWriter();
        pWriter->WriteLine( L"" );

        if ( mLineFormat == LineFormat_Indent ) {
            for ( i = 0; i < mIndentLevel; i++ ) {
                pWriter->Write( mIndent );
            }
        }
        mCharWritten = 0;
    }

    mbCharsWritten = false;
}

FdoStringP FdoXmlWriter::StackElement::EncodeValue ( FdoStringP value )
{
    return value.Replace(L"&",L"&amp;").Replace(L"<",L"&lt;").Replace(L">",L"&gt;");
}


