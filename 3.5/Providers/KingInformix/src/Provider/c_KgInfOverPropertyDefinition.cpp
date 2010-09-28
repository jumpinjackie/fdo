/*
 * Copyright (C) 2006  Haris Kurtagic
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
#include <c_KgInfOverPropertyDefinition.h>

c_KgInfOverPropertyDefinition* c_KgInfOverPropertyDefinition::Create()
{
    c_KgInfOverPropertyDefinition* propdef = new c_KgInfOverPropertyDefinition();
    return propdef;
}

c_KgInfOverPropertyDefinition::c_KgInfOverPropertyDefinition(void)
{
}

c_KgInfOverPropertyDefinition::~c_KgInfOverPropertyDefinition(void)
{
}

void c_KgInfOverPropertyDefinition::Dispose(void)
{
    delete this;
}

c_KgInfOverColumnDefinition* c_KgInfOverPropertyDefinition::GetColumn() const
{
    return FDO_SAFE_ADDREF(m_ColumnDefinition.p);
}

void c_KgInfOverPropertyDefinition::SetColumn(c_KgInfOverColumnDefinition * Definition)
{
    m_ColumnDefinition = FDO_SAFE_ADDREF(Definition);
}

void c_KgInfOverPropertyDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    BaseType::InitFromXml(pContext, attrs);
}

FdoXmlSaxHandler* c_KgInfOverPropertyDefinition::XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs)
{
	FdoXmlSaxHandler* ret = NULL;

	try
	{

    ret = BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
		if( ret == NULL ) 
		{
			if( FdoCommonOSUtil::wcsicmp(Name, c_KgInfOverXmlGlobals::g_KgOraColumnElement) == 0 ) 
			{
				FdoKgOraColumnDefinitionP newcolumn = c_KgInfOverColumnDefinition::Create();
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

}//end of c_KgInfOverPropertyDefinition::XmlStartElement

FdoBoolean c_KgInfOverPropertyDefinition::XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}

void c_KgInfOverPropertyDefinition::_writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags)
{

    Writer->WriteStartElement(c_KgInfOverXmlGlobals::g_KgOraPropertyElement);

    BaseType::_writeXml(Writer, Flags);

    if (m_ColumnDefinition != NULL) {
        m_ColumnDefinition->_writeXml(Writer, Flags);
    }

    Writer->WriteEndElement();
}//end of c_KgInfOverPropertyDefinition::_writeXml


