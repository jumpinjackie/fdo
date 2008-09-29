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


ArcSDEPropertyMapping::ArcSDEPropertyMapping() :
    m_ColumnType(-1L)
{
}

ArcSDEPropertyMapping::~ArcSDEPropertyMapping(void)
{
}


FdoString* ArcSDEPropertyMapping::GetColumnName()
{
    return m_ColumnName;
}

void ArcSDEPropertyMapping::SetColumnName(FdoString* columnName)
{
    m_ColumnName = columnName;
}


LONG ArcSDEPropertyMapping::GetColumnType()
{
    return m_ColumnType;
}

void ArcSDEPropertyMapping::SetColumnType(LONG sde_type)
{
    m_ColumnType = sde_type;
}



void ArcSDEPropertyMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    //TODO
}

FdoXmlSaxHandler* ArcSDEPropertyMapping::XmlStartElement(
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

FdoBoolean ArcSDEPropertyMapping::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname
)
{
    //TODO
    return(false);
}


ArcSDEPropertyMapping* ArcSDEPropertyMapping::Create(void)
{
    return new ArcSDEPropertyMapping();
}


ArcSDEPropertyMappingCollection* ArcSDEPropertyMappingCollection::Create(ArcSDEClassMapping *parent)
{
    return new ArcSDEPropertyMappingCollection(parent);
}

