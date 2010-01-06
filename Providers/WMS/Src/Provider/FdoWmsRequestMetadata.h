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

#ifndef FDOWMSREQUESTMETADATA_H
#define FDOWMSREQUESTMETADATA_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsRequestMetadata.h>

class FdoWmsRequestMetadata : public FdoOwsRequestMetadata
{
    typedef FdoOwsRequestMetadata BaseType;
    
protected:
    FdoWmsRequestMetadata();
    FdoWmsRequestMetadata(FdoString* name);
    virtual ~FdoWmsRequestMetadata();
    virtual void Dispose() { delete this; }

public:
    FDOWMS_API static FdoOwsRequestMetadata* Create(FdoString* name);
    FDOWMS_API virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );
    FDOWMS_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);
    FDOWMS_API FdoStringCollection* GetFormats();

private:
    FdoXmlCharDataHandlerP                 mXmlContentHandler;    
    FdoStringsP                            mFormats;
};

typedef FdoPtr<FdoWmsRequestMetadata> FdoWmsRequestMetadataP;

#endif//FDOWMSREQUESTMETADATA_H
