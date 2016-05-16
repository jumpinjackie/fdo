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
#include <c_KgMssOverPropertyDefinition.h>

c_KgMssOverPropertyDefinition* c_KgMssOverPropertyDefinition::Create()
{
    c_KgMssOverPropertyDefinition* propdef = new c_KgMssOverPropertyDefinition();
    return propdef;
}

c_KgMssOverPropertyDefinition::c_KgMssOverPropertyDefinition(void)
{
}

c_KgMssOverPropertyDefinition::~c_KgMssOverPropertyDefinition(void)
{
}

void c_KgMssOverPropertyDefinition::Dispose(void)
{
    delete this;
}

c_KgMssOverColumnDefinition* c_KgMssOverPropertyDefinition::GetColumn() const
{
    return FDO_SAFE_ADDREF(m_ColumnDefinition.p);
}

void c_KgMssOverPropertyDefinition::SetColumn(c_KgMssOverColumnDefinition * Definition)
{
    m_ColumnDefinition = FDO_SAFE_ADDREF(Definition);
}

void c_KgMssOverPropertyDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    BaseType::InitFromXml(pContext, attrs);
}

FdoXmlSaxHandler* c_KgMssOverPropertyDefinition::XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs)
{
	FdoXmlSaxHandler* ret = NULL;

	try
	{

    ret = BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
		if( ret == NULL ) 
		{
			if( FdoCommonOSUtil::wcsicmp(Name, c_KgMssOverXmlGlobals::g_KgOraColumnElement) == 0 ) 
			{
				FdoKgOraColumnDefinitionP newcolumn = c_KgMssOverColumnDefinition::Create();
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

}//end of c_KgMssOverPropertyDefinition::XmlStartElement

FdoBoolean c_KgMssOverPropertyDefinition::XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}

void c_KgMssOverPropertyDefinition::_writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags)
{

    Writer->WriteStartElement(c_KgMssOverXmlGlobals::g_KgOraPropertyElement);

    BaseType::_writeXml(Writer, Flags);

    if (m_ColumnDefinition != NULL) {
        m_ColumnDefinition->_writeXml(Writer, Flags);
    }

    Writer->WriteEndElement();
}//end of c_KgMssOverPropertyDefinition::_writeXml


