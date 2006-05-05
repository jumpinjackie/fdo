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

#ifndef FDOOWSREQUESTMETADATA_H
#define FDOOWSREQUESTMETADATA_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoOwsRequestMetadata : 
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;

private:
    FdoStringP m_name;
    FdoPtr<FdoStringCollection> m_httpGetURLs;
    FdoPtr<FdoStringCollection> m_httpPostURLs;
    FdoPtr<FdoStringCollection> m_httpURLs;

protected:
    FdoOwsRequestMetadata();
    FdoOwsRequestMetadata(FdoString* name);
    virtual ~FdoOwsRequestMetadata();
    virtual void Dispose() { delete this; }

public:
    FDOOWS_API static FdoOwsRequestMetadata* Create(FdoString* name);

    FDOOWS_API FdoString* GetName();
    FDOOWS_API FdoStringCollection* GetHttpGetUrls();
    FDOOWS_API FdoStringCollection* GetHttpPostUrls();
    FDOOWS_API FdoBoolean CanSetName();

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

typedef FdoPtr<FdoOwsRequestMetadata> FdoOwsRequestMetadataP;

class FdoOwsRequestMetadataCollection : public FdoNamedCollection<FdoOwsRequestMetadata, FdoException>
{
protected:
    FdoOwsRequestMetadataCollection();
    virtual ~FdoOwsRequestMetadataCollection();
    virtual void Dispose() { delete this; }

public:
    FDOOWS_API static FdoOwsRequestMetadataCollection* Create();
};

typedef FdoPtr<FdoOwsRequestMetadataCollection> FdoOwsRequestMetadatasP;


#endif


