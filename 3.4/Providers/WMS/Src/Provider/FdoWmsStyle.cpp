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

#include <stdafx.h>
#include "FdoWmsStyle.h"
#include "FdoWmsStyleCollection.h"
#include "FdoWmsBoundingBox.h"
#include "FdoWmsBoundingBoxCollection.h"
#include "FdoWmsXmlGlobals.h"

FdoWmsStyle::FdoWmsStyle(void) 
{
}

FdoWmsStyle::~FdoWmsStyle(void)
{
}

void FdoWmsStyle::Dispose()
{
    delete this;
}

FdoWmsStyle* FdoWmsStyle::Create(void)
{
    return new FdoWmsStyle();
}

void FdoWmsStyle::InitFromXml(FdoXmlSaxContext* context, FdoXmlAttributeCollection* attrs)
{
}

FdoXmlSaxHandler* FdoWmsStyle::XmlStartElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
	FdoXmlSaxHandler* pRet = NULL;

	try
	{
        VALIDATE_ARGUMENT(name);
        VALIDATE_ARGUMENT(context);

        pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
		if (pRet == NULL) {
			if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesStyleName) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesStyleTitle) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesStyleAbstract) == 0) 
            {
                mXmlContentHandler = FdoXmlCharDataHandler::Create();
                pRet = mXmlContentHandler;
			}
		}
	}
	catch (FdoException* e)
	{
		context->AddError(e);
		e->Release();
	}

	return pRet;
}

FdoBoolean FdoWmsStyle::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    try 
    {
        VALIDATE_ARGUMENT(name);
        VALIDATE_ARGUMENT(context);

        if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesStyleName) == 0) {
            SetName(mXmlContentHandler->GetString());
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesStyleTitle) == 0) {
            SetTitle(mXmlContentHandler->GetString());
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesStyleAbstract) == 0) {
            SetAbstract(mXmlContentHandler->GetString());
        }

        FDO_SAFE_RELEASE(mXmlContentHandler.p);
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return BaseType::XmlEndElement(context, uri, name, qname);
}

FdoBoolean FdoWmsStyle::CanSetName() const
{
    return true;
}

FdoString* FdoWmsStyle::GetName() const
{
    return mName;
}

void FdoWmsStyle::SetName(FdoString* name)
{
    mName = name;
}

FdoString* FdoWmsStyle::GetTitle() const
{
    return mTitle;
}

void FdoWmsStyle::SetTitle(FdoString* title)
{
    mTitle = title;
}

FdoString* FdoWmsStyle::GetAbstract() const
{
    return mAbstract;
}

void FdoWmsStyle::SetAbstract(FdoString* abstract)
{
    mAbstract = abstract;
}

