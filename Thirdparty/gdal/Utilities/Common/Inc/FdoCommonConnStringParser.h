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
#ifndef _MSC_VER
    #define _strdup strdup
#endif

#include "FdoCommonStringUtil.h"

const wchar_t CONNECTIONPROPERTY_DELIMITER[] = L";";
const wchar_t CONNECTIONPROPERTY_SEPARATOR[] = L"=";

class FdoCommonConnPropDictionary;

class FdoCommonConnStringParser
{
    class ParsStringMapElem
    {
        FdoStringP m_name;
        FdoStringP m_value;
        char* m_mbVal;
    public:
        ParsStringMapElem(FdoString* name, FdoString* value)
        {
            m_name = name;
            m_value = (value == NULL) ? L"" : value;
            m_mbVal = NULL;
        }
        ~ParsStringMapElem()
        {
            free(m_mbVal);
        }
        FdoString* GetName(){ return m_name; }
        FdoString* GetValue(){ return m_value; }
        void SetValue(FdoString* value)
        {
            m_value = value;
            free(m_mbVal);
            m_mbVal = NULL;
        }
        char* GetMbValue()
        {
            if (m_mbVal == NULL && m_value.GetLength() != 0)
            {
                char* mbValue;
                wide_to_multibyte(mbValue, (FdoString*)m_value);
                m_mbVal = _strdup(mbValue);
            }
            return m_mbVal;
        }
    };
    class ParsStringMapElems
    {
        ParsStringMapElem** m_elements;
        size_t m_count;
        size_t m_capacity;
    private:
        void resize()
        {
            size_t old_capacity = m_capacity;
            m_capacity = (m_capacity == 0) ? 8 : (size_t)(1.4 * m_capacity);
            ParsStringMapElem** newArray = new ParsStringMapElem*[m_capacity];
            for (size_t i = 0; i < old_capacity; i++)
            {
                newArray[i] = m_elements[i];
            }
            delete[] m_elements;
            m_elements = newArray;
        }
    public:
        ParsStringMapElems()
        {
            m_elements = NULL;
            m_count = 0;
            m_capacity = 0;
        }
        ~ParsStringMapElems()
        {
            for (size_t i = 0; i < m_count; i++)
                delete m_elements[i];
            delete[] m_elements;
        }
        size_t size() { return m_count; }

        void Assign(FdoString* name, FdoString* value)
        {
            ParsStringMapElem* elem = find(name);
            if (elem == NULL)
            {
                ParsStringMapElem* newElem = new ParsStringMapElem(name, value);
                if (m_count <= m_capacity)
                    resize();
                m_elements[m_count] = newElem;
                m_count ++;
            }
            else
                elem->SetValue(value);
        }
        
        ParsStringMapElem* at(size_t idx)
        {
            return (idx < m_count) ? m_elements[idx] : NULL;
        }

        // A connection can have max 4-6 property values
        // we can get faster search in this case by not using a map
        ParsStringMapElem* find(FdoString* name)
        {
            for (size_t i = 0; i < m_count; i++)
            {
                if (wcscmp(m_elements[i]->GetName(), name) == 0)
                    return m_elements[i];
            }
            return NULL;
        }
    };
private:
    ParsStringMapElems m_valueMap;
    bool m_connStringValid;

public:
    FdoCommonConnStringParser (FdoCommonConnPropDictionary* propDictionary, const wchar_t* connection_string);
    virtual ~FdoCommonConnStringParser (void){};

    const char* GetPropertyValue(FdoString* propertyName);
    const wchar_t* GetPropertyValueW(FdoString* propertyName);
    bool IsPropertyValueSet(FdoString* propertyName);
    bool IsConnStringValid(){return m_connStringValid;};
    bool HasInvalidProperties(FdoCommonConnPropDictionary* propDictionary);
    const wchar_t* GetFirstInvalidPropertyName(FdoCommonConnPropDictionary* propDictionary);

protected:
    void SetPropertyValue (FdoCommonConnPropDictionary* propDictionary, const wchar_t* keyword, const wchar_t* value, bool isQuoted = false);
};

#endif // FDOCOMMONCONNSTRINGPARSER_H

