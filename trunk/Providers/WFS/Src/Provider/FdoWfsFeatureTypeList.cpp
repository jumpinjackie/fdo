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
#include "FdoWfsFeatureTypeList.h"
#include "FdoWfsFeatureType.h"
#include "FdoWfsOperations.h"


FdoWfsFeatureTypeList::FdoWfsFeatureTypeList() : m_operations(0)
{
    m_featureTypes = FdoWfsFeatureTypeCollection::Create();
}

FdoWfsFeatureTypeList::~FdoWfsFeatureTypeList()
{
}

FdoWfsFeatureTypeList* FdoWfsFeatureTypeList::Create()
{
    return new FdoWfsFeatureTypeList();
}

FdoWfsFeatureTypeCollection* FdoWfsFeatureTypeList::GetFeatureTypes() 
{ 
    return FDO_SAFE_ADDREF(m_featureTypes.p); 
}


void FdoWfsFeatureTypeList::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
}

FdoXmlSaxHandler* FdoWfsFeatureTypeList::XmlStartElement(
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
        switch (myContext->StateFeatureTypeList())
        {
        case 0:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Operations) == 0)
                    myContext->SetStateFeatureTypeList(1);
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::FeatureType) == 0)
                {
                    FdoPtr<FdoWfsFeatureType> featureType = FdoWfsFeatureType::Create();
                    featureType->InitFromXml(context, atts);
                    m_featureTypes->Add(featureType);
                    pRet = featureType.p;
                }
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoWfsGlobals::FeatureTypeList));

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

    return pRet;
}

FdoBoolean FdoWfsFeatureTypeList::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT)));
		
        FdoWfsCapabilitiesSaxContext* myContext = static_cast<FdoWfsCapabilitiesSaxContext*>(context);
        switch (myContext->StateFeatureTypeList())
        {
        case 0:
            break;
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Operations) == 0)
                    myContext->SetStateFeatureTypeList(0);
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



