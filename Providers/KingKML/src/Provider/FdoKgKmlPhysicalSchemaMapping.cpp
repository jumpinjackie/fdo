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
#include <FdoKgKmlPhysicalSchemaMapping.h>
#include "KgKmlProvider.h"

FdoKgKmlPhysicalSchemaMapping* FdoKgKmlPhysicalSchemaMapping::Create()
{
    FdoKgKmlPhysicalSchemaMapping* pObject = new FdoKgKmlPhysicalSchemaMapping();
    
    return pObject;
}

FdoKgKmlPhysicalSchemaMapping::FdoKgKmlPhysicalSchemaMapping(void)
{
    m_Classes = FdoKgKmlClassCollection::Create(this);
    
}

FdoKgKmlPhysicalSchemaMapping::~FdoKgKmlPhysicalSchemaMapping(void)
{
}

void FdoKgKmlPhysicalSchemaMapping::Dispose(void)
{
    delete this;
}

FdoString* FdoKgKmlPhysicalSchemaMapping::GetProvider()
{
    return D_KGORA_PROVIDER_NAME;
}

FdoKgKmlClassCollection* FdoKgKmlPhysicalSchemaMapping::GetClasses() const
{
    return FDO_SAFE_ADDREF(m_Classes.p);
}

void FdoKgKmlPhysicalSchemaMapping::InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs)
{
    BaseType::InitFromXml(Context, Attrs);
}

FdoXmlSaxHandler* FdoKgKmlPhysicalSchemaMapping::XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs)
{
    FdoXmlSaxHandler* ret = NULL;

  try 
  {
 
		
		ret = BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
		if( ret == NULL ) 
		{
      if( FdoCommonOSUtil::wcsicmp(Name, FdoKgKmlXmlGlobals::g_KgOraClassDefinitionElement) == 0 ) 
      {
				FdoKgKmlClassDefinitionP newclass = FdoKgKmlClassDefinition::Create();
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
  
}//end of FdoKgKmlPhysicalSchemaMapping::XmlStartElement

FdoBoolean FdoKgKmlPhysicalSchemaMapping::XmlEndElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}

void FdoKgKmlPhysicalSchemaMapping::_writeXml( FdoXmlWriter* Writer, const FdoXmlFlags* Flags )
{
	
	Writer->WriteStartElement(FdoKgKmlXmlGlobals::g_KgOraSchemaMapping);

	BaseType::_writeXml(Writer, Flags);	

  Writer->WriteAttribute(FdoXml::mXmlnsPref, FdoKgKmlXmlGlobals::g_KgOraXmlnsValue); 

	for (FdoInt32 i = 0; i <  m_Classes->GetCount(); i++)	
	{
		FdoKgKmlClassDefinitionP classDef = m_Classes->GetItem(i);
		classDef->_writeXml(Writer, Flags);
	}

	Writer->WriteEndElement();
}//end of FdoKgKmlPhysicalSchemaMapping::_writeXml



FdoKgKmlClassDefinition* FdoKgKmlPhysicalSchemaMapping::FindByClassName(FdoString *ClassName)
{

  for (FdoInt32 i=0; i<m_Classes->GetCount(); i++)
  {
    FdoPtr<FdoKgKmlClassDefinition> classdef = m_Classes->GetItem(i);
    // TODO: do case-insensitive compare on WIN32 platform, case-sensitive on other platforms
    if( wcscmp(classdef->GetName(), ClassName) == 0 )
    {
      return  FDO_SAFE_ADDREF(classdef.p);  
      break;
    }
  }
  
  return NULL;
}//end of FdoKgKmlPhysicalSchemaMapping::FindByClassName

