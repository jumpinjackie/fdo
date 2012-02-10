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
#include "FdoCommonConnPropDictionary.h"


void FdoCommonConnStringParser::SetPropertyValue (FdoCommonConnPropDictionary* propDictionary, const wchar_t* keyword, const wchar_t* value, bool isQuoted)
{
    bool bAddValue = true;
    FdoInt32 propCount=0;
    FdoString** propNames = NULL;
    if (propDictionary)
    {
        bAddValue = false;
        propNames = propDictionary->GetPropertyNames(propCount);
        for (FdoInt32 i = 0; i < propCount; i++)
        {
            if (0 == FdoCommonOSUtil::wcsnicmp (propNames[i], keyword, wcslen (keyword)))
            {
                bAddValue = true;
                break;
            }
        }
    }
    if (bAddValue)
    {
        FdoStringP pkeyword = ((FdoStringP)keyword).Lower();
        m_valueMap.Assign(pkeyword, value);
        if (isQuoted && NULL != propDictionary)
        {
            FdoPtr<ConnectionProperty> pProp = propDictionary->FindProperty (keyword);
            pProp->SetIsPropertyQuoted (isQuoted);
        }
    }
}

bool FdoCommonConnStringParser::HasInvalidProperties(FdoCommonConnPropDictionary* propDictionary)
{
    bool bRetVal = false;
    FdoInt32 propCount = 0;
    FdoString** propNames = NULL;
    FdoInt32 internalPropCount = (FdoInt32)m_valueMap.size();
    if (propDictionary)
    {
        propNames = propDictionary->GetPropertyNames(propCount);
        for (FdoInt32 i = 0; i < propCount; i++)
        {
            if (IsPropertyValueSet(propNames[i]))
                internalPropCount--;
        }
        bRetVal = (internalPropCount != 0);
    }
    return bRetVal;
}

const wchar_t* FdoCommonConnStringParser::GetFirstInvalidPropertyName(FdoCommonConnPropDictionary* propDictionary)
{
    FdoInt32 propCount = 0;
    FdoString** propNames = NULL;
    if (!propDictionary || !m_valueMap.size())
        return NULL;
    propNames = propDictionary->GetPropertyNames(propCount);

    for (size_t it = 0; it < m_valueMap.size(); it++)
    {
        FdoString* pName = m_valueMap.at(it)->GetName();
        bool foundProp = false;
        for (FdoInt32 i = 0; i < propCount; i++)
        {
            if (0 == FdoCommonOSUtil::wcsicmp (propNames[i], pName))
            {
                foundProp = true;
                break;
            }
        }
        if (!foundProp)
            return pName;
    }
    return NULL;
}

