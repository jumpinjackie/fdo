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

#ifndef FDOWFSCAPABILITIESSAXCONTEXT_H
#define FDOWFSCAPABILITIESSAXCONTEXT_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsXmlSaxContext.h>

class FdoWfsCapabilitiesSaxContext : public FdoOwsXmlSaxContext
{
private:
    int m_stateFeatureTypeList;
    int m_stateFeatureType;
    int m_stateSpatial_Capabilities;
    int m_stateScalar_Capabilities;

    FdoPtr<FdoXmlCharDataHandler> m_featureTypeCharDataHandler;

public:
    int StateFeatureTypeList() const { return m_stateFeatureTypeList; }
    int StateFeatureType() const { return m_stateFeatureType; }
    int StateSpatial_Capabilities() const { return m_stateSpatial_Capabilities; }
    int StateScalar_Capabilities() const { return m_stateScalar_Capabilities; }
    void SetStateFeatureTypeList(int state) { m_stateFeatureTypeList = state; }
    void SetStateFeatureType(int state) { m_stateFeatureType = state; }
    void SetStateSpatial_Capabilities(int state) { m_stateSpatial_Capabilities = state; }
    void SetStateScalar_Capabilities(int state) { m_stateScalar_Capabilities = state; }

    FdoXmlCharDataHandler* FeatureTypeCharDataHandler() { return FDO_SAFE_ADDREF(m_featureTypeCharDataHandler.p); }
    void SetFeatureTypeCharDataHandler(FdoXmlCharDataHandler* handler) { m_featureTypeCharDataHandler = FDO_SAFE_ADDREF(handler); }
 
    FdoWfsCapabilitiesSaxContext(FdoXmlReader* reader) : FdoOwsXmlSaxContext(reader),
    m_stateFeatureTypeList(0),
    m_stateFeatureType(0),
    m_stateSpatial_Capabilities(0),
    m_stateScalar_Capabilities(0)
    {

    }
};


#endif


