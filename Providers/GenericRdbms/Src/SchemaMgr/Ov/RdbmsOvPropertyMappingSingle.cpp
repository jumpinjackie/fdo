
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
#include <Rdbms/Override/RdbmsOvPropertyMappingSingle.h>

FdoRdbmsOvPropertyMappingSingle::FdoRdbmsOvPropertyMappingSingle()
{
}

FdoRdbmsOvPropertyMappingSingle::~FdoRdbmsOvPropertyMappingSingle()
{
}

FdoString* FdoRdbmsOvPropertyMappingSingle::GetPrefix()
{
    return mPrefix;
}

void FdoRdbmsOvPropertyMappingSingle::SetPrefix(FdoString *prefix)
{
    mPrefix = prefix;
}

void FdoRdbmsOvPropertyMappingSingle::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvPropertyMappingDefinition::InitFromXml( pContext, attrs );

    FdoXmlAttributeP att = attrs->FindItem( L"prefix" );
    if ( att ) {
        mPrefix = att->GetValue();
    }
}

void FdoRdbmsOvPropertyMappingSingle::_writeXml(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    xmlWriter->WriteStartElement( L"PropertyMappingSingle" );

    if ( mPrefix.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"prefix", mPrefix );

    FdoRdbmsOvPropertyMappingDefinition::_writeXml( xmlWriter, flags );

    xmlWriter->WriteEndElement();
}

FdoXmlSaxHandler* FdoRdbmsOvPropertyMappingSingle::XmlStartElement(
    FdoXmlSaxContext* context,
    FdoString* uri,
    FdoString* name,
    FdoString* qname,
    FdoXmlAttributeCollection* atts
)
{
    // Handle generic sub-elements.
    FdoXmlSaxHandler* pRet = FdoRdbmsOvPropertyMappingDefinition::XmlStartElement(context, uri, name, qname, atts);

    if ( !pRet )
        // report unexpected sub-elements.
        pRet = SubElementError( context, L"PropertyMappingSingle", name );

    return(pRet);
}



