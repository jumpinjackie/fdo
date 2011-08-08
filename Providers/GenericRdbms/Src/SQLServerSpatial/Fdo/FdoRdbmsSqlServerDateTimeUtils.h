/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

#include <vector>
#include <string>

#ifndef FDORDBMSSQLSERVERDATETIMEUTILS_H
#define FDORDBMSSQLSERVERDATETIMEUTILS_H

#ifdef _WIN32
    #define _ITOAW(buffer, val) _itow(val, buffer, 10);
#else
    #define _ITOAW(buffer, val) swprintf(buffer, 30, L"%d", val);
#endif	

enum TokenDateFormatType
{
    TokenDateFormatType_Unknown,
    TokenDateFormatType_Separator,
    TokenDateFormatType_Year2,
    TokenDateFormatType_Year4,
    TokenDateFormatType_Month_AbbName_All_Lower,
    TokenDateFormatType_Month_AbbName_All_Upper,
    TokenDateFormatType_Month_AbbName_First_Upper,
    TokenDateFormatType_Month_FullName_All_Lower,
    TokenDateFormatType_Month_FullName_All_Upper,
    TokenDateFormatType_Month_FullName_First_Upper,
    TokenDateFormatType_Month_Number,
    TokenDateFormatType_Day_AbbName_All_Lower,
    TokenDateFormatType_Day_AbbName_All_Upper,
    TokenDateFormatType_Day_AbbName_First_Upper,
    TokenDateFormatType_Day_FullName_All_Lower,
    TokenDateFormatType_Day_FullName_All_Upper,
    TokenDateFormatType_Day_FullName_First_Upper,
    TokenDateFormatType_Day_Number,
    TokenDateFormatType_Hour24,
    TokenDateFormatType_Hour12,
    TokenDateFormatType_Minute,
    TokenDateFormatType_Second,
    TokenDateFormatType_am,
    TokenDateFormatType_pm
};

class DateToken
{
    friend class DateTokenFormat;
    std::wstring m_value;
    TokenDateFormatType m_type;
private:
    DateToken(const wchar_t* val, TokenDateFormatType type = TokenDateFormatType_Unknown)
        : m_type(type), m_value(val)
    {
        
    }
    DateToken(const wchar_t* val, size_t sz, TokenDateFormatType type = TokenDateFormatType_Unknown)
        : m_type(type)
    {
        m_value = std::wstring(val, sz);
    }
public:
    size_t GetTokenLen()
    {
        return m_value.size();
    }
    const wchar_t* GetTokenValue()
    {
        return m_value.c_str();
    }
    TokenDateFormatType GetTokenType()
    {
        return m_type;
    }
};

