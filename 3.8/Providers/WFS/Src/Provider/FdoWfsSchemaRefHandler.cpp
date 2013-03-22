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

#include "stdafx.h"
#include "FdoWfsSchemaRefHandler.h"
#include "FdoWfsGlobals.h"

FdoWfsSchemaRefHandler::FdoWfsSchemaRefHandler()
{
    m_refs = FdoDictionary::Create();
}

FdoXmlSaxHandler* FdoWfsSchemaRefHandler::XmlStartElement( 
	FdoXmlSaxContext* saxContext,
	FdoString* uri, 
	FdoString* localName, 
	FdoString* qName, 
	FdoXmlAttributeCollection* atts 
)
{
    if ( FdoXml::mXsUri == uri ) {
        FdoStringP pLocalName( localName );
        FdoXmlAttributeP nsAtt;
        FdoXmlAttributeP locationAtt;

        if ( pLocalName == FdoWfsGlobals::import ) {
            nsAtt = atts->FindItem( FdoWfsGlobals::NameSpace );
            locationAtt = atts->FindItem( FdoWfsGlobals::schemaLocation );
        }

        if ( pLocalName == FdoWfsGlobals::include ) {
            locationAtt = atts->FindItem( FdoWfsGlobals::schemaLocation );
        }

        if ( locationAtt ) {
            FdoStringP location = locationAtt->GetValue();

            // Avoid adding same location twice.
            if ( !m_refs->FindItem(location) ) {
                FdoStringP uri;
                if ( nsAtt ) 
                    uri = nsAtt->GetValue();

                FdoDictionaryElementP item = FdoDictionaryElement::Create( location, uri );

                m_refs->Add( item );
            }
        }
	}
    return NULL;
}



