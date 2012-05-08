/*
 * Copyright (C) 2007  Haris Kurtagic
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
#include <c_KgInfOverPhysicalSchemaMapping.h>
#include "FdoKgInfProvider.h"

c_KgInfOverPhysicalSchemaMapping* c_KgInfOverPhysicalSchemaMapping::Create()
{
    c_KgInfOverPhysicalSchemaMapping* pObject = new c_KgInfOverPhysicalSchemaMapping();
    
    return pObject;
}

c_KgInfOverPhysicalSchemaMapping::c_KgInfOverPhysicalSchemaMapping(void)
{
    m_Classes = c_KgInfOverClassCollection::Create(this);
    
}

c_KgInfOverPhysicalSchemaMapping::~c_KgInfOverPhysicalSchemaMapping(void)
{
}

void c_KgInfOverPhysicalSchemaMapping::Dispose(void)
{
    delete this;
}

FdoString* c_KgInfOverPhysicalSchemaMapping::GetProvider()
{
    return D_KGINF_PROVIDER_NAME;
}

c_KgInfOverClassCollection* c_KgInfOverPhysicalSchemaMapping::GetClasses() const
{
    return FDO_SAFE_ADDREF(m_Classes.p);
}

void c_KgInfOverPhysicalSchemaMapping::InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs)
{
    BaseType::InitFromXml(Context, Attrs);
}

FdoXmlSaxHandler* c_KgInfOverPhysicalSchemaMapping::XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs)
{
    FdoXmlSaxHandler* ret = NULL;

  try 
  {
 
		
		ret = BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
		if( ret == NULL ) 
		{
      if( FdoCommonOSUtil::wcsicmp(Name, c_KgInfOverXmlGlobals::g_KgOraClassDefinitionElement) == 0 ) 
      {
				FdoPtr<c_KgInfOverClassDefinition> newclass = c_KgInfOverClassDefinition::Create();
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
  
}//end of c_KgInfOverPhysicalSchemaMapping::XmlStartElement

FdoBoolean c_KgInfOverPhysicalSchemaMapping::XmlEndElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}

void c_KgInfOverPhysicalSchemaMapping::_writeXml( FdoXmlWriter* Writer, const FdoXmlFlags* Flags )
{
	
	Writer->WriteStartElement(c_KgInfOverXmlGlobals::g_KgOraSchemaMapping);

	BaseType::_writeXml(Writer, Flags);	

  Writer->WriteAttribute(FdoXml::mXmlnsPref, c_KgInfOverXmlGlobals::g_KgOraXmlnsValue); 

	for (FdoInt32 i = 0; i <  m_Classes->GetCount(); i++)	
	{
		FdoPtr<c_KgInfOverClassDefinition> classDef = m_Classes->GetItem(i);
		classDef->_writeXml(Writer, Flags);
	}

	Writer->WriteEndElement();
}//end of c_KgInfOverPhysicalSchemaMapping::_writeXml



c_KgInfOverClassDefinition* c_KgInfOverPhysicalSchemaMapping::FindByClassName(FdoString *ClassName)
{

  for (FdoInt32 i=0; i<m_Classes->GetCount(); i++)
  {
    FdoPtr<c_KgInfOverClassDefinition> classdef = m_Classes->GetItem(i);
    // TODO: do case-insensitive compare on WIN32 platform, case-sensitive on other platforms
    if( wcscmp(classdef->GetName(), ClassName) == 0 )
    {
      return  FDO_SAFE_ADDREF(classdef.p);  
      break;
    }
  }
  
  return NULL;
}//end of c_KgInfOverPhysicalSchemaMapping::FindByClassName

