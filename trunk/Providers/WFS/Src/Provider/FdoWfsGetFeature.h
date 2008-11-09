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

#ifndef FDOWFSGETFEATURE_H
#define FDOWFSGETFEATURE_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsRequest.h>

class FdoWfsGetFeature : public FdoOwsRequest
{
private:
    FdoPtr<FdoStringCollection> m_propertiesToSelect;
    FdoStringP m_from;
    FdoPtr<FdoFilter> m_where;
    FdoStringP m_targetNamespace;
    FdoStringP m_srsName;
    bool m_encodeWithClassName;
    FdoStringP m_schemaName;

protected:
    FdoWfsGetFeature() {};
    FdoWfsGetFeature(FdoString* targetNamespace, FdoString* srsName,
                                FdoStringCollection* propertiesToSelect,
                                FdoString* from,
                                FdoFilter* where,
                                FdoString* schemaName);
    virtual ~FdoWfsGetFeature();
    virtual void Dispose() { delete this; }

public:
    static FdoWfsGetFeature* Create(FdoString* targetNamespace, FdoString* srsName,
                                FdoStringCollection* propertiesToSelect,
                                FdoString* from,
                                FdoFilter* where,
                                FdoString* schemaName);

    virtual FdoStringP EncodeKVP();
    virtual FdoStringP EncodeXml();

    virtual void EncodeWithClassName(bool bVal) {m_encodeWithClassName = bVal;};
    virtual void SetSchemaName(FdoStringP schemaName) {m_schemaName = schemaName;}
};

typedef FdoPtr<FdoWfsGetFeature> FdoWfsGetFeatureP;




#endif

