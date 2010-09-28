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
#include <OWS/FdoOwsGeographicBoundingBox.h>
#include <OWS/FdoOwsGlobals.h>

FdoOwsGeographicBoundingBox::FdoOwsGeographicBoundingBox(void) :
    mWestBoundLongitude(0),
    mEastBoundLongitude(0),
    mNorthBoundLatitude(0),
    mSouthBoundLatitude(0)
{
}

FdoOwsGeographicBoundingBox::~FdoOwsGeographicBoundingBox(void)
{
}

void FdoOwsGeographicBoundingBox::Dispose()
{
    delete this;
}

FdoOwsGeographicBoundingBox* FdoOwsGeographicBoundingBox::Create()
{
    return new FdoOwsGeographicBoundingBox();
}

void FdoOwsGeographicBoundingBox::InitFromXml(FdoXmlSaxContext* context, FdoXmlAttributeCollection* attrs)
{
    try {
        VALIDATE_ARGUMENT(attrs);
        VALIDATE_ARGUMENT(context);

        FdoXmlAttributeP att = attrs->FindItem(FdoOwsGlobals::CapabilitiesLatLonBoundingBoxWBL);
        if (att != NULL) {
            SetWestBoundLongitude(FdoCommonOSUtil::wtof(att->GetValue()));
        }

        att = attrs->FindItem(FdoOwsGlobals::CapabilitiesLatLonBoundingBoxEBL);
        if (att != NULL) {
            SetEastBoundLongitude(FdoCommonOSUtil::wtof(att->GetValue()));
        }

        att = attrs->FindItem(FdoOwsGlobals::CapabilitiesLatLonBoundingBoxNBL);
        if (att != NULL) {
            SetNorthBoundLatitude(FdoCommonOSUtil::wtof(att->GetValue()));
        }

        att = attrs->FindItem(FdoOwsGlobals::CapabilitiesLatLonBoundingBoxSBL);
        if (att != NULL) {
            SetSouthBoundLatitude(FdoCommonOSUtil::wtof(att->GetValue()));
        }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }
}

FdoXmlSaxHandler* FdoOwsGeographicBoundingBox::XmlStartElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
	FdoXmlSaxHandler* pRet = NULL;

	try
	{
        VALIDATE_ARGUMENT(name);
        VALIDATE_ARGUMENT(context);

        pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
		if (pRet == NULL) {
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::CapabilitiesExGeoBoundingBoxWBL) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::CapabilitiesExGeoBoundingBoxEBL) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::CapabilitiesExGeoBoundingBoxNBL) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::CapabilitiesExGeoBoundingBoxSBL) == 0) 
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

FdoBoolean FdoOwsGeographicBoundingBox::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    try 
    {
        VALIDATE_ARGUMENT(name);
        VALIDATE_ARGUMENT(context);

        if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::CapabilitiesExGeoBoundingBoxWBL) == 0) {
            SetWestBoundLongitude(FdoCommonOSUtil::wtof(mXmlContentHandler->GetString()));
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::CapabilitiesExGeoBoundingBoxEBL) == 0) {
            SetEastBoundLongitude(FdoCommonOSUtil::wtof(mXmlContentHandler->GetString()));
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::CapabilitiesExGeoBoundingBoxNBL) == 0) {
            SetNorthBoundLatitude(FdoCommonOSUtil::wtof(mXmlContentHandler->GetString()));
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::CapabilitiesExGeoBoundingBoxSBL) == 0) {
            SetSouthBoundLatitude(FdoCommonOSUtil::wtof(mXmlContentHandler->GetString()));
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

FdoDouble FdoOwsGeographicBoundingBox::GetWestBoundLongitude() const
{
    return mWestBoundLongitude;
}

void FdoOwsGeographicBoundingBox::SetWestBoundLongitude(FdoDouble value)
{
    mWestBoundLongitude = value;
}

FdoDouble FdoOwsGeographicBoundingBox::GetEastBoundLongitude() const
{
    return mEastBoundLongitude;
}

void FdoOwsGeographicBoundingBox::SetEastBoundLongitude(FdoDouble value)
{
    mEastBoundLongitude = value;
}

FdoDouble FdoOwsGeographicBoundingBox::GetNorthBoundLatitude() const
{
    return mNorthBoundLatitude;
}

void FdoOwsGeographicBoundingBox::SetNorthBoundLatitude(FdoDouble value)
{
    mNorthBoundLatitude = value;
}

FdoDouble FdoOwsGeographicBoundingBox::GetSouthBoundLatitude() const
{
    return mSouthBoundLatitude;
}

void FdoOwsGeographicBoundingBox::SetSouthBoundLatitude(FdoDouble value)
{
    mSouthBoundLatitude = value;
}

