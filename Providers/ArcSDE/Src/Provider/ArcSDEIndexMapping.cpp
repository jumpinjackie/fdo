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


ArcSDEIndexMapping::ArcSDEIndexMapping(void)
: m_Unique(true),
  m_Ascending(true)
{
    m_IndexPropertyMappings = FdoStringCollection::Create();
}

ArcSDEIndexMapping::~ArcSDEIndexMapping(void)
{
}


bool ArcSDEIndexMapping::IsUnique()
{
    return m_Unique;
}

void ArcSDEIndexMapping::SetUnique(bool value)
{
    m_Unique = value;
}

bool ArcSDEIndexMapping::IsAscending()
{
    return m_Ascending;
}

void ArcSDEIndexMapping::SetAscending(bool value)
{
    m_Ascending = value;
}

FdoStringCollection* ArcSDEIndexMapping::GetPropertyNames()
{
    return FDO_SAFE_ADDREF(m_IndexPropertyMappings.p);
}


void ArcSDEIndexMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    //TODO
}

FdoXmlSaxHandler* ArcSDEIndexMapping::XmlStartElement(
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

FdoBoolean ArcSDEIndexMapping::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname
)
{
    //TODO
    return(false);
}



ArcSDEIndexMapping* ArcSDEIndexMapping::Create(void)
{
    return new ArcSDEIndexMapping();
}


ArcSDEIndexMappingCollection* ArcSDEIndexMappingCollection::Create(ArcSDEClassMapping *parent)
{
    return new ArcSDEIndexMappingCollection(parent);
}

