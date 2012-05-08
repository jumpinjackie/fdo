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
#ifndef _c_KgInfOverPhysicalSchemaMapping_h
#define _c_KgInfOverPhysicalSchemaMapping_h

#ifdef _WIN32
#pragma once
#endif //_WIN32



class c_KgInfOverPhysicalSchemaMapping : public FdoPhysicalSchemaMapping
{
    typedef FdoPhysicalSchemaMapping BaseType;

public:
    FDOKGINF_API static c_KgInfOverPhysicalSchemaMapping* Create();

    FDOKGINF_API virtual FdoString* GetProvider();
    FDOKGINF_API c_KgInfOverClassCollection* GetClasses() const; 

    FDOKGINF_API virtual void InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs);
    FDOKGINF_API virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs);
    FDOKGINF_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName);
    FDOKGINF_API virtual void _writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags);

    /// find using classname:
    FDOKGINF_API c_KgInfOverClassDefinition* FindByClassName(FdoString *ClassName);

protected:
    c_KgInfOverPhysicalSchemaMapping(void);
    virtual ~c_KgInfOverPhysicalSchemaMapping(void);
    virtual void Dispose(void);

private:
    c_KgInfOverClassCollectionP m_Classes;
};

typedef FdoPtr<c_KgInfOverPhysicalSchemaMapping> FdoKgOraPhysicalSchemaMappingP;

#endif // FdoKgOraPhysicalSchemaMapping_H