class DateTokenFormat
{
    std::vector<DateToken*> m_values;
    std::wstring m_result;
    bool m_hasValidTokens;

public:
    DateTokenFormat() : m_hasValidTokens(false)
    {
    }
    ~DateTokenFormat()
    {
        Clear();
    }
    void Clear()
    {
        for(std::vector<DateToken*>::iterator it = m_values.begin(); it < m_values.end(); it++)
            delete *it;
        m_values.clear();
    }
private:
    TokenDateFormatType StringToDateFormat(const wchar_t* specifier)
{
    if (wcsncmp(specifier, L"YYYY", 4) == 0)
        return TokenDateFormatType_Year4;

    if (wcsncmp(specifier, L"YY", 2) == 0)
        return TokenDateFormatType_Year2;

    if (wcsncmp(specifier, L"MONTH", 5) == 0)
        return TokenDateFormatType_Month_FullName_All_Upper;

    if (wcsncmp(specifier, L"Month", 5) == 0)
        return TokenDateFormatType_Month_FullName_First_Upper;

    if (wcsncmp(specifier, L"month", 5) == 0)
        return TokenDateFormatType_Month_FullName_All_Lower;

    if (wcsncmp(specifier, L"MON", 3) == 0)
        return TokenDateFormatType_Month_AbbName_All_Upper;

    if (wcsncmp(specifier, L"mon", 3) == 0)
        return TokenDateFormatType_Month_AbbName_All_Lower;

    if (wcsncmp(specifier, L"Mon", 3) == 0)
        return TokenDateFormatType_Month_AbbName_First_Upper;

    if (wcsncmp(specifier, L"MM", 2) == 0)
        return TokenDateFormatType_Month_Number;

    if (wcsncmp(specifier, L"DAY", 3) == 0)
        return TokenDateFormatType_Day_FullName_All_Upper;

    if (wcsncmp(specifier, L"Day", 3) == 0)
        return TokenDateFormatType_Day_FullName_First_Upper;

    if (wcsncmp(specifier, L"day", 3) == 0)
        return TokenDateFormatType_Day_FullName_All_Lower;

    if (wcsncmp(specifier, L"DY", 2) == 0)
        return TokenDateFormatType_Day_AbbName_All_Upper;

    if (wcsncmp(specifier, L"dy", 2) == 0)
        return TokenDateFormatType_Day_AbbName_All_Lower;

    if (wcsncmp(specifier, L"Dy", 2) == 0)
        return TokenDateFormatType_Day_AbbName_First_Upper;

    if (wcsncmp(specifier, L"DD", 2) == 0)
        return TokenDateFormatType_Day_Number;

    if (wcsncmp(specifier, L"hh24", 4) == 0 || wcsncmp(specifier, L"hh", 2) == 0)
        return TokenDateFormatType_Hour24;

    if (wcsncmp(specifier, L"hh12", 4) == 0)
        return TokenDateFormatType_Hour12;

    if (wcsncmp(specifier, L"mm", 2) == 0)
        return TokenDateFormatType_Minute;

    if (wcsncmp(specifier, L"ss", 2) == 0)
        return TokenDateFormatType_Second;

    if (wcsncmp(specifier, L"am", 2) == 0 || wcsncmp(specifier, L"AM", 2) == 0)
        return TokenDateFormatType_am;

    if (wcsncmp(specifier, L"pm", 2) == 0 || wcsncmp(specifier, L"PM", 2) == 0)
        return TokenDateFormatType_pm;

    // should we thow an exception !?
    // we lose custom formats like "My date YY/MM/DD"
    return TokenDateFormatType_Unknown;
}


    bool HasValidTokens() {return m_hasValidTokens;}
    DateToken* AddDateToken(const wchar_t* val, TokenDateFormatType type = TokenDateFormatType_Unknown)
    {
        DateToken* ret = new DateToken(val, type);
        m_values.push_back(ret);
        return ret;
    }
    DateToken* AddDateToken(const wchar_t* val, size_t sz, TokenDateFormatType type = TokenDateFormatType_Unknown)
    {
        DateToken* ret = new DateToken(val, sz, type);
        m_values.push_back(ret);
        return ret;
    }
public:
    void ProcessFormat(const wchar_t* str)
    {
        Clear();
        if (str != NULL && *str != '\0')
        {
            size_t startPos = 0;
            const wchar_t* strtmp = str;
            int state = -1;
            do
            {
                bool isSepChar = !isalnum(*strtmp);
                switch(state)
                {
                case -1: // start
                    state = isSepChar ? 0 : 1;
                    break;
                case 0: // separators
                    if (!isSepChar)
                    {
                        AddDateToken(str + startPos, (int)(strtmp-str) - startPos, TokenDateFormatType_Separator);
                        state = 1;
                        startPos = (int)(strtmp-str);
                    }            
                    break;
                case 1: // tokens
                    if (isSepChar)
                    {
                        TokenDateFormatType type = StringToDateFormat(str + startPos);
                        if (type != TokenDateFormatType_Unknown && type != TokenDateFormatType_Separator)
                            m_hasValidTokens = true;
                        AddDateToken(str + startPos, (int)(strtmp-str) - startPos, type);
                        state = 0;
                        startPos = (int)(strtmp-str);
                    }            
                    break;
                }
                strtmp++;
            }
            while (*strtmp != '\0');
            if (state == 0)
                AddDateToken(str + startPos, TokenDateFormatType_Separator);
            else
            {
                TokenDateFormatType type = StringToDateFormat(str + startPos);
                if (type != TokenDateFormatType_Unknown && type != TokenDateFormatType_Separator)
                    m_hasValidTokens = true;
                AddDateToken(str + startPos, type);
            }
        }
        else
        {
            // use the default format e.g.  "21-JUL-2009 13:24:14.000"
            AddDateToken(L"", TokenDateFormatType_Day_Number);
            AddDateToken(L"-", TokenDateFormatType_Separator);
            AddDateToken(L"", TokenDateFormatType_Month_AbbName_All_Upper);
            AddDateToken(L"-", TokenDateFormatType_Separator);
            AddDateToken(L"", TokenDateFormatType_Year4);
            AddDateToken(L" ", TokenDateFormatType_Separator);
            AddDateToken(L"", TokenDateFormatType_Hour24);
            AddDateToken(L":", TokenDateFormatType_Separator);
            AddDateToken(L"", TokenDateFormatType_Minute);
            AddDateToken(L":", TokenDateFormatType_Separator);
            AddDateToken(L"", TokenDateFormatType_Second);
            m_hasValidTokens = true;
        }
    }

