//
// Copyright (C) 2004-2010  Autodesk, Inc.
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

#ifndef FdoOwsOperationsMetadata_H
#define FdoOwsOperationsMetadata_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoOwsXLink : 
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;

private:
    FdoStringP  m_type;
    FdoStringP  m_href;

public:
    FDOOWS_API static FdoOwsXLink* Create(void)  { return new FdoOwsXLink(); }

protected:
    FDOOWS_API FdoOwsXLink(void) {}
    FDOOWS_API virtual ~FdoOwsXLink(void) {}
    FDOOWS_API void Dispose() { delete this; }

public:
    FDOOWS_API FdoString* GetType() const { return m_type; }
    FDOOWS_API FdoString* GetHref() const { return m_href; }

public:
    FDOOWS_API virtual void InitFromXml(
        FdoXmlSaxContext* context, 
        FdoXmlAttributeCollection* attrs);
    
    FDOOWS_API virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts);

    FDOOWS_API virtual FdoBoolean XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname);
};

typedef FdoPtr<FdoOwsXLink> FdoOwsXLinkP;


class FdoOwsHttp : 
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;

private:
    FdoOwsXLinkP  m_httpGetXlink;
	FdoOwsXLinkP  m_httpPostXlink;

public:
    FDOOWS_API static FdoOwsHttp* Create(void)  { return new FdoOwsHttp(); }

protected:
    FDOOWS_API FdoOwsHttp(void) {}
    FDOOWS_API virtual ~FdoOwsHttp(void) {}
    FDOOWS_API void Dispose() { delete this; }

public:
    FDOOWS_API FdoOwsXLink* GetHttpGetXLink() const { return FDO_SAFE_ADDREF(m_httpGetXlink.p); }
	FDOOWS_API FdoOwsXLink* GetHttpPostXLink() const { return FDO_SAFE_ADDREF(m_httpPostXlink.p); }

public:
    FDOOWS_API virtual void InitFromXml(
        FdoXmlSaxContext* context, 
        FdoXmlAttributeCollection* attrs);
    
    FDOOWS_API virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts);

    FDOOWS_API virtual FdoBoolean XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname);
};

typedef FdoPtr<FdoOwsHttp> FdoOwsHttpP;


class FdoOwsDcp : 
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;

private:
    FdoOwsHttpP m_http;

public:
    FDOOWS_API static FdoOwsDcp* Create(void)  { return new FdoOwsDcp(); }

protected:
    FDOOWS_API FdoOwsDcp(void) {}
    FDOOWS_API virtual ~FdoOwsDcp(void) {}
    FDOOWS_API void Dispose() { delete this; }

public:
    FDOOWS_API FdoOwsHttp* GetHttp() const  { return FDO_SAFE_ADDREF( m_http.p ); }

public:
    FDOOWS_API virtual void InitFromXml(
        FdoXmlSaxContext* context, 
        FdoXmlAttributeCollection* attrs);
    
    FDOOWS_API virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts);

    FDOOWS_API virtual FdoBoolean XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname);
};

typedef FdoPtr<FdoOwsDcp> FdoOwsDcpP;



class FdoOwsParameter : 
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;

private:
    FdoStringP  m_name;
    FdoStringsP m_values;
    FdoXmlCharDataHandlerP m_xmlContentHandler;

public:
    FDOOWS_API static FdoOwsParameter* Create(void)  { return new FdoOwsParameter(); }

protected:
    FDOOWS_API FdoOwsParameter(void) { m_values = FdoStringCollection::Create(); }
    FDOOWS_API virtual ~FdoOwsParameter(void) {}
    FDOOWS_API void Dispose() { delete this; }

public:
    FDOOWS_API FdoBoolean CanSetName() const { return true; }
    FDOOWS_API FdoString* GetName() const { return m_name; }
    FDOOWS_API void SetName( FdoString* value )   { m_name = value; }
    FDOOWS_API FdoStringCollection* GetValues() const  { return FDO_SAFE_ADDREF( m_values.p ); }

