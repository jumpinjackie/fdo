// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include <Fdo/Schema/TopoGeometryPropertyDefinition.h>
#include "XmlContext.h"

FdoTopoGeometryPropertyDefinition::FdoTopoGeometryPropertyDefinition(
    FdoString * name, 
    FdoString * description, 
    FdoString * topologyName,
    bool        system
)
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

FdoTopoGeometryPropertyDefinition::~FdoTopoGeometryPropertyDefinition()
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

FdoString * FdoTopoGeometryPropertyDefinition::GetTopologyName()
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

void FdoTopoGeometryPropertyDefinition::SetTopologyName(
    FdoString * topologyName
)
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

FdoTopoGeometryPropertyDefinition * FdoTopoGeometryPropertyDefinition::GetDependsOnTopoGeometry()
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

void FdoTopoGeometryPropertyDefinition::SetDependsOnTopoGeometry(
    FdoTopoGeometryPropertyDefinition * dependsOnTopoGeometry
)
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

void FdoTopoGeometryPropertyDefinition::InitFromXml(const FdoString* classTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

void FdoTopoGeometryPropertyDefinition::InitFromXml(FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

FdoXmlSaxHandler* FdoTopoGeometryPropertyDefinition::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

FdoBoolean FdoTopoGeometryPropertyDefinition::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname
)
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

void FdoTopoGeometryPropertyDefinition::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

void    FdoTopoGeometryPropertyDefinition::_StartChanges()
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}
void    FdoTopoGeometryPropertyDefinition::_BeginChangeProcessing()
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}
void    FdoTopoGeometryPropertyDefinition::_AcceptChanges()
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}
void    FdoTopoGeometryPropertyDefinition::_RejectChanges()
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}
void    FdoTopoGeometryPropertyDefinition::_EndChangeProcessing()
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}

void FdoTopoGeometryPropertyDefinition::Dispose()
{
    delete this;
}
