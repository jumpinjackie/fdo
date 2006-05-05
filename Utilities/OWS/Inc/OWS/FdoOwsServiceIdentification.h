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

#ifndef FDOOWSSERVICEIDENTIFICATION_H
#define FDOOWSSERVICEIDENTIFICATION_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoOwsServiceIdentification :
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;

private:
    FdoStringP m_name;
    FdoStringP m_title;
    FdoStringP m_abstract;
    FdoPtr<FdoStringCollection> m_keywords;
    FdoStringP m_url;
    FdoStringP m_fees;
    FdoStringP m_accessConstrains;

protected:
    FdoOwsServiceIdentification();
    virtual ~FdoOwsServiceIdentification();
    virtual void Dispose() { delete this; }

public:
    FDOOWS_API static FdoOwsServiceIdentification* Create();

    FDOOWS_API FdoString* GetName() const;
    FDOOWS_API FdoString* GetTitle() const;
    FDOOWS_API FdoString* GetAbstract() const;
    FDOOWS_API FdoStringCollection* GetKeywords() const;
    FDOOWS_API FdoString* GetUrl() const;
    FDOOWS_API FdoString* GetFees() const;
    FDOOWS_API FdoString* GetAccessConstrains() const;

	FDOOWS_API virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);
    FDOOWS_API virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );
    FDOOWS_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);
};

typedef FdoPtr<FdoOwsServiceIdentification> FdoOwsServiceIdentificationP;

#endif


