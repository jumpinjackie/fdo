// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "CommonInternal.h"

FdoDictionaryElement* FdoCommonInternal::CreateDictionaryElement(FdoStringP& name, FdoStringP& value, int dummy)
{
    FdoDictionaryElement* elem = new FdoDictionaryElement();
    elem->mName = name;
    elem->mValue = value;

    return elem;
}

FdoXmlAttribute* FdoCommonInternal::CreateXmlAttribute(
    FdoStringP& name, 
    FdoStringP& value, 
    FdoStringP& localName,
    FdoStringP& uri,
    FdoStringP& prefix,
    FdoStringP& valueUri,
    FdoStringP& localValue,
    FdoStringP& valuePrefix
)
{
    FdoXmlAttribute* attr = new FdoXmlAttribute();
    attr->mName = name;
    attr->mValue = value;

    attr->mLocalName = localName;
    attr->mUri = uri;
    attr->mPrefix = prefix;
    attr->mValueUri = valueUri;
    attr->mLocalValue = (localValue != L"") ? localValue : value;
    attr->mValuePrefix = valuePrefix;

    return attr;
}
