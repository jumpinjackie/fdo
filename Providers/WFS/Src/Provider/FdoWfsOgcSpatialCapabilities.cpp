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
                // WFS 1.0 and WFS 1.1 use different element name for Spatial Operator element.
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Spatial_Operators) == 0)
                    myContext->SetStateSpatial_Capabilities(1);
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::SpatialOperators) == 0)
                    myContext->SetStateSpatial_Capabilities(2);
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoWfsGlobals::Spatial_Capabilities));

                break;
            }
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Intersects) == 0)
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
        case 2:
            {
                bool bSuccess = false;
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::SpatialOperator) == 0)
                {
                    FdoPtr<FdoXmlAttribute> attr = atts->FindItem(FdoWfsGlobals::name);
                    if (attr != NULL)
                    {
                        FdoStringP value = attr->GetValue();
                        if (value == FdoWfsGlobals::Intersects)
                        {
                            m_spatialOperators |= SpatialOperators_Intersect;
                            bSuccess = true;
                        }
                        else if (value == FdoWfsGlobals::BBOX)
                        {
                            m_spatialOperators |= SpatialOperators_BBOX;
                            bSuccess = true;
                        }
                        else if (value == FdoWfsGlobals::Contains)
                        {
                            m_spatialOperators |= SpatialOperators_Contains;
                            bSuccess = true;
                        }
                        else if (value == FdoWfsGlobals::Crosses)
                        {
                            m_spatialOperators |= SpatialOperators_Crosses;
                            bSuccess = true;
                        }
                        else if (value == FdoWfsGlobals::Disjoint)
                        {
                            m_spatialOperators |= SpatialOperators_Disjoint;
                            bSuccess = true;
                        }
                        else if (value == FdoWfsGlobals::Equals)
                        {
                            m_spatialOperators |= SpatialOperators_Equals;
                            bSuccess = true;
                        }
                        else if (value == FdoWfsGlobals::Overlaps)
                        {
                            m_spatialOperators |= SpatialOperators_Overlaps;
                            bSuccess = true;
                        }
                        else if (value == FdoWfsGlobals::Touches)
                        {
                            m_spatialOperators |= SpatialOperators_Touches;
                            bSuccess = true;
                        }
                        else if (value == FdoWfsGlobals::Within)
                        {
                            m_spatialOperators |= SpatialOperators_Within;
                            bSuccess = true;
                        }
                        else if (value == FdoWfsGlobals::DWithin)
                        {
                            m_spatialOperators |= SpatialOperators_DWithin;
                            bSuccess = true;
                        }
                        else if (value == FdoWfsGlobals::Beyond)
                        {
                            m_spatialOperators |= SpatialOperators_Beyond;
                            bSuccess = true;
                        }
                    }
                }
                if (!bSuccess)
                    throw FdoException::Create(FdoException::NLSGetMessage(
                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                            name, FdoWfsGlobals::SpatialOperator));
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
        case 2:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::SpatialOperators) == 0)
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





