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

#ifndef FDOOWSSERVICEMETADATA_H
#define FDOOWSSERVICEMETADATA_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoOwsServiceIdentification;
class FdoOwsCapabilities;

class FdoOwsServiceMetadata :
    public FdoIDisposable,
    public virtual FdoXmlDeserializable
{
    typedef FdoXmlDeserializable BaseType;

private:
    FdoStringP m_version;
    FdoStringP m_updateSequence;
    FdoPtr<FdoOwsServiceIdentification> m_serviceId;
    FdoPtr<FdoOwsCapabilities> m_capabilities;

protected:
    FDOOWS_API FdoOwsServiceMetadata();
    FDOOWS_API virtual ~FdoOwsServiceMetadata();
    FDOOWS_API virtual void Dispose() { delete this; }

    FDOOWS_API virtual FdoOwsServiceIdentification* OnCreateServiceIdentification();
    FDOOWS_API virtual FdoOwsCapabilities* OnCreateCapabilities();

public:
    FDOOWS_API static FdoOwsServiceMetadata* Create();

    FDOOWS_API FdoString* GetVersion() const;
    FDOOWS_API FdoString* GetUpdateSequence() const;
    FDOOWS_API FdoOwsServiceIdentification* GetServiceIdentification() const;
    FDOOWS_API FdoOwsCapabilities* GetCapabilities() const;

    FDOOWS_API virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );
    FDOOWS_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);
    FDOOWS_API virtual FdoXmlSaxContext* GetSaxContext();
};

typedef FdoPtr<FdoOwsServiceMetadata> FdoOwsServiceMetadataP;


#endif


