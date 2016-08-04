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

#include <Fdo/Commands/Schema/PhysicalElementMapping.h>
#include <Rdbms/Override/RdbmsOvPropertyMappingConcrete.h>

FdoRdbmsOvPropertyMappingConcrete::FdoRdbmsOvPropertyMappingConcrete()
{
}

FdoRdbmsOvPropertyMappingConcrete::~FdoRdbmsOvPropertyMappingConcrete()
{
}

void FdoRdbmsOvPropertyMappingConcrete::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvPropertyMappingDefinition::InitFromXml( pContext, attrs );

}

void FdoRdbmsOvPropertyMappingConcrete::_writeXml(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    xmlWriter->WriteStartElement( L"PropertyMappingConcrete" );

    FdoRdbmsOvPropertyMappingRelation::_writeXml( xmlWriter, flags );

    xmlWriter->WriteEndElement();
}

FdoXmlSaxHandler* FdoRdbmsOvPropertyMappingConcrete::XmlStartElement(
    FdoXmlSaxContext* context,
    FdoString* uri,
    FdoString* name,
    FdoString* qname,
    FdoXmlAttributeCollection* atts
)
{
    // Handle generic sub-elements.
    FdoXmlSaxHandler* pRet = FdoRdbmsOvPropertyMappingRelation::XmlStartElement(context, uri, name, qname, atts);

    if ( !pRet )
        // report unexpected sub-elements.
        pRet = SubElementError( context, L"PropertyMappingConcrete", name );

    return(pRet);
}

