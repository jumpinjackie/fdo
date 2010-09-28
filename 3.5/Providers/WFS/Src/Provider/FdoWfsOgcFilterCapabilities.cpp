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

#include "stdafx.h"
#include "FdoWfsOgcFilterCapabilities.h"
#include "FdoWfsOgcSpatialCapabilities.h"
#include "FdoWfsOgcScalarCapabilities.h"

FdoWfsOgcFilterCapabilities::FdoWfsOgcFilterCapabilities()
{
    m_spatialCapabilities = FdoWfsOgcSpatialCapabilities::Create();
    m_scalarCapabilities = FdoWfsOgcScalarCapabilities::Create();
}

FdoWfsOgcFilterCapabilities::~FdoWfsOgcFilterCapabilities()
{
}

FdoWfsOgcFilterCapabilities* FdoWfsOgcFilterCapabilities::Create()
{
    return new FdoWfsOgcFilterCapabilities();
}

void FdoWfsOgcFilterCapabilities::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
}

FdoWfsOgcSpatialCapabilities* FdoWfsOgcFilterCapabilities::GetSpatialCapabilities()
{ 
    return FDO_SAFE_ADDREF(m_spatialCapabilities.p); 
}

FdoWfsOgcScalarCapabilities* FdoWfsOgcFilterCapabilities::GetScalarCapabilities()
{ 
    return FDO_SAFE_ADDREF(m_scalarCapabilities.p); 
}


FdoXmlSaxHandler* FdoWfsOgcFilterCapabilities::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* pRet = NULL;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT)));
		
        if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Spatial_Capabilities) == 0)
            pRet = m_spatialCapabilities.p;
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Scalar_Capabilities) == 0)
            pRet = m_scalarCapabilities.p;
        else
            throw FdoException::Create(FdoException::NLSGetMessage(
                                    FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                    "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                    name, FdoWfsGlobals::Filter_Capabilities));


	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return pRet;}

FdoBoolean FdoWfsOgcFilterCapabilities::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    return false;
}



