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
#include "Constants.h"
#include "RowData.h"
#include "ShapeCPG.h"
#include <wctype.h>
#include <FdoCommonOSUtil.h>
#include <math.h>

/*****************************************************************************
 * Name         : RowData
 *
 * Description  : Constructor
 *
 * Parameters   : ColumnInfo* pColumnInfo
 *              : void* buffer
 *
 * Return       : N/A
 *
 * Notes        : N/A
 *
 *****************************************************************************/
/*
 * Made one change in Sep 18, 2008
 * From: Only records in DBF file which start with cVALID_RECORD_ID can be accepted as valid record.
 * To: Only records in DBF file which start cDELETED_RECORD_ID can be accepted as deleted record.
*/
RowData::RowData (ColumnInfo* pColumnInfo, void* buffer) :
    mDeleted (true),                                   // change default value from "false" to "true" 
    mColumnInfo (pColumnInfo),
    mBuffer (buffer),
    mReading (NULL != buffer)
{
    char* p;
    int size;
    int count;
    wchar_t** s;
    int width;

    p = (char*)this + sizeof (RowData);
    mStrings = (wchar_t**)p;
    p += sizeof (wchar_t*) * pColumnInfo->GetNumColumns ();
    size = 0;
    count = pColumnInfo->GetNumColumns ();
    s = mStrings;
    for (int i = 0; i < count; i++)
    {
        width = pColumnInfo->GetColumnWidthAt (i);
        if (kColumnCharType == mColumnInfo->GetColumnTypeAt (i))
        {
            *s = (wchar_t*)p;
            p += sizeof (wchar_t) * (width + 1);
        }
        s++;
        size += width;
    }
    if (NULL == buffer)
    {
        mBuffer = p;
        size++; // deleted flag
        // all fields are space padded
        memset (mBuffer, ' ', size);
    }
    else if (cDELETED_RECORD_ID != ((char*)buffer)[0])  // change from cVALID_RECORD_ID to cDELETED_RECORD_ID 
        SetDeleted (false);                             // change from "true" to "false" 
}

void* RowData::operator new (size_t nSize, ColumnInfo* pColumnInfo, void* buffer)
{
    size_t size;
    int count;
    int width;
    void *ret;

    size = nSize;
    size += sizeof (wchar_t*) * pColumnInfo->GetNumColumns ();
    count = pColumnInfo->GetNumColumns ();
    for (int i = 0; i < count; i++)
    {
        width = pColumnInfo->GetColumnWidthAt (i);
        if (kColumnCharType == pColumnInfo->GetColumnTypeAt (i))
            size += sizeof (wchar_t) * (width + 1);
        if (NULL == buffer)
            size += width; // for the buffer
    }
    if (NULL == buffer)
    {
        size++; // deleted flag
        size++; // file terminator \x1a if appending
    }
    ret = ::new char [size];
    memset (ret, 0, size);

    return (ret);
}

void RowData::operator delete (void *pObj)
{
    delete [] (char *)pObj;
}

void RowData::operator delete (void *pObj, ColumnInfo* pColumnInfo, void* buffer)
{
    delete [] (char *)pObj;
}

/*****************************************************************************
 * Name         : ~RowData
 *
 * Description  : Destructor
 *
 * Parameters   :
 *
 * Return       : N/A
 *
 * Notes        : N/A
 *
 *****************************************************************************/
RowData::~RowData()
{
}

/// <summary>Sets the decimal data.</summary>
/// <param name="index">Input the column index of the data to set.</param> 
/// <param name="value">Input the value for the column.</param> 
/// <returns>Returns nothing.</returns> 
void RowData::SetData (int index, bool bIsNull, double value)
{
    char buffer[50];
    int width;
    char* raw;
    size_t length;

    if (kColumnDecimalType != mColumnInfo->GetColumnTypeAt (index))
        throw FdoException::Create (NlsMsgGet(SHP_ILLEGAL_COLUMN_ASSIGNMENT, "Illegal column assignment."));
    width = mColumnInfo->GetColumnWidthAt (index);
    raw = ((char*)mBuffer) + mColumnInfo->GetColumnOffsetAt (index);

    if (bIsNull)
        memset (raw, ' ', width);
    else
    {
        // try first with "precision.scale" format:
        sprintf (buffer, "%*.*f", width, mColumnInfo->GetColumnScaleAt(index), value);
        length = strlen (buffer);

        // if it's too long, check for ".000" and remove
        if (length > (unsigned int)width)
        {
            char *p = &buffer[length - 1];
            while (p > buffer)
                if (*p == '0')
                    p--;
                else 
                    break;
            if ((p != buffer) && (*p == '.'))
            {
                *p = '\0';
                length = p - buffer;
            }
        }

        // Validate length:
        if (length > (unsigned int)width)
        {
            sprintf (buffer, "%g", value);
            length = strlen (buffer);

            if (length > (unsigned int)width)
            {
                wchar_t* wBuffer = NULL;
                multibyte_to_wide(wBuffer, buffer);
                throw FdoException::Create(NlsMsgGet(SHP_VALUE_TOO_WIDE_FOR_COLUMN, "The value '%1$ls' is too wide for column '%2$ls'.",
                    wBuffer, mColumnInfo->GetColumnNameAt(index)));
            }
        }

        // all fields are space padded
        memset (raw, ' ', width-length);

		// Substitute ',' (when the locale is French, for example) with '.', the only valid decimal separator.
		if (length > 0)
		{
			char *p = &buffer[length - 1];
			while (p > buffer)
			{
				if (*p == ',')
				{
					*p = '.';
					break;
				}
				p--;
			}
		}

        // right justify
        strncpy ((char*)(raw + width - length), buffer, length);
    }
}

