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
#include <KingOracle/Override/FdoKgOraColumnDefinition.h>

FdoKgOraColumnDefinition* FdoKgOraColumnDefinition::Create()
{
    FdoKgOraColumnDefinition* pObject = new FdoKgOraColumnDefinition();

    return pObject;
}

FdoKgOraColumnDefinition::FdoKgOraColumnDefinition(void)
{
}

FdoKgOraColumnDefinition::~FdoKgOraColumnDefinition(void)
{
}

void FdoKgOraColumnDefinition::Dispose(void)
{
    delete this;
}

void FdoKgOraColumnDefinition::InitFromXml(GisXmlSaxContext* Context, GisXmlAttributeCollection* Attrs)
{
    BaseType::InitFromXml(Context, Attrs);
}

GisXmlSaxHandler* FdoKgOraColumnDefinition::XmlStartElement(GisXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, GisXmlAttributeCollection* Attrs)
{
    return BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
}

GisBoolean FdoKgOraColumnDefinition::XmlEndElement(GisXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}

void FdoKgOraColumnDefinition::_writeXml(GisXmlWriter* Writer, const FdoXmlFlags* Flags)
{
     Writer->WriteStartElement(FdoKgOraXmlGlobals::g_KgOraColumnElement);

    BaseType::_writeXml(Writer, Flags);

    Writer->WriteEndElement();
}//end of FdoKgOraColumnDefinition::_writeXml




