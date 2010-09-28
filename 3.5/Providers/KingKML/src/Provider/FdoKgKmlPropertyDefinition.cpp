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
#include <FdoKgKmlPropertyDefinition.h>

FdoKgKmlPropertyDefinition* FdoKgKmlPropertyDefinition::Create()
{
    FdoKgKmlPropertyDefinition* propdef = new FdoKgKmlPropertyDefinition();
    return propdef;
}

FdoKgKmlPropertyDefinition::FdoKgKmlPropertyDefinition(void)
{
}

FdoKgKmlPropertyDefinition::~FdoKgKmlPropertyDefinition(void)
{
}

void FdoKgKmlPropertyDefinition::Dispose(void)
{
    delete this;
}

FdoKgKmlColumnDefinition* FdoKgKmlPropertyDefinition::GetColumn() const
{
    return FDO_SAFE_ADDREF(m_ColumnDefinition.p);
}

void FdoKgKmlPropertyDefinition::SetColumn(FdoKgKmlColumnDefinition * Definition)
{
    m_ColumnDefinition = FDO_SAFE_ADDREF(Definition);
}

void FdoKgKmlPropertyDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    BaseType::InitFromXml(pContext, attrs);
}

FdoXmlSaxHandler* FdoKgKmlPropertyDefinition::XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs)
{
	FdoXmlSaxHandler* ret = NULL;

	try
	{

    ret = BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
		if( ret == NULL ) 
		{
			if( FdoCommonOSUtil::wcsicmp(Name, FdoKgKmlXmlGlobals::g_KgOraColumnElement) == 0 ) 
			{
				FdoKgOraColumnDefinitionP newcolumn = FdoKgKmlColumnDefinition::Create();
				newcolumn->InitFromXml(Context, Attrs);
				SetColumn(newcolumn);
				ret = newcolumn;
			}
		}
	}
	catch (FdoException* e)
	{
		Context->AddError(e);
		e->Release();
	}

	return ret;

}//end of FdoKgKmlPropertyDefinition::XmlStartElement

FdoBoolean FdoKgKmlPropertyDefinition::XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}

void FdoKgKmlPropertyDefinition::_writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags)
{

    Writer->WriteStartElement(FdoKgKmlXmlGlobals::g_KgOraPropertyElement);

    BaseType::_writeXml(Writer, Flags);

    if (m_ColumnDefinition != NULL) {
        m_ColumnDefinition->_writeXml(Writer, Flags);
    }

    Writer->WriteEndElement();
}//end of FdoKgKmlPropertyDefinition::_writeXml


