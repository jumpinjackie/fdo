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
#include "FdoWmsBoundingBox.h"
#include "FdoWmsXmlGlobals.h"

FdoWmsBoundingBox::FdoWmsBoundingBox(void) :
    mMinX(0),
    mMinY(0),
    mMaxX(0),
    mMaxY(0),
    mResX(0),
    mResY(0)
{
}

FdoWmsBoundingBox::~FdoWmsBoundingBox(void)
{
}

void FdoWmsBoundingBox::Dispose()
{
    delete this;
}

FdoWmsBoundingBox* FdoWmsBoundingBox::Create()
{
    return new FdoWmsBoundingBox();
}

void FdoWmsBoundingBox::InitFromXml(FdoXmlSaxContext* context, FdoXmlAttributeCollection* attrs)
{
    try {
        VALIDATE_ARGUMENT(attrs);
        VALIDATE_ARGUMENT(context);

        FdoXmlAttributeP att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesSRS);
        if (att != NULL) {
            SetCRS(att->GetValue());
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesCRS);
        if (att != NULL) {
            SetCRS(att->GetValue());
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesBoundingBoxMinX);
        if (att != NULL) {
            SetMinX(FdoCommonOSUtil::wtof(att->GetValue()));
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesBoundingBoxMinY);
        if (att != NULL) {
            SetMinY(FdoCommonOSUtil::wtof(att->GetValue()));
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesBoundingBoxMaxX);
        if (att != NULL) {
            SetMaxX(FdoCommonOSUtil::wtof(att->GetValue()));
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesBoundingBoxMaxY);
        if (att != NULL) {
            SetMaxY(FdoCommonOSUtil::wtof(att->GetValue()));
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesBoundingBoxResX);
        if (att != NULL) {
            SetResX(FdoCommonOSUtil::wtof(att->GetValue()));
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesBoundingBoxResY);
        if (att != NULL) {
            SetResY(FdoCommonOSUtil::wtof(att->GetValue()));
        }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }
}

FdoXmlSaxHandler* FdoWmsBoundingBox::XmlStartElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
    return BaseType::XmlStartElement(context, uri, name, qname, atts);
}

FdoBoolean FdoWmsBoundingBox::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}

FdoDouble FdoWmsBoundingBox::GetMinX() const
{
    return mMinX;
}

void FdoWmsBoundingBox::SetMinX(FdoDouble value)
{
    mMinX = value;
}

FdoDouble FdoWmsBoundingBox::GetMinY() const
{
    return mMinY;
}

void FdoWmsBoundingBox::SetMinY(FdoDouble value)
{
    mMinY = value;
}

FdoDouble FdoWmsBoundingBox::GetMaxX() const
{
    return mMaxX;
}

void FdoWmsBoundingBox::SetMaxX(FdoDouble value)
{
    mMaxX = value;
}

FdoDouble FdoWmsBoundingBox::GetMaxY() const
{
    return mMaxY;
}

void FdoWmsBoundingBox::SetMaxY(FdoDouble value)
{
    mMaxY = value;
}

FdoDouble FdoWmsBoundingBox::GetResX() const
{
    return mResX;
}

void FdoWmsBoundingBox::SetResX(FdoDouble value)
{
    mResX = value;
}

FdoDouble FdoWmsBoundingBox::GetResY() const
{
    return mResY;
}

void FdoWmsBoundingBox::SetResY(FdoDouble value)
{
    mResY = value;
}

FdoString* FdoWmsBoundingBox::GetCRS() const
{
    return mCRS;
}

void FdoWmsBoundingBox::SetCRS(FdoString* value)
{
    mCRS = value;
}
