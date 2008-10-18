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
#include "FdoWfsOgcSpatialCapabilities.h"

FdoWfsOgcSpatialCapabilities::FdoWfsOgcSpatialCapabilities() : m_spatialOperators(0)
{
}

FdoWfsOgcSpatialCapabilities::~FdoWfsOgcSpatialCapabilities()
{
}

FdoWfsOgcSpatialCapabilities* FdoWfsOgcSpatialCapabilities::Create()
{
    return new FdoWfsOgcSpatialCapabilities();
}

void FdoWfsOgcSpatialCapabilities::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
}

FdoXmlSaxHandler* FdoWfsOgcSpatialCapabilities::XmlStartElement(
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
		
        FdoWfsCapabilitiesSaxContext* myContext = static_cast<FdoWfsCapabilitiesSaxContext*>(context);
        switch (myContext->StateSpatial_Capabilities())
        {
        case 0:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Spatial_Operators) == 0)
                    myContext->SetStateSpatial_Capabilities(1);
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoWfsGlobals::Spatial_Capabilities));

                break;
            }
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Intersect) == 0)
                    m_spatialOperators |= SpatialOperators_Intersect;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::BBOX) == 0)
                    m_spatialOperators |= SpatialOperators_BBOX;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Contains) == 0)
                    m_spatialOperators |= SpatialOperators_Contains;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Crosses) == 0)
                    m_spatialOperators |= SpatialOperators_Crosses;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Disjoint) == 0)
                    m_spatialOperators |= SpatialOperators_Disjoint;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Equals) == 0)
                    m_spatialOperators |= SpatialOperators_Equals;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Overlaps) == 0)
                    m_spatialOperators |= SpatialOperators_Overlaps;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Touches) == 0)
                    m_spatialOperators |= SpatialOperators_Touches;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Within) == 0)
                    m_spatialOperators |= SpatialOperators_Within;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::DWithin) == 0)
                    m_spatialOperators |= SpatialOperators_DWithin;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Beyond) == 0)
                    m_spatialOperators |= SpatialOperators_Beyond;
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoWfsGlobals::Spatial_Operators));
                break;
            }
        default:
            throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR)));
        }

	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return pRet;
}

FdoBoolean FdoWfsOgcSpatialCapabilities::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT)));
		
        FdoWfsCapabilitiesSaxContext* myContext = static_cast<FdoWfsCapabilitiesSaxContext*>(context);
        switch (myContext->StateSpatial_Capabilities())
        {
        case 0:
            break;
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Spatial_Operators) == 0)
                    myContext->SetStateSpatial_Capabilities(0);
                break;
            }
        default:
            throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR)));
        }
	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return ret;
}