/// <summary>Sets the string data.</summary>
/// <param name="index">Input the column index of the data to set.</param> 
/// <param name="value">Input the value for the column.</param> 
/// <returns>Returns nothing.</returns> 
void RowData::SetData (int index, bool bIsNull, WCHAR* value, WCHAR *codepage)
{
    char* chars;

    if (NULL == value)
        SetData (index, bIsNull, (char*)NULL);
    else
    {
        ShapeCPG    *cpg = new ShapeCPG();

#ifdef _WIN32
        wide_to_multibyte_cpg (chars, value, cpg->ConvertCodePageWin(codepage));
#else
        wide_to_multibyte_cpg (chars, value, cpg->ConvertCodePageLinux(codepage));
#endif
        delete cpg;

        SetData (index, bIsNull, chars);
        // for consistency, we copy the string to the wide character buffer too
        // although this is not strictly needed
        wcscpy (mStrings[index], value);
    }
}

/// <summary>Sets the string data.</summary>
/// <param name="index">Input the column index of the data to set.</param> 
/// <param name="value">Input the value for the column.</param> 
/// <returns>Returns nothing.</returns> 
void RowData::SetData (int index, bool bIsNull, CHAR* value)
{
    int width;
    char* raw;
    size_t length;

    if (kColumnCharType != mColumnInfo->GetColumnTypeAt (index))
        throw FdoException::Create (NlsMsgGet(SHP_ILLEGAL_COLUMN_ASSIGNMENT, "Illegal column assignment."));
    raw = ((char*)mBuffer) + mColumnInfo->GetColumnOffsetAt (index);
    width = mColumnInfo->GetColumnWidthAt (index);
    if (bIsNull || (NULL == value))
        memset (raw, ' ', width);
    else
    {
        length = strlen (value);

        // Validate length:
        if (length > (unsigned int)width)
        {
            wchar_t* wValue = NULL;
            multibyte_to_wide(wValue, value);
            throw FdoException::Create(NlsMsgGet(SHP_VALUE_TOO_WIDE_FOR_COLUMN, "The value '%1$ls' is too wide for column '%2$ls'.",
                wValue, mColumnInfo->GetColumnNameAt(index)));
        }

        strncpy ((char*)raw, value, length);
        memset (raw + length, ' ', width - length);
    }
}

/// <summary>Sets the date data.</summary>
/// <param name="index">Input the column index of the data to set.</param> 
/// <param name="value">Input the value for the column.</param> 
/// <returns>Returns nothing.</returns> 
void RowData::SetData (int index, bool bIsNull, Date value)
{
    char buffer[50];
    int width;
    char* raw;

    if (kColumnDateType != mColumnInfo->GetColumnTypeAt (index))
        throw FdoException::Create (NlsMsgGet(SHP_ILLEGAL_COLUMN_ASSIGNMENT, "Illegal column assignment."));
    width = mColumnInfo->GetColumnWidthAt (index);
    raw = ((char*)mBuffer) + mColumnInfo->GetColumnOffsetAt (index);

    if (bIsNull)
        memset(raw, ' ', width);
    else
    {
        sprintf (buffer, "%04d%02d%02d", value.nYear, value.nMonth, value.nDay);
        strncpy (raw, buffer, 8);
    }
}

/// <summary>Sets the logical (boolean) data.</summary>
/// <param name="index">Input the column index of the data to set.</param> 
/// <param name="value">Input the value for the column.</param> 
/// <returns>Returns nothing.</returns> 
void RowData::SetData (int index, bool bIsNull, bool value)
{
    char* raw;

    if (kColumnLogicalType != mColumnInfo->GetColumnTypeAt (index))
        throw FdoException::Create (NlsMsgGet(SHP_ILLEGAL_COLUMN_ASSIGNMENT, "Illegal column assignment."));
    raw = ((char*)mBuffer) + mColumnInfo->GetColumnOffsetAt (index);

    if (bIsNull)
        *raw = '?';  // special DBF character indicating "unset" fo boolean values
    else
        *raw = (value ? 'Y' : 'N');
}

