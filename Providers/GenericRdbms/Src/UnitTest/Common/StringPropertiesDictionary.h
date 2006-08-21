#ifndef CPP_UNIT_STRINGPROPSDICT_H
#define CPP_UNIT_STRINGPROPSDICT_H

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

#include <map>
#include <string>

typedef std::map<std::wstring, std::wstring> PROPSSTRING_MAP;
typedef PROPSSTRING_MAP::iterator PROPSSTRING_MAPIT;

class StringPropertiesDictionary : public FdoIDisposable
{
protected:
    PROPSSTRING_MAP m_ValueMap;

public:
	StringPropertiesDictionary(void)
	{
	}
protected:
	virtual ~StringPropertiesDictionary(void)
	{
		Clear();
	}
	void Dispose()
    {
        delete this;
    };

public:
	virtual bool ParseStringProperties(FdoString* pInStringp)
	{
		m_ValueMap.clear();
		if(!pInStringp)
			return false;
		FdoStringP pInStr = pInStringp;
		pInStr = pInStr.Replace(L"\n", L";");
		pInStr = pInStr.Replace(L"\r", L";");
		FdoString* pInString = pInStr;
		std::wstring LastKey;
		short State = 0;
		int pPoz = 0, pPozPN, pPozPV, pPozEnd;
		do
		{
			switch(State)
			{
				case 0:
					if (*(pInString+pPoz) == L'=')
					{
						m_ValueMap.clear();
						return false;
					}
					else if (*(pInString+pPoz) != L' ' && *(pInString+pPoz) != L';')
					{
						pPozEnd = pPozPN = pPoz;
						State = 1;
					}
				break;
				case 1:
					if (*(pInString+pPoz) == L'=')
					{
						LastKey = std::wstring(pInString+pPozPN, pPozEnd-pPozPN);
						m_ValueMap[LastKey] = L"";
						pPozPV = pPoz+1;
						State = 2;
					}
					else if(*(pInString+pPoz) == L';' || *(pInString+pPoz) == L'\0')
					{
						LastKey = std::wstring(pInString+pPozPN, pPozEnd-pPozPN);
						m_ValueMap[LastKey] = L"";
						State = 0;
					}
					else if(*(pInString+pPoz) != L' ')
					{
						pPozEnd = pPoz+1;
					}
				break;
				case 2:
					if(*(pInString+pPoz) == L'=')
					{
						m_ValueMap.clear();
						return false;
					}
					else if(*(pInString+pPoz) == L';' || *(pInString+pPoz) == L'\0')
					{
						m_ValueMap[LastKey] = std::wstring(pInString+pPozPV, pPoz-pPozPV);
						State = 0;
					}
				break;
			}
			pPoz++;
		}while(*(pInString+pPoz-1) != L'\0');
		return true;
	}

	virtual FdoStringP GetStringProperties()
	{
		PROPSSTRING_MAPIT it;
		std::wstring retVal = L"";
		for(it = m_ValueMap.begin(); it != m_ValueMap.end(); it++)
		{
			retVal += it->first;
			retVal += L"=";
			retVal += it->second;
			retVal += L";";
		}
		return retVal.c_str();
	}

	virtual bool PropertyExist(FdoString* name)
	{
		if (name == NULL)
			return false;
		return (m_ValueMap.find(name) != m_ValueMap.end());
	}

	FdoString* GetPropertyValue (FdoString* name)
	{
		PROPSSTRING_MAPIT it = m_ValueMap.find(name);
		return (it == m_ValueMap.end()) ? NULL : it->second.c_str();
	}

	void SetProperty (FdoString* name, FdoString* value)
	{
		if (name != NULL)
			m_ValueMap[name] = value;
	}

	void RemoveProperty (FdoString* name)
	{
		if (name != NULL)
		{
			PROPSSTRING_MAPIT it = m_ValueMap.find(name);
			if (it != m_ValueMap.end())
				m_ValueMap.erase(it);
		}
	}
	virtual void Clear ()
	{
		m_ValueMap.clear();
	}
};

#endif // CPP_UNIT_STRINGPROPSDICT_H
