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
#include "XmlError.h"

FdoSchemaXmlError::FdoSchemaXmlError( FdoXmlAttributeCollection* attrs )
{
   mParms = FdoStringCollection::Create();

   // Convert error level from character to enum.

   FdoStringP ErrorLevel = FdoXmlAttributeP(attrs->GetItem( L"level" ))->GetValue();

   switch ( *((const FdoString*) ErrorLevel) ) {
   case 'h':
       mErrorLevel = FdoXmlFlags::ErrorLevel_High;
       break;
   case 'l':
       mErrorLevel = FdoXmlFlags::ErrorLevel_Low;
       break;
   case 'v':
       mErrorLevel = FdoXmlFlags::ErrorLevel_VeryLow;
       break;
   default:
       mErrorLevel = FdoXmlFlags::ErrorLevel_Normal;
       break;
   }

   // Get the error message number.
   mErrorNum = FdoStringP(FdoXmlAttributeP(attrs->GetItem( L"number" ))->GetValue()).ToLong();
}

FdoSchemaXmlError* FdoSchemaXmlError::Create( FdoXmlAttributeCollection* attrs )
{
    return new FdoSchemaXmlError( attrs );
}

void FdoSchemaXmlError::Apply( FdoSchemaXmlContext* pContext )
{
    FdoFeatureSchemasP schemas = pContext->GetMergeContext()->GetSchemas();
    FdoXmlFlagsP flags = schemas->GetDeserializationFlags();
    FdoXmlFlags::ErrorLevel errorLevel = flags->GetErrorLevel();
    FdoInt32 i;

    // Skip error 23 when error level is high. Error 56 (always present in this case)
    // supercedes it.
    if ( (errorLevel == FdoXmlFlags::ErrorLevel_High) && (mErrorNum == SCHEMA_23_UNSUPPORTEDXSDGBLELEM) )
        return;

    // Error levels are in reverse order. Log the error only if 
    // this error's level is not higher than the current level.
    if ( mErrorLevel >= errorLevel ) {

        // Convert error number to message string.
        FdoStringP errMsg( FdoException::NLSGetMessage(mErrorNum, "XML Error") );

        // Substitute in the parameters.
        // TODO: try to handle this in NLSGetMessage.
        // Also: try to switch substitution indicator from '#' to 
        // something more standard like '%'.
        for ( i = 0; i < mParms->GetCount(); i++ ) {
            errMsg = errMsg.Replace( 
                (const FdoString*) FdoStringP::Format( L"#%d$ls", i+1 ), 
                (const FdoString*) mParms->GetItem(i)->GetString()
            );
        }

        pContext->AddError( FdoSchemaExceptionP(FdoSchemaException::Create(errMsg)) );
    }
}

FdoXmlSaxHandler* FdoSchemaXmlError::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* pRet = NULL;

    if ( wcscmp( name, L"P" ) == 0 ) {
        // Set a SAX Handler for reading the parameter value.
        mXmlContentHandler = FdoXmlCharDataHandler::Create();
        pRet = mXmlContentHandler;
    }

    return(pRet);
}

FdoBoolean FdoSchemaXmlError::XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname
)
{
    if ( mXmlContentHandler != NULL ) {
        if ( wcscmp(name, L"P") == 0 ) 
            // Add parameter value to parameter list.
            mParms->Add( mXmlContentHandler->GetString() );
    }

    return(false);
}

