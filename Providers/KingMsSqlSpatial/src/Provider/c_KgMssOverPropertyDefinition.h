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
#ifndef FDOKGORAPROPERTYDEFINITION_H
#define FDOKGORAPROPERTYDEFINITION_H

#ifdef _WIN32
#pragma once
#endif //_WIN32



class c_KgMssOverPropertyDefinition : public FdoPhysicalPropertyMapping
{
	typedef FdoPhysicalElementMapping BaseType;

public:
    FDOKGMSS_API static c_KgMssOverPropertyDefinition* Create();

public:
    FDOKGMSS_API c_KgMssOverColumnDefinition* GetColumn() const;
    FDOKGMSS_API void SetColumn(c_KgMssOverColumnDefinition * definition);

    FDOKGMSS_API virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);
    FDOKGMSS_API virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs);
    FDOKGMSS_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName);
    FDOKGMSS_API virtual void _writeXml( FdoXmlWriter* Writer, const FdoXmlFlags* Flags );

protected:
    c_KgMssOverPropertyDefinition(void);
    virtual ~c_KgMssOverPropertyDefinition(void);
    virtual void Dispose(void);

private:
    FdoKgOraColumnDefinitionP m_ColumnDefinition;
};



#endif // FDOKGORAPROPERTYDEFINITION_H


