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
#include "FdoWfsSchemaCopyHandler.h"
#include "FdoWfsGlobals.h"

FdoWfsSchemaCopyHandler* FdoWfsSchemaCopyHandler::Create( FdoXmlWriter* writer)
{
    return new FdoWfsSchemaCopyHandler(writer);
}

FdoWfsSchemaCopyHandler::FdoWfsSchemaCopyHandler( FdoXmlWriter* writer) :
                    FdoXmlCopyHandler(writer)
{
}

FdoXmlSaxHandler* FdoWfsSchemaCopyHandler::XmlStartElement( 
	FdoXmlSaxContext* saxContext,
	FdoString* uri, 
	FdoString* localName, 
	FdoString* qName, 
	FdoXmlAttributeCollection* atts 
)
{

    if ( FdoXml::mXsUri == uri ) {
        FdoStringP pLocalName( localName );

        if ( pLocalName == FdoWfsGlobals::import ||
            pLocalName == FdoWfsGlobals::include ||
            pLocalName == FdoWfsGlobals::annotation) {
                m_ignore.push_back(0);
        }
	}
    if (m_ignore.size() == 0)
        return FdoXmlCopyHandler::XmlStartElement( saxContext, uri, localName, qName, atts );
    else
        return NULL;
}

FdoBoolean FdoWfsSchemaCopyHandler::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
    if (m_ignore.size() == 0)
        ret = FdoXmlCopyHandler::XmlEndElement(context, uri, name, qname);

    
    if (FdoXml::mXsUri == uri ) {
        FdoStringP pLocalName( name );
        if ( pLocalName == FdoWfsGlobals::import ||
            pLocalName == FdoWfsGlobals::include ||
            pLocalName == FdoWfsGlobals::annotation) 
            m_ignore.pop_back();
    }
    return ret;

}

void FdoWfsSchemaCopyHandler::XmlCharacters(FdoXmlSaxContext* context, FdoString* content)
{
    if (m_ignore.size() == 0)
        FdoXmlCopyHandler::XmlCharacters(context, content);
}




