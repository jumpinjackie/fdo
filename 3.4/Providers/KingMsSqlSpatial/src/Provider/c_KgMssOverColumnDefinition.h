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
#ifndef c_KgMssOverColumnDefinition_h
#define c_KgMssOverColumnDefinition_h

#ifdef _WIN32
#pragma once
#endif //_WIN32

class c_KgMssOverColumnDefinition : public FdoPhysicalElementMapping
{
	typedef FdoPhysicalElementMapping BaseType;

public:
    FDOKGMSS_API static c_KgMssOverColumnDefinition* Create();

public:
    FDOKGMSS_API virtual void InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs);
    FDOKGMSS_API virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs);
    FDOKGMSS_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName);
    FDOKGMSS_API virtual void _writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags);

protected:
    c_KgMssOverColumnDefinition(void);
    virtual ~c_KgMssOverColumnDefinition(void);
    virtual void Dispose(void);
};

typedef FdoPtr<c_KgMssOverColumnDefinition> FdoKgOraColumnDefinitionP;

#endif // c_KgMssOverColumnDefinition_h


