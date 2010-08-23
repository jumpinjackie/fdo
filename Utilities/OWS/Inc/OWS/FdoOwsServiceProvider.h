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

#ifndef FDOOWSSERVICEPROVIDER_H
#define FDOOWSSERVICEPROVIDER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoOwsOperationsMetadata.h"

class FdoOwsContactInfo :
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;

private:
    FdoStringP m_phoneNumber;
    FdoStringP m_faxNumber;
    FdoStringP m_deliveryPoint;
    FdoStringP m_city;
    FdoStringP m_administrativeArea;
    FdoStringP m_postalCode;
    FdoStringP m_county;
    FdoStringP m_electronicMailAddress;
    FdoStringP m_hoursOfService;
    FdoStringP m_contactInstructions;
    FdoPtr<FdoOwsXLink>  m_onlineResource;
    FdoXmlCharDataHandlerP m_xmlContentHandler;

protected:
    FDOOWS_API FdoOwsContactInfo() {}
    FDOOWS_API virtual ~FdoOwsContactInfo() {}
    FDOOWS_API virtual void Dispose() { delete this; }

public:
    FDOOWS_API static FdoOwsContactInfo* Create() { return new FdoOwsContactInfo(); }

public:
    FDOOWS_API FdoString* GetPhoneNumber() const  { return m_phoneNumber; }
    FDOOWS_API FdoString* GetFaxNumber() const  { return m_faxNumber; }
    FDOOWS_API FdoString* GetDeliveryPoint() const  { return m_deliveryPoint; }
    FDOOWS_API FdoString* GetCity() const  { return m_city; }
    FDOOWS_API FdoString* GetAdministrativeArea() const  { return m_administrativeArea; }
    FDOOWS_API FdoString* GetPostalCode() const  { return m_postalCode; }
    FDOOWS_API FdoString* GetCounty() const  { return m_county; }
    FDOOWS_API FdoString* GetElectronicMailAddress() const  { return m_electronicMailAddress; }
    FDOOWS_API FdoOwsXLink* GetOnlineResource() const  { return FDO_SAFE_ADDREF( m_onlineResource.p ); }
    FDOOWS_API FdoString* GetHoursOfService() const  { return m_hoursOfService; }
    FDOOWS_API FdoString* GetContactInstructions() const  { return m_contactInstructions; }

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

typedef FdoPtr<FdoOwsContactInfo> FdoOwsContactInfoP;


class FdoOwsServiceContact :
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;

private:
    FdoStringP m_name;
    FdoStringP m_position;
    FdoOwsContactInfoP m_contactInfo;
    FdoXmlCharDataHandlerP m_xmlContentHandler;

protected:
    FDOOWS_API FdoOwsServiceContact() {}
    FDOOWS_API virtual ~FdoOwsServiceContact() {}
    FDOOWS_API virtual void Dispose() { delete this; }

public:
    FDOOWS_API static FdoOwsServiceContact* Create()  { return new FdoOwsServiceContact(); }

public:
    FDOOWS_API FdoString* GetName() const  { return m_name; }
    FDOOWS_API FdoString* GetPosition() const  { return m_position; }
    FDOOWS_API FdoOwsContactInfo* GetContactInfo() const  { return FDO_SAFE_ADDREF( m_contactInfo.p ); }

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

typedef FdoPtr<FdoOwsServiceContact> FdoOwsServiceContactP;


class FdoOwsServiceProvider :
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;

private:
    FdoStringP m_name;
    FdoOwsXLinkP m_site;
    FdoOwsServiceContactP m_serviceContact;
    FdoXmlCharDataHandlerP m_xmlContentHandler;

protected:
    FDOOWS_API FdoOwsServiceProvider() {}
    FDOOWS_API virtual ~FdoOwsServiceProvider() {}
    FDOOWS_API virtual void Dispose() { delete this; }

public:
    FDOOWS_API static FdoOwsServiceProvider* Create() { return new FdoOwsServiceProvider(); }

public:
    FDOOWS_API FdoString* GetName() const  { return m_name; }
    FDOOWS_API FdoOwsXLink* GetSite() const  { return FDO_SAFE_ADDREF( m_site.p ); }
    FDOOWS_API FdoOwsServiceContact* GetServiceContact() const  { return FDO_SAFE_ADDREF( m_serviceContact.p ); }

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

typedef FdoPtr<FdoOwsServiceProvider> FdoOwsServiceProviderP;


#endif // FdoOwsServiceProvider_H