    typedef std::pair< int, std::pair<size_t, size_t> > TokenInfoPair;
    const wchar_t* ToDateTimeSQL(const wchar_t* exp)
    {
        m_result.clear();

        size_t idx = 1;
        bool hasAmPm = false;
        int hasDate = 0;
        int hasTime = 0;
        // <type, pos, len>
        std::vector< TokenInfoPair > tokens(7);
        for(std::vector<DateToken*>::iterator it = m_values.begin(); it < m_values.end(); it++)
        {
            DateToken* tk = *it;
            switch(tk->GetTokenType())
            {
            case TokenDateFormatType_Year2:
                {
                    TokenInfoPair& token = tokens.at(0);
                    if (token.first == 0)
                    {
                        token.first = 1; // we have year
                        token.second.first = idx; // pos of year value
                        token.second.second = 2; // pos of year value
                        hasDate++;
                    }
                    idx += 2;
                }
                break;

            case TokenDateFormatType_Year4:
                {
                    TokenInfoPair& token = tokens.at(0);
                    if (token.first == 0)
                        hasDate++;
                    // let's overwite the year in case we already find it
                    // since this is a 4 digits year
                    token.first = 1; // we have year
                    token.second.first = idx; // pos of year value
                    token.second.second = 4; // pos of year value
                    idx += 4;
                }
                break;

            case TokenDateFormatType_Month_FullName_All_Lower:
            case TokenDateFormatType_Month_FullName_All_Upper:
            case TokenDateFormatType_Month_FullName_First_Upper:
            case TokenDateFormatType_Day_FullName_All_Lower:
            case TokenDateFormatType_Day_FullName_All_Upper:
            case TokenDateFormatType_Day_FullName_First_Upper:
            case TokenDateFormatType_Day_AbbName_All_Lower:
            case TokenDateFormatType_Day_AbbName_All_Upper:
            case TokenDateFormatType_Day_AbbName_First_Upper:
                throw FdoException::Create(L"Invalid parameters!");

            case TokenDateFormatType_Month_AbbName_All_Lower:
            case TokenDateFormatType_Month_AbbName_All_Upper:
            case TokenDateFormatType_Month_AbbName_First_Upper:
                {
                    TokenInfoPair& token = tokens.at(1);
                    if (token.first == 0)
                    {
                        token.first = 2; // we have month (AbbName)
                        token.second.first = idx; // pos of month value
                        token.second.second = 3; // pos of month value
                        hasDate++;
                    }
                    idx += 3;
                }
                break;

            case TokenDateFormatType_Month_Number:
                {
                    TokenInfoPair& token = tokens.at(1);
                    if (token.first == 0)
                        hasDate++;
                    // let's overwite the month in case we already find it
                    // since this is a month number
                    token.first = 1; // we have month (number)
                    token.second.first = idx; // pos of month value
                    token.second.second = 2; // pos of month value
                    idx += 2;
                }
                break;

            case TokenDateFormatType_Day_Number:
                {
                    TokenInfoPair& token = tokens.at(2);
                    if (token.first == 0)
                    {
                        hasDate++;
                        token.first = 1; // we have day (number)
                        token.second.first = idx; // pos of day value
                        token.second.second = 2; // pos of day value
                    }
                    idx += 2;
                }
                break;

            case TokenDateFormatType_Hour24:
            case TokenDateFormatType_Hour12:
                {
                    TokenInfoPair& token = tokens.at(3);
                    if (token.first == 0)
                    {
                        hasTime++;
                        token.first = 1; // we have hour (number)
                        token.second.first = idx; // pos of hour value
                        token.second.second = 2; // pos of hour value
                    }
                    idx += 2;
                }
                break;

            case TokenDateFormatType_Minute:
                {
                    TokenInfoPair& token = tokens.at(4);
                    if (token.first == 0)
                    {
                        hasTime++;
                        token.first = 1; // we have minute (number)
                        token.second.first = idx; // pos of minute value
                        token.second.second = 2; // pos of minute value
                    }
                    idx += 2;
                }
                break;

            case TokenDateFormatType_Second:
                {
                    // here is hard to know if we have only 2 digits or
                    // 5 digits and a dot, so let's get 2 only
                    TokenInfoPair& token = tokens.at(5);
                    if (token.first == 0)
                    {
                        hasTime++;
                        token.first = 1; // we have minute (number)
                        token.second.first = idx; // pos of minute value
                        token.second.second = 2; // pos of minute value
                    }
                    idx += 2;
                }
                break;
            case TokenDateFormatType_am:
            case TokenDateFormatType_pm:
                {
                    TokenInfoPair& token = tokens.at(6);
                    if (token.first == 0)
                    {
                        token.first = 1; // we have AM/PM
                        token.second.first = idx; // pos of minute value
                        token.second.second = 2; // pos of minute value
                    }
                    hasAmPm = true;
                    idx += 2;
                }
                break;
            default: // Separator & Unknown
                idx += tk->GetTokenLen();
                break;
            }
        }
        // incomplete data/time?
        if (hasDate < 3 && hasTime < 3)
            return NULL;

        wchar_t buffer[30];
        m_result.append(L"CONVERT(datetime2,");
        if (hasDate >= 3)
        {
            for (size_t i = 0; i < 3; i++)
            {
                TokenInfoPair& token = tokens.at(i);
                // we have year yy, so we need to add 2000 or 1900
                if (i == 0 && token.second.second == 2)
                {
                    m_result.append(L"CONVERT(NVARCHAR, CASE WHEN CONVERT(BIGINT, SUBSTRING(");
                    m_result.append(exp);
                    m_result.append(L",");
                    _ITOAW(buffer, token.second.first)
                    m_result.append(buffer);
                    m_result.append(L",");
                    _ITOAW(buffer, token.second.second)
                    m_result.append(buffer);
                    m_result.append(L"))<50 THEN CONVERT(BIGINT, SUBSTRING(");
                    m_result.append(exp);
                    m_result.append(L",");
                    _ITOAW(buffer, token.second.first)
                    m_result.append(buffer);
                    m_result.append(L",");
                    _ITOAW(buffer, token.second.second)
                    m_result.append(buffer);
                    m_result.append(L"))+2000 ELSE CONVERT(BIGINT, SUBSTRING(");
                    m_result.append(exp);
                    m_result.append(L",");
                    _ITOAW(buffer, token.second.first)
                    m_result.append(buffer);
                    m_result.append(L",");
                    _ITOAW(buffer, token.second.second)
                    m_result.append(buffer);
                    m_result.append(L"))+1900 END)");
                }
                else
                {
                    // we have month abbr?
                    if (i == 1 && token.first == 2)
                    {
                        m_result.append(L" CASE UPPER(SUBSTRING(");
                        m_result.append(exp);
                        m_result.append(L",");
                        _ITOAW(buffer, token.second.first)
                        m_result.append(buffer);
                        m_result.append(L",");
                        _ITOAW(buffer, token.second.second)
                        m_result.append(buffer);
                        m_result.append(L")) WHEN UPPER(LEFT(DATENAME(MONTH,'2000-01-01'),3)) THEN N'01' WHEN UPPER(LEFT(DATENAME(MONTH,'2000-02-01'),3)) THEN N'02' WHEN UPPER(LEFT(DATENAME(MONTH,'2000-03-01'),3)) THEN N'03' WHEN UPPER(LEFT(DATENAME(MONTH,'2000-04-01'),3)) THEN N'04' WHEN UPPER(LEFT(DATENAME(MONTH,'2000-05-01'),3)) THEN N'05' WHEN UPPER(LEFT(DATENAME(MONTH,'2000-06-01'),3)) THEN N'06' WHEN UPPER(LEFT(DATENAME(MONTH,'2000-07-01'),3)) THEN N'07' WHEN UPPER(LEFT(DATENAME(MONTH,'2000-08-01'),3)) THEN N'08' WHEN UPPER(LEFT(DATENAME(MONTH,'2000-09-01'),3)) THEN N'09' WHEN UPPER(LEFT(DATENAME(MONTH,'2000-10-01'),3)) THEN N'10' WHEN UPPER(LEFT(DATENAME(MONTH,'2000-11-01'),3)) THEN N'11' WHEN UPPER(LEFT(DATENAME(MONTH,'2000-12-01'),3)) THEN N'12' ELSE N'' END");
                    }
                    else
                    {
                        m_result.append(L"SUBSTRING(");
                        m_result.append(exp);
                        m_result.append(L",");
                        _ITOAW(buffer, token.second.first)
                        m_result.append(buffer);
                        m_result.append(L",");
                        _ITOAW(buffer, token.second.second)
                        m_result.append(buffer);
                        m_result.append(L")");
                    }
                }
                if (i != 2)
                    m_result.append(L"+'-'+");
                else
                    m_result.append(L"+' '");
            }
        }
        if (hasTime >= 3)
        {
            for (size_t i = 3; i < 6; i++)
            {
                TokenInfoPair& token = tokens.at(i);
                m_result.append(L"SUBSTRING(");
                m_result.append(exp);
                m_result.append(L",");
                if (!hasAmPm || i != 0)
                {
                    _ITOAW(buffer, token.second.first)
                    m_result.append(buffer);
                    m_result.append(L",");
                    _ITOAW(buffer, token.second.second)
                    m_result.append(buffer);
                }
                else // in case we have AP/PM we need to add +12 at hour when is PM
                {
                    TokenInfoPair& amPmToken = tokens.at(6);
                    m_result.append(L"CASE WHEN UPPER(SUBSTRING(");
                    m_result.append(exp);
                    m_result.append(L",");
                    _ITOAW(buffer, amPmToken.second.first)
                    m_result.append(buffer);
                    m_result.append(L",");
                    _ITOAW(buffer, amPmToken.second.second)
                    m_result.append(buffer);
                    m_result.append(L"))=N'PM' THEN CONVERT(NVARCHAR, CONVERT(BIGINT, SUBSTRING(");
                    m_result.append(exp);
                    m_result.append(L",");
                    _ITOAW(buffer, token.second.first)
                    m_result.append(buffer);
                    m_result.append(L",");
                    _ITOAW(buffer, token.second.second)
                    m_result.append(buffer);
                    m_result.append(L")) + 12) ELSE SUBSTRING(");
                    m_result.append(exp);
                    m_result.append(L",");
                    _ITOAW(buffer, token.second.first)
                    m_result.append(buffer);
                    m_result.append(L",");
                    _ITOAW(buffer, token.second.second)
                    m_result.append(buffer);
                    m_result.append(L") END");
                }
                if (i != 2)
                    m_result.append(L")+':'+");
                else
                    m_result.append(L")");
            }
        }
        m_result.append(L")");
        return m_result.c_str();
    }

