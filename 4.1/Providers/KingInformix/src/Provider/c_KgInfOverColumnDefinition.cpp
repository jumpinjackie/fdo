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
#include <c_KgInfOverColumnDefinition.h>

c_KgInfOverColumnDefinition* c_KgInfOverColumnDefinition::Create()
{
    c_KgInfOverColumnDefinition* pObject = new c_KgInfOverColumnDefinition();

    return pObject;
}

c_KgInfOverColumnDefinition::c_KgInfOverColumnDefinition(void)
{
}

c_KgInfOverColumnDefinition::~c_KgInfOverColumnDefinition(void)
{
}

void c_KgInfOverColumnDefinition::Dispose(void)
{
    delete this;
}

void c_KgInfOverColumnDefinition::InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs)
{
    BaseType::InitFromXml(Context, Attrs);
}

FdoXmlSaxHandler* c_KgInfOverColumnDefinition::XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs)
{
    return BaseType::XmlStartElement(Context, Uri, Name, QName, Attrs);
}

FdoBoolean c_KgInfOverColumnDefinition::XmlEndElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName)
{
	return BaseType::XmlEndElement(Context, Uri, Name, QName);
}

void c_KgInfOverColumnDefinition::_writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags)
{
     Writer->WriteStartElement(c_KgInfOverXmlGlobals::g_KgOraColumnElement);

    BaseType::_writeXml(Writer, Flags);

    Writer->WriteEndElement();
}//end of c_KgInfOverColumnDefinition::_writeXml




