/*
 * Copyright (C) 2006  SL-King d.o.o
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
 */

#include "stdafx.h"
#include <KingOracle/Override/FdoKgOraPhysicalSchemaMapping.h>
#include "../Provider/KgOraProvider.h"

FdoKgOraPhysicalSchemaMapping* FdoKgOraPhysicalSchemaMapping::Create()
{
    FdoKgOraPhysicalSchemaMapping* pObject = new FdoKgOraPhysicalSchemaMapping();
    
    return pObject;
}

FdoKgOraPhysicalSchemaMapping::FdoKgOraPhysicalSchemaMapping(void)
{
    m_Classes = FdoKgOraClassCollection::Create(this);
    
}

FdoKgOraPhysicalSchemaMapping::~FdoKgOraPhysicalSchemaMapping(void)
{
}

void FdoKgOraPhysicalSchemaMapping::Dispose(void)
{
    delete this;
}

FdoString* FdoKgOraPhysicalSchemaMapping::GetProvider()
{
    return D_KGORA_PROVIDER_NAME;
}

FdoKgOraClassCollection* FdoKgOraPhysicalSchemaMapping::GetClasses() const
{
    return FDO_SAFE_ADDREF(m_Classes.p);
}

void FdoKgOraPhysicalSchemaMapping::InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs)
{
    BaseType::InitFromXml(Context, Attrs);
}

FdoXmlSaxHandler* FdoKgOraPhysicalSchemaMapping::XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs)
{
    FdoXmlSaxHandler* ret = NULL;

    try 
    {
        ret = BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
        if( ret == NULL ) 
        {
            if( FdoCommonOSUtil::wcsicmp(Name, FdoKgOraXmlGlobals::g_KgOraClassDefinitionElement) == 0 ) 
            {
                FdoKgOraClassDefinitionP newclass = FdoKgOraClassDefinition::Create();
                newclass->InitFromXml(Context, Attrs);
                m_Classes->Add(newclass);
                ret = newclass;
            }
        }
    }
    catch (FdoException* e) 
    {
        Context->AddError(e);
        e->Release();
    }

    return ret;
  
}//end of FdoKgOraPhysicalSchemaMapping::XmlStartElement

FdoBoolean FdoKgOraPhysicalSchemaMapping::XmlEndElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}

void FdoKgOraPhysicalSchemaMapping::_writeXml( FdoXmlWriter* Writer, const FdoXmlFlags* Flags )
{
	
	Writer->WriteStartElement(FdoKgOraXmlGlobals::g_KgOraSchemaMapping);

	BaseType::_writeXml(Writer, Flags);	

    Writer->WriteAttribute(FdoXml::mXmlnsPref, FdoKgOraXmlGlobals::g_KgOraXmlnsValue); 

	for (FdoInt32 i = 0; i <  m_Classes->GetCount(); i++)	
	{
		FdoKgOraClassDefinitionP classDef = m_Classes->GetItem(i);
		classDef->_writeXml(Writer, Flags);
	}

	Writer->WriteEndElement();
}//end of FdoKgOraPhysicalSchemaMapping::_writeXml



FdoKgOraClassDefinition* FdoKgOraPhysicalSchemaMapping::FindByClassName(FdoString *ClassName)
{

  for (FdoInt32 i=0; i<m_Classes->GetCount(); i++)
  {
    FdoPtr<FdoKgOraClassDefinition> classdef = m_Classes->GetItem(i);
    // TODO: do case-insensitive compare on WIN32 platform, case-sensitive on other platforms
    if( wcscmp(classdef->GetName(), ClassName) == 0 )
    {
      return  FDO_SAFE_ADDREF(classdef.p);  
      break;
    }
  }
  
  return NULL;
}//end of FdoKgOraPhysicalSchemaMapping::FindByClassName

