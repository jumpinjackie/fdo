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
#include "ColumnInfo.h"
#include "Constants.h"

/*****************************************************************************
 * Name         : ColumnInfo
 *
 * Description  : Constructor
 *
 * Parameters   : int nColumns
 *
 * Return       : N/A
 *
 * Notes        : N/A
 *
 *****************************************************************************/
ColumnInfo::ColumnInfo (int nColumns)
{
    char* p;

    m_nNumColumns = nColumns;

    p = (char*)this + sizeof (ColumnInfo);
    m_pColumnWidth = (int*)p;
    p += (m_nNumColumns * sizeof (int));
    m_pColumnOffset = (int*)p;
    p += (m_nNumColumns * sizeof (int));
    m_pColumnScale = (int*)p;
    p += (m_nNumColumns * sizeof (int));
    m_pColumnType = (eDBFColumnType*)p;
    p += (m_nNumColumns * sizeof (eDBFColumnType));
    m_pwszColumnNames = (WCHAR**)p;
    p += (m_nNumColumns * sizeof (WCHAR*));
    for (int i = 0; i < m_nNumColumns; i++)
    {
        m_pColumnWidth[i] = 0;
        m_pColumnOffset[i] = 0;
        m_pColumnScale[i] = 0;
        m_pColumnType[i] = kColumnUnsupportedType;
        m_pwszColumnNames[i] = (WCHAR*)p;
        *p = L'\0';
        p += (sizeof (WCHAR) * (nDBF_COLNAME_LENGTH + 1));
    }
}

ColumnInfo::ColumnInfo (const ColumnInfo &right)
{
    char* p;

    m_nNumColumns = right.m_nNumColumns;
    p = (char*)this + sizeof (ColumnInfo);
    m_pColumnWidth = (int*)p;
    p += (m_nNumColumns * sizeof (int));
    m_pColumnOffset = (int*)p;
    p += (m_nNumColumns * sizeof (int));
    m_pColumnScale = (int*)p;
    p += (m_nNumColumns * sizeof (int));
    m_pColumnType = (eDBFColumnType*)p;
    p += (m_nNumColumns * sizeof (eDBFColumnType));
    m_pwszColumnNames = (WCHAR**)p;
    p += (m_nNumColumns * sizeof (WCHAR*));
    for (int i = 0; i < m_nNumColumns; i++)
    {
        m_pColumnWidth[i] = right.m_pColumnWidth[i];
        m_pColumnOffset[i] = right.m_pColumnOffset[i];
        m_pColumnScale[i] = right.m_pColumnScale[i];
        m_pColumnType[i] = right.m_pColumnType[i];
        m_pwszColumnNames[i] = (WCHAR*)p;
        wcscpy (m_pwszColumnNames[i], right.m_pwszColumnNames[i]);
        p += (sizeof (WCHAR) * (nDBF_COLNAME_LENGTH + 1));
    }
}

void* ColumnInfo::operator new (size_t nSize, int nColumns)
{
    size_t size;
    void *ret;

    size = nSize
        + (nColumns * sizeof (int))             // column widths
        + (nColumns * sizeof (int))             // column offset
        + (nColumns * sizeof (int))             // column scales
        + (nColumns * sizeof (eDBFColumnType))  // column types
        + (nColumns * sizeof (WCHAR*))          // column name pointers
        + (nColumns * sizeof (WCHAR) * (nDBF_COLNAME_LENGTH + 1));  // column names
    ret = ::new char [size];

    return (ret);
}

void ColumnInfo::operator delete (void *pObj)
{
    delete [] (char *)pObj;
}

void ColumnInfo::operator delete (void *pObj, int nColumns)
{
    delete [] (char *)pObj;
}

void ColumnInfo::operator delete (void *pObj, const ColumnInfo &right)
{
    delete [] (char *)pObj;
}

/*****************************************************************************
 * Name         : ~ColumnInfo
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
ColumnInfo::~ColumnInfo()
{
}

