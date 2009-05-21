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
#ifndef FdoKgKmlPhysicalSchemaMapping_H
#define FdoKgKmlPhysicalSchemaMapping_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <FdoKgKmlClassCollection.h>

class FdoKgKmlPhysicalSchemaMapping : public FdoPhysicalSchemaMapping
{
    typedef FdoPhysicalSchemaMapping BaseType;

public:
    FDOKGKML_API static FdoKgKmlPhysicalSchemaMapping* Create();

    FDOKGKML_API virtual FdoString* GetProvider();
    FDOKGKML_API FdoKgKmlClassCollection* GetClasses() const; 

    FDOKGKML_API virtual void InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs);
    FDOKGKML_API virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs);
    FDOKGKML_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName);
    FDOKGKML_API virtual void _writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags);

    /// find using classname:
    FDOKGKML_API FdoKgKmlClassDefinition* FindByClassName(FdoString *ClassName);

protected:
    FdoKgKmlPhysicalSchemaMapping(void);
    virtual ~FdoKgKmlPhysicalSchemaMapping(void);
    virtual void Dispose(void);

private:
    FdoKgOraClassCollectionP m_Classes;
};

typedef FdoPtr<FdoKgKmlPhysicalSchemaMapping> FdoKgOraPhysicalSchemaMappingP;

#endif // FdoKgOraPhysicalSchemaMapping_H


