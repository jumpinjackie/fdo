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

#ifndef FDOWMSSERVICEMETADATA_H
#define FDOWMSSERVICEMETADATA_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsServiceMetadata.h>
#include "FdoWmsCapabilities.h"

class FdoWmsServiceMetadata : public FdoOwsServiceMetadata
{
    typedef FdoOwsServiceMetadata BaseType;

protected:
    FdoWmsServiceMetadata();
    virtual ~FdoWmsServiceMetadata();
    virtual void Dispose();
    virtual FdoOwsCapabilities* OnCreateCapabilities();

public:
    static FdoWmsServiceMetadata* Create();
    virtual FdoOwsCapabilities* GetCapabilities() const;

    virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );

    virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);

private:
    FdoWmsCapabilitiesP m_capabilities;
};

typedef FdoPtr<FdoWmsServiceMetadata> FdoWmsServiceMetadataP;

#endif


