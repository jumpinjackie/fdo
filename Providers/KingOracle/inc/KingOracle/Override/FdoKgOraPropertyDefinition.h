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
#ifndef FDOKGORAPROPERTYDEFINITION_H
#define FDOKGORAPROPERTYDEFINITION_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <KingOracle/Override/FdoKgOraColumnDefinition.h>

class FdoKgOraPropertyDefinition : public FdoPhysicalPropertyMapping
{
	typedef FdoPhysicalElementMapping BaseType;

public:
    FDOKGORA_API static FdoKgOraPropertyDefinition* Create();

public:
    FDOKGORA_API FdoKgOraColumnDefinition* GetColumn() const;
    FDOKGORA_API void SetColumn(FdoKgOraColumnDefinition * definition);

    FDOKGORA_API virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);
    FDOKGORA_API virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs);
    FDOKGORA_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName);
    FDOKGORA_API virtual void _writeXml( FdoXmlWriter* Writer, const FdoXmlFlags* Flags );

protected:
    FdoKgOraPropertyDefinition(void);
    virtual ~FdoKgOraPropertyDefinition(void);
    virtual void Dispose(void);

private:
    FdoKgOraColumnDefinitionP m_ColumnDefinition;
};

typedef FdoPtr<FdoKgOraPropertyDefinition> FdoKgOraPropertyDefinitionP;

#endif // FDOKGORAPROPERTYDEFINITION_H


