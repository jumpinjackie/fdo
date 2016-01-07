/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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
 *
 */

#ifndef FDOOWSEXCEPTIONREPORT_H
#define FDOOWSEXCEPTIONREPORT_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoOwsExceptionReport :
    public FdoIDisposable,
    public virtual FdoXmlDeserializable
{
    typedef FdoXmlDeserializable BaseType;
private:
    FdoStringP m_exceptionMessage;

protected:
    FdoOwsExceptionReport();
    virtual ~FdoOwsExceptionReport();
    virtual void Dispose() { delete this; }

public:
    static FdoOwsExceptionReport* Create();

    FdoString* GetExceptionMessage();

    virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );
    virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);
    virtual void XmlCharacters(FdoXmlSaxContext*, FdoString*);
    virtual FdoXmlSaxContext* GetSaxContext();

};


#endif

