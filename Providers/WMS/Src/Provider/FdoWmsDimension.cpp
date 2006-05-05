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
#include "FdoWmsDimension.h"
#include "FdoWmsXmlGlobals.h"

FdoWmsDimension::FdoWmsDimension(void) 
{
}

FdoWmsDimension::~FdoWmsDimension(void)
{
}

void FdoWmsDimension::Dispose()
{
    delete this;
}

FdoWmsDimension* FdoWmsDimension::Create(void)
{
    return new FdoWmsDimension();
}

void FdoWmsDimension::InitFromXml(FdoXmlSaxContext* context, FdoXmlAttributeCollection* attrs)
{
    try {
        VALIDATE_ARGUMENT(attrs);
        VALIDATE_ARGUMENT(context);

        FdoXmlAttributeP att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesDimensionName);
        if (att != NULL) {
            SetName(att->GetValue());
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesDimensionUnits);
        if (att != NULL) {
            SetUnits(att->GetValue());
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesDimensionUnitSymbol);
        if (att != NULL) {
            SetUnitSymbol(att->GetValue());
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesDimensionDefault);
        if (att != NULL) {
            SetDefault(att->GetValue());
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesDimensionMultipleValues);
        if (att != NULL) {
            FdoBoolean bMultipleValues = false;
            FdoStringP value = att->GetValue();
            if (wcscmp(value, FdoWmsXmlGlobals::WmsCapabilitiesTrueString) == 0) {
                bMultipleValues = true;
            }
            SetMultipleValues(bMultipleValues);
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesDimensionNearestValue);
        if (att != NULL) {
            FdoBoolean bNearestValue = false;
            FdoStringP value = att->GetValue();
            if (wcscmp(value, FdoWmsXmlGlobals::WmsCapabilitiesTrueString) == 0) {
                bNearestValue = true;
            }
            SetNearestValue(bNearestValue);
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesDimensionCurrent);
        if (att != NULL) {
            FdoBoolean bCurrent = false;
            FdoStringP value = att->GetValue();
            if (wcscmp(value, FdoWmsXmlGlobals::WmsCapabilitiesTrueString) == 0) {
                bCurrent = true;
            }
            SetCurrent(bCurrent);
        }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }
}

FdoXmlSaxHandler* FdoWmsDimension::XmlStartElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
    return BaseType::XmlStartElement(context, uri, name, qname, atts);
}

FdoBoolean FdoWmsDimension::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}

FdoBoolean FdoWmsDimension::CanSetName() const
{
    return true;
}

FdoString* FdoWmsDimension::GetName() const
{
    return mName;
}

void FdoWmsDimension::SetName(FdoString* value)
{
    mName = value;
}

FdoString* FdoWmsDimension::GetUnits() const
{
    return mUnits;
}

void FdoWmsDimension::SetUnits(FdoString* value)
{
    mUnits = value;
}

FdoString* FdoWmsDimension::GetUnitSymbol() const
{
    return mUnitSymbol;
}

void FdoWmsDimension::SetUnitSymbol(FdoString* value)
{
    mUnitSymbol = value;
}

FdoString* FdoWmsDimension::GetDefault() const
{
    return mDefault;
}

void FdoWmsDimension::SetDefault(FdoString* value)
{
    mDefault = value;
}

FdoBoolean FdoWmsDimension::GetMultipleValues() const
{
    return mMultipleValues;
}

void FdoWmsDimension::SetMultipleValues(FdoBoolean value)
{
    mMultipleValues = value;
}

FdoBoolean FdoWmsDimension::GetNearestValue() const
{
    return mNearestValue;
}

void FdoWmsDimension::SetNearestValue(FdoBoolean value)
{
    mNearestValue = value;
}

FdoBoolean FdoWmsDimension::GetCurrent() const
{
    return mCurrent;
}

void FdoWmsDimension::SetCurrent(FdoBoolean value)
{
    mCurrent = value;
}



