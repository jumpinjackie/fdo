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
 */

#ifndef FDOCOMMONCONNSTRINGPARSER_H
#define FDOCOMMONCONNSTRINGPARSER_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

typedef std::pair<std::wstring,std::string>     StringMapElem;
typedef std::map <std::wstring, StringMapElem > StringMap;
typedef StringMap::const_iterator               StringMapIter;

const wchar_t CONNECTIONPROPERTY_DELIMITER[] = L";";
const wchar_t CONNECTIONPROPERTY_SEPARATOR[] = L"=";


class FdoCommonConnStringParser
{
private:
    wchar_t* mString;

    StringMap m_valueMap;


public:
    FdoCommonConnStringParser (FdoIConnectionPropertyDictionary* propDictionary, const wchar_t* connection_string);
    virtual ~FdoCommonConnStringParser (void);

    const char* GetPropertyValue(FdoString* propertyName);
    const wchar_t* GetPropertyValueW(FdoString* propertyName);
    bool IsPropertyValueSet(FdoString* propertyName);

protected:
    bool match (wchar_t* token, const wchar_t* keyword);
};

#endif // FDOCOMMONCONNSTRINGPARSER_H