    const wchar_t* ToSQL(const wchar_t* exp)
    {
        m_result.clear();
        for(std::vector<DateToken*>::iterator it = m_values.begin(); it < m_values.end(); it++)
        {
            DateToken* tk = *it;
            switch(tk->GetTokenType())
            {
            case TokenDateFormatType_Year2:
                m_result.append(L"RIGHT(N'0'+CAST(DATEPART(YEAR,");
                m_result.append(exp); // add "" if needed
                m_result.append(L") AS NVARCHAR), 2)+");
                break;
            case TokenDateFormatType_Year4:
                m_result.append(L"RIGHT(N'000'+CAST(DATEPART(YEAR,");
                m_result.append(exp); // add "" if needed
                m_result.append(L") AS NVARCHAR), 4)+");
                break;
            case TokenDateFormatType_Month_AbbName_All_Lower:
                m_result.append(L"LOWER(LEFT(DATENAME(MONTH, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L"), 3))+");
                break;
            case TokenDateFormatType_Month_AbbName_All_Upper:
                m_result.append(L"UPPER(LEFT(DATENAME(MONTH, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L"), 3))+");
                break;
            case TokenDateFormatType_Month_FullName_All_Lower:
                m_result.append(L"LOWER(DATENAME(MONTH, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L"))+");
                break;
            case TokenDateFormatType_Month_FullName_All_Upper:
                m_result.append(L"UPPER(DATENAME(MONTH, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L"))+");
                break;
            case TokenDateFormatType_Month_FullName_First_Upper:
                m_result.append(L"DATENAME(MONTH, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L")+");
                break;
            case TokenDateFormatType_Month_AbbName_First_Upper:
                m_result.append(L"LEFT(DATENAME(MONTH, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L"),3)+");
                break;
            case TokenDateFormatType_Month_Number:
                m_result.append(L"RIGHT(N'0'+CAST(DATEPART(MONTH,");
                m_result.append(exp); // add "" if needed
                m_result.append(L") AS NVARCHAR), 2)+");
                break;
            case TokenDateFormatType_Day_AbbName_All_Lower:
                m_result.append(L"LOWER(LEFT(DATENAME(WEEKDAY, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L"), 3))+");
                break;
            case TokenDateFormatType_Day_AbbName_All_Upper:
                m_result.append(L"UPPER(LEFT(DATENAME(WEEKDAY, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L"), 3))+");
                break;
            case TokenDateFormatType_Day_FullName_All_Lower:
                m_result.append(L"LOWER(DATENAME(WEEKDAY, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L"))+");
                break;
            case TokenDateFormatType_Day_FullName_All_Upper:
                m_result.append(L"UPPER(DATENAME(WEEKDAY, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L"))+");
                break;
            case TokenDateFormatType_Day_FullName_First_Upper:
                m_result.append(L"DATENAME(WEEKDAY, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L")+");
                break;
            case TokenDateFormatType_Day_AbbName_First_Upper:
                m_result.append(L"LEFT(DATENAME(MONTH, ");
                m_result.append(exp); // add "" if needed
                m_result.append(L"),3)+");
                break;                    
            case TokenDateFormatType_Day_Number:
                m_result.append(L"RIGHT(N'0'+CAST(DATEPART(DAY,");
                m_result.append(exp); // add "" if needed
                m_result.append(L") AS NVARCHAR),2)+");
                break;
            case TokenDateFormatType_Hour24:
                m_result.append(L"RIGHT(N'0'+CAST(DATEPART(HOUR,");
                m_result.append(exp); // add "" if needed
                m_result.append(L") AS NVARCHAR),2)+");
                break;
            case TokenDateFormatType_Hour12:
                m_result.append(L"RIGHT(N'0'+CAST(CASE WHEN DATEPART(HOUR,");
                m_result.append(exp); // add "" if needed
                m_result.append(L")>12 THEN DATEPART(HOUR,");
                m_result.append(exp); // add "" if needed
                m_result.append(L")-12 ELSE DATEPART(HOUR,");
                m_result.append(exp); // add "" if needed
                m_result.append(L") END AS NVARCHAR),2)+");
                break;
            case TokenDateFormatType_Minute:
                m_result.append(L"RIGHT(N'0'+CAST(DATEPART(MINUTE,");
                m_result.append(exp); // add "" if needed
                m_result.append(L") AS NVARCHAR),2)+");
                break;
            case TokenDateFormatType_Second:
                m_result.append(L"RIGHT(N'0'+CAST(DATEPART(SECOND,");
                m_result.append(exp);
                m_result.append(L") AS NVARCHAR),2)+");
                m_result.append(L"(CASE WHEN DATEPART(MILLISECOND,");
                m_result.append(exp);
                m_result.append(L")=0 THEN N'' ELSE N'.'+CAST(DATEPART(MILLISECOND,");
                m_result.append(exp);
                m_result.append(L") AS NVARCHAR) END)+");
                break;
            case TokenDateFormatType_am:
            case TokenDateFormatType_pm:
                m_result.append(L"(CASE WHEN DATEPART(HOUR,");
                m_result.append(exp); // add "" if needed
                m_result.append(L")>12 THEN 'PM' ELSE 'AM' END)+");
                break;
            default: // Separator & Unknown
                m_result.append(L"\'");
                m_result.append(tk->GetTokenValue());
                m_result.append(L"\'+");
                break;
            }
        }
        m_result.resize(m_result.size()-1);
        return m_result.c_str();
    }
};

#endif
