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
#ifndef FDOKGKMLRAPROPERTYDEFINITION_H
#define FDOKGKMLRAPROPERTYDEFINITION_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <FdoKgKmlColumnDefinition.h>

class FdoKgKmlPropertyDefinition : public FdoPhysicalPropertyMapping
{
	typedef FdoPhysicalElementMapping BaseType;

public:
    FDOKGKML_API static FdoKgKmlPropertyDefinition* Create();

public:
    FDOKGKML_API FdoKgKmlColumnDefinition* GetColumn() const;
    FDOKGKML_API void SetColumn(FdoKgKmlColumnDefinition * definition);

    FDOKGKML_API virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);
    FDOKGKML_API virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs);
    FDOKGKML_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName);
    FDOKGKML_API virtual void _writeXml( FdoXmlWriter* Writer, const FdoXmlFlags* Flags );

protected:
    FdoKgKmlPropertyDefinition(void);
    virtual ~FdoKgKmlPropertyDefinition(void);
    virtual void Dispose(void);

private:
    FdoKgOraColumnDefinitionP m_ColumnDefinition;
};

typedef FdoPtr<FdoKgKmlPropertyDefinition> FdoKgOraPropertyDefinitionP;

#endif // FDOKGORAPROPERTYDEFINITION_H


