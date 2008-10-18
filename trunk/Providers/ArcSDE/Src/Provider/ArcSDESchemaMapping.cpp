/*
 * 
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
#include "ArcSDEProvider.h"


ArcSDESchemaMapping::ArcSDESchemaMapping(void)
{
    m_ClassMappings = ArcSDEClassMappingCollection::Create(this);
}

ArcSDESchemaMapping::~ArcSDESchemaMapping(void)
{
}

FdoString* ArcSDESchemaMapping::GetProvider()
{
    return ARCSDE_PROVIDER_NAME;
}


ArcSDEClassMappingCollection* ArcSDESchemaMapping::GetClasses()
{
    return FDO_SAFE_ADDREF(m_ClassMappings.p);
}


void ArcSDESchemaMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    //TODO
}

FdoXmlSaxHandler* ArcSDESchemaMapping::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    //TODO
    return(NULL);
}

FdoBoolean ArcSDESchemaMapping::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname
)
{
    //TODO
    return(false);
}


void ArcSDESchemaMapping::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    //TODO
}



ArcSDESchemaMapping* ArcSDESchemaMapping::Create(void)
{
    return new ArcSDESchemaMapping();
}

