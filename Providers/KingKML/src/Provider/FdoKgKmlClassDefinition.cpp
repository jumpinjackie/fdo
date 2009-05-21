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
#include <FdoKgKmlClassDefinition.h>

FdoKgKmlClassDefinition* FdoKgKmlClassDefinition::Create()
{
    FdoKgKmlClassDefinition* pObject = new FdoKgKmlClassDefinition();

    return pObject;
}

FdoKgKmlClassDefinition::FdoKgKmlClassDefinition(void)
{
  m_Properties = FdoKgKmlPropertyDefinitionCollection::Create(this);
  m_IsPointGeometry = false;
}

FdoKgKmlClassDefinition::~FdoKgKmlClassDefinition(void)
{
}

void FdoKgKmlClassDefinition::Dispose(void)
{
    delete this;
}

FdoKgKmlPropertyDefinitionCollection* FdoKgKmlClassDefinition::GetProperties() const
{
    return FDO_SAFE_ADDREF(m_Properties.p);
}


void FdoKgKmlClassDefinition::InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs)
{
	try
	{

    BaseType::InitFromXml(Context, Attrs);

		FdoXmlAttributeP att = Attrs->FindItem(FdoKgKmlXmlGlobals::g_KgOraFullTableNameAttribute);
    if (att != NULL) 
    {
        SetOracleFullTableName(att->GetValue());
    }
  }
	catch (FdoException* e)
	{
		Context->AddError(e);
		e->Release();
	}
}//end of FdoKgKmlClassDefinition::InitFromXml

FdoXmlSaxHandler* FdoKgKmlClassDefinition::XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs)
{
	FdoXmlSaxHandler* ret = NULL;

	try
	{
   
    ret = BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
		if (ret == NULL) 
		{
			if (FdoCommonOSUtil::wcsicmp(Name, FdoKgKmlXmlGlobals::g_KgOraPropertyElement) == 0) 
			{
				FdoKgOraPropertyDefinitionP new_prop = FdoKgKmlPropertyDefinition::Create();
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
}//end of FdoKgKmlClassDefinition::XmlStartElement

FdoBoolean FdoKgKmlClassDefinition::XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}//end of FdoKgKmlClassDefinition::XmlEndElement

void FdoKgKmlClassDefinition::_writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags)
{

	Writer->WriteStartElement(FdoKgKmlXmlGlobals::g_KgOraClassDefinitionElement);

	BaseType::_writeXml(Writer, Flags);	

  Writer->WriteAttribute(FdoKgKmlXmlGlobals::g_KgOraFullTableNameAttribute, GetOracleFullTableName());

  for (FdoInt32 i = 0; i <  m_Properties->GetCount(); i++)	
  {
		FdoKgOraPropertyDefinitionP prop_def = m_Properties->GetItem(i);
		prop_def->_writeXml(Writer, Flags);
	}

	Writer->WriteEndElement();
}//end of FdoKgKmlClassDefinition::_writeXml




FdoKgKmlPropertyDefinition* FdoKgKmlClassDefinition::FindByColumnName(FdoString* ColumnName)
{
    
  // Find property with column name ColumnName
  for (FdoInt32 i=0; i<m_Properties->GetCount(); i++)
  {
    FdoPtr<FdoKgKmlPropertyDefinition> prop_def = m_Properties->GetItem(i);
    FdoPtr<FdoKgKmlColumnDefinition> coldef = prop_def->GetColumn();
    if( wcscmp(coldef->GetName(), ColumnName) == 0 )
    {
      return FDO_SAFE_ADDREF( prop_def.p );        
    }
  }

  return NULL;
  
}//end of FdoKgKmlClassDefinition::FindByColumnName

