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
  m_IsPointGeometry = false;
  
  
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
    return FDO_SAFE_ADDREF(m_Properties.p);
}


void FdoKgOraClassDefinition::InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs)
{
	try
	{

    BaseType::InitFromXml(Context, Attrs);

		FdoXmlAttributeP att = Attrs->FindItem(FdoKgOraXmlGlobals::g_KgOraFullTableNameAttribute);
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
}//end of FdoKgOraClassDefinition::InitFromXml

FdoXmlSaxHandler* FdoKgOraClassDefinition::XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs)
{
	FdoXmlSaxHandler* ret = NULL;

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
	catch (FdoException* e)
	{
		Context->AddError(e);
		e->Release();
	}

	return ret;
}//end of FdoKgOraClassDefinition::XmlStartElement

FdoBoolean FdoKgOraClassDefinition::XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}//end of FdoKgOraClassDefinition::XmlEndElement

void FdoKgOraClassDefinition::_writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags)
{

	Writer->WriteStartElement(FdoKgOraXmlGlobals::g_KgOraClassDefinitionElement);

	BaseType::_writeXml(Writer, Flags);	

  Writer->WriteAttribute(FdoKgOraXmlGlobals::g_KgOraFullTableNameAttribute, GetOracleFullTableName());

  for (FdoInt32 i = 0; i <  m_Properties->GetCount(); i++)	
  {
		FdoKgOraPropertyDefinitionP prop_def = m_Properties->GetItem(i);
		prop_def->_writeXml(Writer, Flags);
	}

	Writer->WriteEndElement();
}//end of FdoKgOraClassDefinition::_writeXml




FdoKgOraPropertyDefinition* FdoKgOraClassDefinition::FindByColumnName(FdoString* ColumnName)
{
    
  // Find property with column name ColumnName
  for (FdoInt32 i=0; i<m_Properties->GetCount(); i++)
  {
    FdoPtr<FdoKgOraPropertyDefinition> prop_def = m_Properties->GetItem(i);
    FdoPtr<FdoKgOraColumnDefinition> coldef = prop_def->GetColumn();
    if( wcscmp(coldef->GetName(), ColumnName) == 0 )
    {
      return FDO_SAFE_ADDREF( prop_def.p );        
    }
  }

  return NULL;
  
}//end of FdoKgOraClassDefinition::FindByColumnName

FDOKGORA_API void FdoKgOraClassDefinition::SetSdoRootMBR( const wchar_t* Mbr )
{
  m_Sdo_Root_MBR = Mbr ? Mbr : L"";
}

