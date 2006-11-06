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
#include <KingOracle/Override/FdoKgOraPropertyDefinition.h>

FdoKgOraPropertyDefinition* FdoKgOraPropertyDefinition::Create()
{
    FdoKgOraPropertyDefinition* propdef = new FdoKgOraPropertyDefinition();
    return propdef;
}

FdoKgOraPropertyDefinition::FdoKgOraPropertyDefinition(void)
{
}

FdoKgOraPropertyDefinition::~FdoKgOraPropertyDefinition(void)
{
}

void FdoKgOraPropertyDefinition::Dispose(void)
{
    delete this;
}

FdoKgOraColumnDefinition* FdoKgOraPropertyDefinition::GetColumn() const
{
    return GIS_SAFE_ADDREF(m_ColumnDefinition.p);
}

void FdoKgOraPropertyDefinition::SetColumn(FdoKgOraColumnDefinition * Definition)
{
    m_ColumnDefinition = GIS_SAFE_ADDREF(Definition);
}

void FdoKgOraPropertyDefinition::InitFromXml(GisXmlSaxContext* pContext, GisXmlAttributeCollection* attrs)
{
    BaseType::InitFromXml(pContext, attrs);
}

GisXmlSaxHandler* FdoKgOraPropertyDefinition::XmlStartElement(GisXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, GisXmlAttributeCollection* Attrs)
{
	GisXmlSaxHandler* ret = NULL;

	try
	{

    ret = BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
		if( ret == NULL ) 
		{
			if( FdoCommonOSUtil::wcsicmp(Name, FdoKgOraXmlGlobals::g_KgOraColumnElement) == 0 ) 
			{
				FdoKgOraColumnDefinitionP newcolumn = FdoKgOraColumnDefinition::Create();
				newcolumn->InitFromXml(Context, Attrs);
				SetColumn(newcolumn);
				ret = newcolumn;
			}
		}
	}
	catch (GisException* e)
	{
		Context->AddError(e);
		e->Release();
	}

	return ret;

}//end of FdoKgOraPropertyDefinition::XmlStartElement

GisBoolean FdoKgOraPropertyDefinition::XmlEndElement(GisXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}

void FdoKgOraPropertyDefinition::_writeXml(GisXmlWriter* Writer, const FdoXmlFlags* Flags)
{

    Writer->WriteStartElement(FdoKgOraXmlGlobals::g_KgOraPropertyElement);

    BaseType::_writeXml(Writer, Flags);

    if (m_ColumnDefinition != NULL) {
        m_ColumnDefinition->_writeXml(Writer, Flags);
    }

    Writer->WriteEndElement();
}//end of FdoKgOraPropertyDefinition::_writeXml


