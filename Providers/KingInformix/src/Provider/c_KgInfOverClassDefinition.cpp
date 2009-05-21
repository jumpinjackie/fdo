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
#include <c_KgInfOverClassDefinition.h>

c_KgInfOverClassDefinition* c_KgInfOverClassDefinition::Create()
{
    c_KgInfOverClassDefinition* pObject = new c_KgInfOverClassDefinition();

    return pObject;
}

c_KgInfOverClassDefinition::c_KgInfOverClassDefinition(void)
{
    m_Properties = c_KgInfOverPropertyDefinitionCollection::Create(this);

}

c_KgInfOverClassDefinition::~c_KgInfOverClassDefinition(void)
{
}

void c_KgInfOverClassDefinition::Dispose(void)
{
    delete this;
}

c_KgInfOverPropertyDefinitionCollection* c_KgInfOverClassDefinition::GetProperties() const
{
    return FDO_SAFE_ADDREF(m_Properties.p);
}


void c_KgInfOverClassDefinition::InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs)
{
	try
	{

    BaseType::InitFromXml(Context, Attrs);

		FdoXmlAttributeP att = Attrs->FindItem(c_KgInfOverXmlGlobals::g_KgOraFullTableNameAttribute);
    if (att != NULL) 
    {
        SetSqlQueryFullName(att->GetValue());
    }
  }
	catch (FdoException* e)
	{
		Context->AddError(e);
		e->Release();
	}
}//end of c_KgInfOverClassDefinition::InitFromXml

FdoXmlSaxHandler* c_KgInfOverClassDefinition::XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs)
{
	FdoXmlSaxHandler* ret = NULL;

	try
	{
   
    ret = BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
		if (ret == NULL) 
		{
			if (FdoCommonOSUtil::wcsicmp(Name, c_KgInfOverXmlGlobals::g_KgOraPropertyElement) == 0) 
			{
				FdoPtr<c_KgInfOverPropertyDefinition> new_prop = c_KgInfOverPropertyDefinition::Create();
				new_prop->InitFromXml(Context, Attrs);
				m_Properties->Add(new_prop);
				ret = new_prop;
			}
		}
	}
	catch (FdoException* e)
	{
		Context->AddError(e);
		e->Release();
	}

	return ret;
}//end of c_KgInfOverClassDefinition::XmlStartElement

FdoBoolean c_KgInfOverClassDefinition::XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}//end of c_KgInfOverClassDefinition::XmlEndElement

void c_KgInfOverClassDefinition::_writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags)
{

	Writer->WriteStartElement(c_KgInfOverXmlGlobals::g_KgOraClassDefinitionElement);

	BaseType::_writeXml(Writer, Flags);	

  Writer->WriteAttribute(c_KgInfOverXmlGlobals::g_KgOraFullTableNameAttribute, GetSqlQueryFullName());

  for (FdoInt32 i = 0; i <  m_Properties->GetCount(); i++)	
  {
		FdoPtr<c_KgInfOverPropertyDefinition> prop_def = m_Properties->GetItem(i);
		prop_def->_writeXml(Writer, Flags);
	}

	Writer->WriteEndElement();
}//end of c_KgInfOverClassDefinition::_writeXml




c_KgInfOverPropertyDefinition* c_KgInfOverClassDefinition::FindByColumnName(FdoString* ColumnName)
{
    
  // Find property with column name ColumnName
  for (FdoInt32 i=0; i<m_Properties->GetCount(); i++)
  {
    FdoPtr<c_KgInfOverPropertyDefinition> prop_def = m_Properties->GetItem(i);
    FdoPtr<c_KgInfOverColumnDefinition> coldef = prop_def->GetColumn();
    if( wcscmp(coldef->GetName(), ColumnName) == 0 )
    {
      return FDO_SAFE_ADDREF( prop_def.p );        
    }
  }

  return NULL;
  
}//end of c_KgInfOverClassDefinition::FindByColumnName

