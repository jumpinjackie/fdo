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


ArcSDEClassMapping::ArcSDEClassMapping(void)
{
    m_PropertyMappings = ArcSDEPropertyMappingCollection::Create(this);
    m_IndexMappings = ArcSDEIndexMappingCollection::Create(this);
    m_GeometryStorageType = ArcSDEGeometryStorageType_Spatial;
}

ArcSDEClassMapping::~ArcSDEClassMapping(void)
{
}


FdoString* ArcSDEClassMapping::GetTableName()
{
    return m_TableName;
}

void ArcSDEClassMapping::SetTableName(FdoString* tableName)
{
    m_TableName = tableName;
}


FdoString* ArcSDEClassMapping::GetDatabaseName()
{
    return m_DatabaseName;
}

void ArcSDEClassMapping::SetDatabaseName(FdoString* databaseName)
{
    m_DatabaseName = databaseName;
}

FdoString* ArcSDEClassMapping::GetOwnerName()
{
    return m_OwnerName;
}

void ArcSDEClassMapping::SetOwnerName(FdoString* ownerName)
{
    m_OwnerName = ownerName;
}

ArcSDEGeometryStorageType ArcSDEClassMapping::GetGeometryStorageType()
{
    return m_GeometryStorageType;
}

void ArcSDEClassMapping::SetGeometryStorageType(ArcSDEGeometryStorageType geometryStorageType)
{
    m_GeometryStorageType = geometryStorageType;
}

ArcSDEPropertyMappingCollection* ArcSDEClassMapping::GetProperties()
{
    return FDO_SAFE_ADDREF(m_PropertyMappings.p);
}

ArcSDEIndexMappingCollection* ArcSDEClassMapping::GetIndexes()
{
    return FDO_SAFE_ADDREF(m_IndexMappings.p);
}


void ArcSDEClassMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    //TODO
}

FdoXmlSaxHandler* ArcSDEClassMapping::XmlStartElement(
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

FdoBoolean ArcSDEClassMapping::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname
)
{
    //TODO
    return(false);
}


ArcSDEClassMapping* ArcSDEClassMapping::Create(void)
{
    return new ArcSDEClassMapping();
}



ArcSDEClassMappingCollection* ArcSDEClassMappingCollection::Create(ArcSDESchemaMapping* parent)
{
    return new ArcSDEClassMappingCollection(parent);
}

