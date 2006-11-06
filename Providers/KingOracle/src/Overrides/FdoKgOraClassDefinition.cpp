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
#include <KingOracle/Override/FdoKgOraClassDefinition.h>

FdoKgOraClassDefinition* FdoKgOraClassDefinition::Create()
{
    FdoKgOraClassDefinition* pObject = new FdoKgOraClassDefinition();

    return pObject;
}

FdoKgOraClassDefinition::FdoKgOraClassDefinition(void)
{
    m_Properties = FdoKgOraPropertyDefinitionCollection::Create(this);

}

FdoKgOraClassDefinition::~FdoKgOraClassDefinition(void)
{
}

void FdoKgOraClassDefinition::Dispose(void)
{
    delete this;
}

FdoKgOraPropertyDefinitionCollection* FdoKgOraClassDefinition::GetProperties() const
{
    return GIS_SAFE_ADDREF(m_Properties.p);
}


void FdoKgOraClassDefinition::InitFromXml(GisXmlSaxContext* Context, GisXmlAttributeCollection* Attrs)
{
	try
	{

    BaseType::InitFromXml(Context, Attrs);

		GisXmlAttributeP att = Attrs->FindItem(FdoKgOraXmlGlobals::g_KgOraFullTableNameAttribute);
    if (att != NULL) 
    {
        SetOracleFullTableName(att->GetValue());
    }
  }
	catch (GisException* e)
	{
		Context->AddError(e);
		e->Release();
	}
}//end of FdoKgOraClassDefinition::InitFromXml

GisXmlSaxHandler* FdoKgOraClassDefinition::XmlStartElement(GisXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, GisXmlAttributeCollection* Attrs)
{
	GisXmlSaxHandler* ret = NULL;

	try
	{
   
    ret = BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
		if (ret == NULL) 
		{
			if (FdoCommonOSUtil::wcsicmp(Name, FdoKgOraXmlGlobals::g_KgOraPropertyElement) == 0) 
			{
				FdoKgOraPropertyDefinitionP new_prop = FdoKgOraPropertyDefinition::Create();
				new_prop->InitFromXml(Context, Attrs);
				m_Properties->Add(new_prop);
				ret = new_prop;
			}
		}
	}
	catch (GisException* e)
	{
		Context->AddError(e);
		e->Release();
	}

	return ret;
}//end of FdoKgOraClassDefinition::XmlStartElement

GisBoolean FdoKgOraClassDefinition::XmlEndElement(GisXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}//end of FdoKgOraClassDefinition::XmlEndElement

void FdoKgOraClassDefinition::_writeXml(GisXmlWriter* Writer, const FdoXmlFlags* Flags)
{

	Writer->WriteStartElement(FdoKgOraXmlGlobals::g_KgOraClassDefinitionElement);

	BaseType::_writeXml(Writer, Flags);	

  Writer->WriteAttribute(FdoKgOraXmlGlobals::g_KgOraFullTableNameAttribute, GetOracleFullTableName());

  for (GisInt32 i = 0; i <  m_Properties->GetCount(); i++)	
  {
		FdoKgOraPropertyDefinitionP prop_def = m_Properties->GetItem(i);
		prop_def->_writeXml(Writer, Flags);
	}

	Writer->WriteEndElement();
}//end of FdoKgOraClassDefinition::_writeXml




FdoKgOraPropertyDefinition* FdoKgOraClassDefinition::FindByColumnName(FdoString* ColumnName)
{
    
  // Find property with column name ColumnName
  for (GisInt32 i=0; i<m_Properties->GetCount(); i++)
  {
    GisPtr<FdoKgOraPropertyDefinition> prop_def = m_Properties->GetItem(i);
    GisPtr<FdoKgOraColumnDefinition> coldef = prop_def->GetColumn();
    if( wcscmp(coldef->GetName(), ColumnName) == 0 )
    {
      return GIS_SAFE_ADDREF( prop_def.p );        
    }
  }

  return NULL;
  
}//end of FdoKgOraClassDefinition::FindByColumnName

