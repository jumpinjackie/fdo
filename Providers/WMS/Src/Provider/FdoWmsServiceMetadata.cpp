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

#include <stdafx.h>
#include "FdoWmsServiceMetadata.h"
#include "FdoWmsXmlGlobals.h"

FdoWmsServiceMetadata::FdoWmsServiceMetadata(void)
{
    m_capabilities = FdoWmsCapabilities::Create();
}

FdoWmsServiceMetadata::~FdoWmsServiceMetadata(void)
{
}

void FdoWmsServiceMetadata::Dispose() 
{
    delete this;
}

FdoWmsServiceMetadata* FdoWmsServiceMetadata::Create()
{
    return new FdoWmsServiceMetadata();
}

FdoXmlSaxHandler* FdoWmsServiceMetadata::XmlStartElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
    return BaseType::XmlStartElement(context, uri, name, qname, atts);
}

FdoBoolean FdoWmsServiceMetadata::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}

FdoOwsCapabilities* FdoWmsServiceMetadata::OnCreateCapabilities()
{
    return FDO_SAFE_ADDREF(m_capabilities.p);
}

FdoOwsCapabilities* FdoWmsServiceMetadata::GetCapabilities() const
{
    return FDO_SAFE_ADDREF(m_capabilities.p);
}
