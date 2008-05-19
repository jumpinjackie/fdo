//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef FDOWMSXMLSTYLE_H
#define FDOWMSXMLSTYLE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWmsStyle : public FdoXmlSaxHandler, public FdoIDisposable
{
	typedef FdoXmlSaxHandler BaseType;

public:
    static FdoWmsStyle* Create(void);

public:
    FdoString* GetName() const;
    FdoBoolean CanSetName() const;
    void SetName(FdoString* name);

    FdoString* GetTitle() const;
    void SetTitle(FdoString* title);

    FdoString* GetAbstract() const;
    void SetAbstract(FdoString* abstract);

    void InitFromXml(FdoXmlSaxContext* context, FdoXmlAttributeCollection* attrs);

    virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );

    FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);
    
protected:
    FdoWmsStyle(void);
    virtual ~FdoWmsStyle(void);
    void Dispose();

private:
    FdoStringP mName;
    FdoStringP mTitle;
    FdoStringP mAbstract;
    FdoXmlCharDataHandlerP mXmlContentHandler;
};

typedef FdoPtr<FdoWmsStyle> FdoWmsStyleP;

#endif // FDOWMSXMLSTYLE_H


