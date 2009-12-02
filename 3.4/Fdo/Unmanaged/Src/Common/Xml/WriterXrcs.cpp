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
#include <FdoCommon.h>
#include <xercesc/util/XMLChar.hpp>
#include <xercesc/util/XMLString.hpp>
#include "WriterXrcs.h"
#include <Common/Xml/UtilXrcs.h>
#include <stdio.h>

FdoXmlWriterXrcs::FdoXmlWriterXrcs(FdoIoTextWriter* writer, FdoBoolean defaultRoot, LineFormat lineFormat, FdoSize lineLength) :
    FdoXmlWriter(writer, defaultRoot, lineFormat, lineLength)
{
}

FdoXmlWriterXrcs::~FdoXmlWriterXrcs(void)
{
    // Call to Close() has to be here instead of in base destructor.
    // Close eventually calls pure virtual function IsValidName(), which is 
    // implemented in this class. 
    Close();
}


FdoXmlWriterXrcs* FdoXmlWriterXrcs::Create( FdoIoTextWriter* writer, FdoBoolean defaultRoot, LineFormat lineFormat, FdoSize lineLength )
{
    // Start Xerces.
    FdoXmlUtilXrcs::Initialize();

    return new FdoXmlWriterXrcs(writer, defaultRoot, lineFormat, lineLength);
}

FdoStringP FdoXmlWriterXrcs::EncodeName( FdoStringP name )
{
    // Decode characters not allowed by FDO.
    FdoStringP lName = name.Replace( L"-dot-", L"." );
    lName = lName.Replace( L"-colon-", L":" );

    // Break the name up by '-' delimiters.

    FdoStringsP tokens = FdoStringCollection::Create( lName, L"-", true );
    FdoInt32 i;
    FdoSize start = 0;
    FdoSize end = 0;
    FdoStringP outName;
    FdoCharacter xChar = 0;
    FdoBoolean bFirstChar = true;

    // Check each token for the encoding pattern.
    for ( i = 0; i < tokens->GetCount(); i++ ) {
        // Tack on a character to match the whole token to the pattern.
        FdoStringP token = FdoStringP(tokens->GetString(i)) + L"x";

        xChar = 0;

        if ( (i > 0) && (swscanf( (const FdoString*) token, L"x%xx", &xChar ) > 0) && (xChar > 0) ) {
            // the token happens to match the encoding pattern. We want to avoid
            // decoding this sub-string on decode. This is done by encoding the leading
            // dash.
            outName += FdoStringP::Format( L"-x%x-", '-' );
        }
        else {
            // The token doesn't match the encoding pattern, just write the dash
            // that was discarded by the tokenizer.

            if ( i > 0  ) 
                outName += L"-";
        }

        // Re-retrieve the token without the appended 'x'
        token = tokens->GetString(i);

        start = 0;
        end = 0;
        const FdoString* inToken = token;

        // check each character in the token
        for ( end = 0; end < token.GetLength(); end++ ) {
            FdoBoolean bValid = false;

            // check if character is valid
            if ( bFirstChar ) {
                bValid = XERCES_CPP_NAMESPACE::XMLChar1_0::isFirstNameChar( inToken[end] );
                bFirstChar = false;
            }
            else {
                bValid = XERCES_CPP_NAMESPACE::XMLChar1_0::isNameChar( inToken[end] );
            }

            if ( !bValid ) {
                // Not valid so write out the previous characters
                if ( end > start ) 
                    outName += token.Mid( start, end - start );

                // Encode this character
                outName += FdoStringP::Format( L"-x%x-", inToken[end] );
                start = end + 1;
            }
        }

        // Write any characters left over
        if ( end > start ) 
            outName += token.Mid( start, end - start );
    }

    return( outName );
}

FdoBoolean FdoXmlWriterXrcs::IsValidName( FdoStringP name )
{
    XMLCh* xName = FdoXmlUtilXrcs::Unicode2Xrcs( name );
    FdoBoolean isValid = XERCES_CPP_NAMESPACE::XMLChar1_0::isValidQName( xName, (const unsigned int)name.GetLength() );
    XERCES_CPP_NAMESPACE::XMLString::release( &xName );
    
    return isValid;
}


