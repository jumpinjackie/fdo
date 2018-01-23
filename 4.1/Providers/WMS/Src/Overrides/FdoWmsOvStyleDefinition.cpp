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
#include "FdoWmsXmlGlobals.h"
#include <WMS/Override/FdoWmsOvStyleDefinition.h>

FdoWmsOvStyleDefinition::FdoWmsOvStyleDefinition(void)
{
}

FdoWmsOvStyleDefinition::~FdoWmsOvStyleDefinition(void)
{
}

FdoWmsOvStyleDefinition* FdoWmsOvStyleDefinition::Create()
{
    return new FdoWmsOvStyleDefinition();
}

void FdoWmsOvStyleDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    return BaseType::InitFromXml(pContext, attrs);
}

FdoXmlSaxHandler* FdoWmsOvStyleDefinition::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts)
{
    return BaseType::XmlStartElement(context, uri, name, qname, atts);
}

FdoBoolean FdoWmsOvStyleDefinition::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}

void FdoWmsOvStyleDefinition::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    VALIDATE_ARGUMENT(xmlWriter);
    VALIDATE_ARGUMENT(flags);
	
	xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsStyleDefinition);
	BaseType::_writeXml(xmlWriter, flags);
	xmlWriter->WriteEndElement();
}

void FdoWmsOvStyleDefinition::Dispose(void)
{
    delete this;
}
