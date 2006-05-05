/*
 * 
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
#include "FdoCommonConnStringParser.h"
#include "FdoCommonOSUtil.h"
#include "FdoCommonStringUtil.h"

#include <malloc.h>
#include <string.h>


bool FdoCommonConnStringParser::match (wchar_t* token, const wchar_t* keyword)
{
    wchar_t* start;
    bool bFoundMatch = false;

    if (0 == FdoCommonOSUtil::wcsnicmp (token, keyword, wcslen (keyword)))
    {
        token += wcslen (keyword);
        if (NULL != (start = wcschr (token, CONNECTIONPROPERTY_SEPARATOR[0])))
        {
            start += 1;

            char* mbValue;
            wide_to_multibyte(mbValue, start);

            m_valueMap[keyword] = StringMapElem(start, mbValue);

            bFoundMatch = true;
        }
    }

    return (bFoundMatch);
}

FdoCommonConnStringParser::FdoCommonConnStringParser (FdoIConnectionPropertyDictionary* propDictionary, const wchar_t* string) :
    mString (NULL)
{
    int         size;
    wchar_t     *rover;
    wchar_t     *token;

    size = (int)((1 + wcslen (string)) * sizeof (wchar_t));
    mString = new wchar_t [size];
    wcscpy (mString, string);
    rover = mString;

    wchar_t     *holder;
    while (NULL != (token = FdoCommonOSUtil::wcstok (rover, CONNECTIONPROPERTY_DELIMITER, &holder)))
    {
        FdoInt32 propCount=0;
        FdoString** propNames = propDictionary->GetPropertyNames(propCount);
        bool bFoundMatch = false;
        for (FdoInt32 i=0; i<propCount && !bFoundMatch; i++)
            bFoundMatch = match (token, propNames[i]);

        rover = NULL;
    }
}

FdoCommonConnStringParser::~FdoCommonConnStringParser (void)
{
    if (NULL != mString)
        delete [] mString;
}


const char* FdoCommonConnStringParser::GetPropertyValue(FdoString* propertyName)
{
    StringMapIter iter = m_valueMap.find(propertyName);
    if (iter == m_valueMap.end())
        return NULL;
    else
        return iter->second.second.c_str();
}

const wchar_t* FdoCommonConnStringParser::GetPropertyValueW(FdoString* propertyName)
{
    StringMapIter iter = m_valueMap.find(propertyName);
    if (iter == m_valueMap.end())
        return NULL;
    else
        return iter->second.first.c_str();
}

bool FdoCommonConnStringParser::IsPropertyValueSet(FdoString* propertyName)
{
    StringMapIter iter = m_valueMap.find(propertyName);
    return (iter != m_valueMap.end());
}

