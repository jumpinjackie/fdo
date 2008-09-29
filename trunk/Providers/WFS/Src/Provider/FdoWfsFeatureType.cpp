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
#include "FdoWfsFeatureType.h"
#include "FdoWfsOperations.h"

FdoWfsFeatureType::FdoWfsFeatureType() : m_operations(0)    
{
	m_SRSExtents = FdoOwsGeographicBoundingBoxCollection::Create ();
}

FdoWfsFeatureType::~FdoWfsFeatureType()
{
}

FdoWfsFeatureType* FdoWfsFeatureType::Create()
{
    return new FdoWfsFeatureType();
}

FdoOwsGeographicBoundingBoxCollection* FdoWfsFeatureType::GetSRSExtents ()
{
	return FDO_SAFE_ADDREF (m_SRSExtents.p);
}

void FdoWfsFeatureType::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
}

FdoXmlSaxHandler* FdoWfsFeatureType::XmlStartElement(
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
        switch (myContext->StateFeatureType())
        {
        case 0:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Name) == 0 ||
                    FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::SRS) == 0 ||
                    FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Title) == 0 ||
					FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Abstract) == 0 ||
					FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Keywords) == 0 ||
					FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::MetadataURL) == 0 )
                {
                    FdoPtr<FdoXmlCharDataHandler> charDataHandler = FdoXmlCharDataHandler::Create();
                    myContext->SetFeatureTypeCharDataHandler(charDataHandler);
                    pRet = charDataHandler.p;
                }
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::LatLongBoundingBox) == 0)
                {
					FdoPtr<FdoOwsGeographicBoundingBox> bbox = FdoOwsGeographicBoundingBox::Create ();
					m_SRSExtents->Add (bbox);
                    FdoPtr<FdoXmlAttribute> attr = atts->FindItem(FdoWfsGlobals::minx);
                    if (attr != NULL)
                    {
                        FdoStringP value = attr->GetValue();
                        bbox->SetWestBoundLongitude (atof((const char*)value));
                    }
                    attr = atts->FindItem(FdoWfsGlobals::miny);
                    if (attr != NULL)
                    {
                        FdoStringP value = attr->GetValue();
                        bbox->SetSouthBoundLatitude (atof((const char*)value));
                    }
                    attr = atts->FindItem(FdoWfsGlobals::maxx);
                    if (attr != NULL)
                    {
                        FdoStringP value = attr->GetValue();
                        bbox->SetEastBoundLongitude (atof((const char*)value));
                    }
                    attr = atts->FindItem(FdoWfsGlobals::maxy);
                    if (attr != NULL)
                    {
                        FdoStringP value = attr->GetValue();
                        bbox->SetNorthBoundLatitude (atof((const char*)value));
                    }
                }
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Operations) == 0)
                    myContext->SetStateFeatureType(1);
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoWfsGlobals::FeatureType));

                break;
            }
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Query) == 0)
                    m_operations |= FdoWfsOperations_Query;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Update) == 0)
                    m_operations |= FdoWfsOperations_Update;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Insert) == 0)
                    m_operations |= FdoWfsOperations_Insert;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Delete) == 0)
                    m_operations |= FdoWfsOperations_Delete;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Lock) == 0)
                    m_operations |= FdoWfsOperations_Lock;
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoWfsGlobals::Operations));
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

    return pRet;}

FdoBoolean FdoWfsFeatureType::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT)));
		
        FdoWfsCapabilitiesSaxContext* myContext = static_cast<FdoWfsCapabilitiesSaxContext*>(context);
        switch (myContext->StateFeatureType())
        {
        case 0:
            {
                FdoPtr<FdoXmlCharDataHandler> charDataHandler = myContext->FeatureTypeCharDataHandler();
                if (charDataHandler != NULL)
                {
                    if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Name) == 0)
                        m_name = charDataHandler->GetString();
                    else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::SRS) == 0)
                    {
                        m_srs = charDataHandler->GetString();
                        m_srs = m_srs.Upper();
                    }
                    else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Title) == 0)
                        m_title = charDataHandler->GetString();
					else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Abstract) == 0)
						m_abstract = charDataHandler->GetString();
					else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Keywords) == 0)
						m_keywords = charDataHandler->GetString();
					else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::MetadataURL) ==0)
					{
						// For now, we don't make use of MetadataURL tag. 
						// So we just ignore them.
					}

                    myContext->SetFeatureTypeCharDataHandler(NULL); 
                }
                break;
            }
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Operations) == 0)
                    myContext->SetStateFeatureType(0);
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

FdoWfsFeatureTypeCollection::FdoWfsFeatureTypeCollection()
{
}

FdoWfsFeatureTypeCollection::~FdoWfsFeatureTypeCollection()
{
}

FdoWfsFeatureTypeCollection* FdoWfsFeatureTypeCollection::Create()
{
    return new FdoWfsFeatureTypeCollection();
}