public:
    FDOOWS_API virtual void InitFromXml(
        FdoXmlSaxContext* context, 
        FdoXmlAttributeCollection* attrs);
    
    FDOOWS_API virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts);

    FDOOWS_API virtual FdoBoolean XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname);
};

typedef FdoPtr<FdoOwsParameter> FdoOwsParameterP;


class FdoOwsParameterCollection : public FdoNamedCollection<FdoOwsParameter, FdoException>
{
public:
    FDOOWS_API static FdoOwsParameterCollection* Create(void)  { return new FdoOwsParameterCollection(); }

protected:
    FDOOWS_API FdoOwsParameterCollection(void) {}
    FDOOWS_API virtual ~FdoOwsParameterCollection(void) {}
    FDOOWS_API void Dispose() { delete this; }
};

typedef FdoPtr<FdoOwsParameterCollection> FdoOwsParameterCollectionP;


class FdoOwsOperation :
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;

private:
    FdoStringP  m_name;
    FdoOwsDcpP  m_dcp;
    FdoOwsParameterCollectionP m_parameters;
    FdoXmlCharDataHandlerP m_xmlContentHandler;

protected:
    FDOOWS_API FdoOwsOperation() { m_parameters = FdoOwsParameterCollection::Create(); }
    FDOOWS_API virtual ~FdoOwsOperation() {}
    FDOOWS_API virtual void Dispose() { delete this; }

public:
    FDOOWS_API static FdoOwsOperation* Create() { return new FdoOwsOperation(); }

public:
    FDOOWS_API FdoBoolean CanSetName() const { return true; }
    FDOOWS_API FdoString* GetName() const { return m_name; }
    FDOOWS_API void SetName( FdoString* value )   { m_name = value; }
    FDOOWS_API FdoOwsDcp* GetDcp() const  { return FDO_SAFE_ADDREF( m_dcp.p ); }
    FDOOWS_API FdoOwsParameterCollection* GetParameters() const  { return FDO_SAFE_ADDREF( m_parameters.p ); }

public:
    FDOOWS_API virtual void InitFromXml(
        FdoXmlSaxContext* context, 
        FdoXmlAttributeCollection* attrs);
    
    FDOOWS_API virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts);

    FDOOWS_API virtual FdoBoolean XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname);
};

typedef FdoPtr<FdoOwsOperation> FdoOwsOperationP;


class FdoOwsOperationCollection : public FdoNamedCollection<FdoOwsOperation, FdoException>
{
public:
    FDOOWS_API static FdoOwsOperationCollection* Create(void)  { return new FdoOwsOperationCollection(); }

protected:
    FDOOWS_API FdoOwsOperationCollection(void) {}
    FDOOWS_API virtual ~FdoOwsOperationCollection(void) {}
    FDOOWS_API void Dispose() { delete this; }
};

typedef FdoPtr<FdoOwsOperationCollection> FdoOwsOperationCollectionP;


class FdoOwsOperationsMetadata :
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;

private:
    FdoOwsOperationCollectionP m_operations;

protected:
    FDOOWS_API FdoOwsOperationsMetadata() { m_operations = FdoOwsOperationCollection::Create(); }
    FDOOWS_API virtual ~FdoOwsOperationsMetadata() {}
    FDOOWS_API virtual void Dispose() { delete this; }

public:
    FDOOWS_API static FdoOwsOperationsMetadata* Create() { return new FdoOwsOperationsMetadata(); }

public:
    FDOOWS_API FdoOwsOperationCollection* GetOperations() const  { return FDO_SAFE_ADDREF( m_operations.p ); }

public:
    FDOOWS_API virtual void InitFromXml(
        FdoXmlSaxContext* context, 
        FdoXmlAttributeCollection* attrs);
    
    FDOOWS_API virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );

    FDOOWS_API virtual FdoBoolean XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname);
};

typedef FdoPtr<FdoOwsOperationsMetadata> FdoOwsOperationsMetadataP;

#endif // FdoOwsOperationsMetadata_H