FdoCommonConnStringParser::FdoCommonConnStringParser (FdoCommonConnPropDictionary* propDictionary, const wchar_t* connection_string) :
    m_connStringValid(false)
{
    if(!connection_string)
        return;
    
    wchar_t* lastKey = NULL;
    size_t lenBuffer = 0;
    wchar_t* lastValue = NULL;
    size_t lenValBuffer = 0;
    bool isError = false;
    short State = 0;
    int pPoz = 0, pPozPN = 0, pPozPV = 0, pPozEnd = 0;
    do
    {
        switch(State)
        {
            case 0: //looking for start property name
                if (*(connection_string+pPoz) == L'=')
                    isError = true;
                else if (*(connection_string+pPoz) != L' ' && *(connection_string+pPoz) != L';')
                {
                    pPozPN = pPoz;
                    pPozEnd = pPoz+1;
                    State = 1;
                }
            break;
            case 1: //get property name
                if (*(connection_string+pPoz) == L'=')
                {
                    if (lenBuffer < (size_t)(pPozEnd-pPozPN+1))
                    {
                        delete[] lastKey;
                        lenBuffer = pPozEnd-pPozPN+1;
                        lastKey = new wchar_t[lenBuffer];
                    }
                    wcsncpy(lastKey, connection_string+pPozPN, (size_t)pPozEnd-pPozPN);
                    lastKey[pPozEnd-pPozPN] = L'\0';
                    SetPropertyValue(propDictionary, lastKey, L"");
                    if (*(connection_string+pPoz+1) == L'\"')
                    {
                        pPoz++;
                        State = 3;
                    }
                    else if (*(connection_string+pPoz+1) == L' ')
                    {
                        pPoz++;
                        State = 4;
                    }
                    else
                    {
                        pPozEnd = pPoz+1;
                        State = 2;
                    }
                    pPozPV = pPoz+1;
                }
                else if(*(connection_string+pPoz) == L';' || *(connection_string+pPoz) == L'\0')
                    isError = true;
                else if(*(connection_string+pPoz) != L' ')
                {
                    pPozEnd = pPoz+1;
                }
            break;
            case 2:  //get property value in case value is not surrounded by "
                if(*(connection_string+pPoz) == L'\"')
                    isError = true;
                else if(*(connection_string+pPoz) == L';' || *(connection_string+pPoz) == L'\0')
                {
                    if (lenValBuffer < (size_t)(pPozEnd-pPozPV+1))
                    {
                        delete[] lastValue;
                        lenValBuffer = pPozEnd-pPozPV+1;
                        lastValue = new wchar_t[lenValBuffer];
                    }
                    wcsncpy(lastValue, connection_string+pPozPV, (size_t)pPozEnd-pPozPV);
                    lastValue[pPozEnd-pPozPV] = L'\0';
                    SetPropertyValue(propDictionary, lastKey, lastValue);
                    State = 0;
                }
                else if(*(connection_string+pPoz) != L' ')
                    pPozEnd = pPoz+1;
            break;
            case 3:  //get property value in case value is surrounded by "
                if(*(connection_string+pPoz) == L'\"')
                {
                    if (lenValBuffer < (size_t)(pPoz-pPozPV+1))
                    {
                        delete[] lastValue;
                        lenValBuffer = pPoz-pPozPV+1;
                        lastValue = new wchar_t[lenValBuffer];
                    }
                    wcsncpy(lastValue, connection_string+pPozPV, (size_t)pPoz-pPozPV);
                    lastValue[pPoz-pPozPV] = L'\0';
                    SetPropertyValue(propDictionary, lastKey, lastValue, true);
                    State = 0;
                }
                else if(*(connection_string+pPoz+1) == L'\0')
                    isError = true;
            break;
            case 4:  //handle space before "
                if (*(connection_string+pPoz) == L'\"')
                {
                    pPoz++;
                    State = 3;
                }
                else if(*(connection_string+pPoz) == L';')
                {
                    State = 0;
                }
                else if(*(connection_string+pPoz) != L' ')
                {
                    pPozEnd = pPoz;
                    State = 2;
                }
                pPozPV = pPoz;
            break;
        }
        pPoz++;
    }while(*(connection_string+pPoz-1) != L'\0' && !isError);
    delete[] lastKey;
    delete[] lastValue;
    m_connStringValid = !isError;
}

const char* FdoCommonConnStringParser::GetPropertyValue(FdoString* propertyName)
{
    FdoStringP pPropertyName = ((FdoStringP)propertyName).Lower();
    ParsStringMapElem* it = m_valueMap.find((FdoString*)pPropertyName);
    return (it == NULL) ? NULL : it->GetMbValue();
}

const wchar_t* FdoCommonConnStringParser::GetPropertyValueW(FdoString* propertyName)
{
    FdoStringP pPropertyName = ((FdoStringP)propertyName).Lower();
    ParsStringMapElem* it = m_valueMap.find((FdoString*)pPropertyName);
    return (it == NULL) ? NULL : it->GetValue();
}

bool FdoCommonConnStringParser::IsPropertyValueSet(FdoString* propertyName)
{
    FdoStringP pPropertyName = ((FdoStringP)propertyName).Lower();
    ParsStringMapElem* it = m_valueMap.find((FdoString*)pPropertyName);
    return (it != NULL);
}

