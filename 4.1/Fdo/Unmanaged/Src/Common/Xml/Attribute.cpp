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
#include <FdoCommon.h>

FdoXmlAttribute* FdoXmlAttribute::Create(
    FdoString* name, 
    FdoString* value, 
    FdoString* localName,
    FdoString* uri,
    FdoString* prefix,
    FdoString* valueUri,
    FdoString* localValue,
    FdoString* valuePrefix
)
{
    return( new FdoXmlAttribute(name, value, localName, uri, prefix, valueUri, localValue, valuePrefix) );
}

FdoXmlAttribute::FdoXmlAttribute(
    FdoString* name, 
    FdoString* value, 
    FdoString* localName,
    FdoString* uri,
    FdoString* prefix,
    FdoString* valueUri,
    FdoString* localValue,
    FdoString* valuePrefix
)
    : FdoDictionaryElement(name, value )
{
    mLocalName = localName;
    mUri = uri;
    mPrefix = prefix;
    mValueUri = valueUri;
    mLocalValue = localValue ? localValue : value;
    mValuePrefix = valuePrefix;
}