/// <summary>Gets the data for a column.</summary>
/// <param name="data">Input a pointer to where to put the data.</param> 
/// <param name="index">Input the column index of the data to get.</param> 
/// <param name="type">The column type for the column.</param> 
/// <returns>Returns nothing.</returns> 
void RowData::GetData (ColumnData* data, int index, eDBFColumnType type, WCHAR* codepage)
{
    int width;
    int offset;
    char* raw;
    char temp;
    char* p;
    wchar_t* str;
    char c;

    width = mColumnInfo->GetColumnWidthAt (index);
    offset = mColumnInfo->GetColumnOffsetAt (index);
    raw = &((char*)mBuffer)[offset];
    temp = *(raw + width);
    *(raw + width) = '\0';
    switch (type)
    {
        case kColumnCharType:
            p = raw + width;
            while (iswspace (*(p - 1)) && ((p - 1) >= raw))
                p--;
            data->bIsNull = (p==raw);
            if (!data->bIsNull)
            {
                *p = '\0';

                ShapeCPG    *cpg = new ShapeCPG();
#ifdef _WIN32
                multibyte_to_wide_cpg (str, raw, cpg->ConvertCodePageWin(codepage));
#else
                multibyte_to_wide_cpg (str, raw, cpg->ConvertCodePageLinux(codepage));
#endif
                delete cpg;

                *p = szSPACE;
                wcscpy (mStrings[index], str);
                data->value.wszData = mStrings[index];
            }
            break;

        case kColumnDecimalType:
            p = raw + width;
            while (iswspace (*(p - 1)) && ((p - 1) >= raw))
                p--;
            data->bIsNull = (p==raw);
            if (!data->bIsNull)
			{
                data->value.dData = atof((const char*)raw);

                // A) In case the number is in scientific format (containing 'E' or 'e') no extra processing
                // is required.

				// B) In case the locale is using ',' as decimal separator, then atof() truncates
				// the number to its integer value. Note errno doesn't seem to be reliable since it returns
				// ERANGE even in valid cases. The code below will do a conversion on the same
				// string with '.' replaced by ',' then the largest value (that is, not truncated) 
				// will be selected for output. 

				char	buffer[50];
				size_t	length = strlen((const char*)raw);
				memcpy(buffer, (const char*)raw, length);
				buffer[length] = '\0';

                bool isScientific = false;
                char *p = &buffer[length - 1];

                // Check for scientific format
                while (p > buffer && !isScientific)
				{
					isScientific = (*p == 'E' || *p == 'e');
					p--;
				}

                if (!isScientific)
                {
				    char *p = &buffer[length - 1];
				    while (p > buffer)
				    {
					    if (*p == '.')
					    {
						    *p = ',';
						    break;
					    }
					    p--;
				    }

				    double value = atof(buffer);
    				
				    // Select the not truncated value.
				    if (fabs(value) > fabs(data->value.dData))
					    data->value.dData = value;
                }
			}
            break;

        case kColumnDateType:
            p = raw + width;
            while (iswspace (*(p - 1)) && ((p - 1) >= raw))
                p--;
            data->bIsNull = (p==raw);
            if (!data->bIsNull)
            {
                // parse YYYYMMDD format
                c = *(raw + 4);
                *(raw + 4) = '\0';
                data->value.tData.nYear = atoi ((const char*)raw);
                *(raw + 4) = c;
                c = *(raw + 6);
                *(raw + 6) = '\0';
                data->value.tData.nMonth = atoi ((const char*)(raw + 4));
                *(raw + 6) = c;
                data->value.tData.nDay = atoi ((const char*)(raw + 6));
            }
            break;

        case kColumnLogicalType:
            c = *((const char*)raw);
            switch (c)
            {
                case 'Y':
                case 'y':
                case 'T':
                case 't':
                    data->bIsNull = false;
                    data->value.bData = true;
                    break;
                case 'N':
                case 'n':
                case 'F':
                case 'f':
                    data->bIsNull = false;
                    data->value.bData = false;
                    break;
                case '?':
                default:
                    data->bIsNull = true;
                    data->value.bData = false;
                    break;
            }
            break;

        case kColumnUnsupportedType:
        default:
            throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_DATATYPE, "The '%1$ls' data type is not supported by Shp.", type));
            break;
    }
    *(raw + width) = temp;
}
