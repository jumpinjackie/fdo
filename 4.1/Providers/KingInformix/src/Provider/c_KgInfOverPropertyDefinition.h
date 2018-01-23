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



class c_KgInfOverPropertyDefinition : public FdoPhysicalPropertyMapping
{
	typedef FdoPhysicalElementMapping BaseType;

public:
    FDOKGINF_API static c_KgInfOverPropertyDefinition* Create();

public:
    FDOKGINF_API c_KgInfOverColumnDefinition* GetColumn() const;
    FDOKGINF_API void SetColumn(c_KgInfOverColumnDefinition * definition);

    FDOKGINF_API virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);
    FDOKGINF_API virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs);
    FDOKGINF_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName);
    FDOKGINF_API virtual void _writeXml( FdoXmlWriter* Writer, const FdoXmlFlags* Flags );

protected:
    c_KgInfOverPropertyDefinition(void);
    virtual ~c_KgInfOverPropertyDefinition(void);
    virtual void Dispose(void);

private:
    FdoKgOraColumnDefinitionP m_ColumnDefinition;
};



#endif // FDOKGORAPROPERTYDEFINITION_H


