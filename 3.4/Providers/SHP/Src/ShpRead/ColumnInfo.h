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

#ifndef COLUMNINFO_H
#define COLUMNINFO_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <malloc.h>
#include <wchar.h>
#include <../Message/Inc/ShpMessage.h>

// ***************************************************************************
// * E N U M E R A T I O N S
// ***************************************************************************
enum eDBFColumnType
{
    kColumnUnsupportedType = 0,
    kColumnCharType,
    kColumnDecimalType,
    kColumnDateType,
    kColumnLogicalType,
};

class ColumnInfo
{
private:
    int m_nNumColumns;
    int* m_pColumnWidth;
    int* m_pColumnOffset;
    int* m_pColumnScale;
    eDBFColumnType* m_pColumnType;
    WCHAR** m_pwszColumnNames;

public:
    ColumnInfo (int nColumns);
    ColumnInfo (const ColumnInfo &right); // copy constructor
    void* operator new (size_t nSize, int nColumns);
    void operator delete (void *pObj);
    void operator delete (void *pObj, int nColumns);
    void operator delete (void *pObj, const ColumnInfo &right);
    virtual ~ColumnInfo();

    const int GetNumColumns()               { return m_nNumColumns; }
    const int* GetColumnWidths()            { return m_pColumnWidth; }
    const int* GetColumnOffsets()           { return m_pColumnOffset; }
    const int* GetColumnScales()            { return m_pColumnScale; }
    const eDBFColumnType* GetColumnTypes()  { return m_pColumnType; }
    const WCHAR** GetColumnNames()          { return (const WCHAR**)m_pwszColumnNames; }

    /*****************************************************************************
     * Name         : SetColumnType
     *
     * Description  : This method sets the column type for the specified column.
     *
     * Parameters   : int            nIndex
     *                eDBFColumnType eType
     *
     * Return       : N/A
     *
     * Notes        : N/A
     *
     *****************************************************************************/
    void SetColumnType(int nIndex, eDBFColumnType eType)
    {
        if((nIndex >= 0) && (nIndex < m_nNumColumns))
        {
            m_pColumnType[nIndex] = eType;
        }
    }

    /*****************************************************************************
     * Name         : SetColumnWidth
     *
     * Description  : This method sets the column width for the specified column.
     *
     * Parameters   : int nIndex
     *                int nWidth
     *
     * Return       : N/A
     *
     * Notes        : N/A
     *
     *****************************************************************************/
    void SetColumnWidth(int nIndex, int nWidth)
    {
        if((nIndex >= 0) && (nIndex < m_nNumColumns))
        {
            if ((nWidth < 0) || (nWidth > 255))
                throw FdoException::Create(NlsMsgGet(SHP_COLUMN_WIDTH_INVALID, "The value '%1$d' is invalid for the column width (must be between 0 and 255).", nWidth));

            m_pColumnWidth[nIndex] = nWidth;
        }
    }

    void SetColumnOffset(int nIndex, int nOffset)
    {
        if((nIndex >= 0) && (nIndex < m_nNumColumns))
        {
            m_pColumnOffset[nIndex] = nOffset;
        }
    }

    /*****************************************************************************
     * Name         : SetColumnScale
     *
     * Description  : This method sets the column scale (i.e. number of digits to the right of the decimal point)
     *                for the specified column.
     *
     * Parameters   : int nIndex
     *                int nWidth
     *
     * Return       : N/A
     *
     * Notes        : Column scales only apply to columns of type kColumnDecimalType
     *
     *****************************************************************************/
    void SetColumnScale(int nIndex, int nScale)
    {
        if((nIndex >= 0) && (nIndex < m_nNumColumns))
        {
            if ((nScale < 0) || (nScale > 255))
                throw FdoException::Create(NlsMsgGet(SHP_COLUMN_SCALE_INVALID, "The value '%1$d' is invalid for the column scale (must be between 0 and 255).", nScale));

            m_pColumnScale[nIndex] = nScale;
        }
    }

    /*****************************************************************************
     * Name         : SetColumnName
     *
     * Description  : This method sets the column name for the specified column.
     *
     * Parameters   : int    nIndex
     *                WCHAR* wszColumnName
     *
     * Return       : N/A
     *
     * Notes        : N/A
     *
     *****************************************************************************/
    void SetColumnName(int nIndex, WCHAR* wszColumnName)
    {
        if(wszColumnName)
        {
            if((nIndex >= 0) && (nIndex < m_nNumColumns))
            {
                wcscpy(m_pwszColumnNames[nIndex],wszColumnName);
            }
        }
    }

    /*****************************************************************************
     * Name         : GetColumnWidthAt
     *
     * Description  : This method gets the column width for the specified column.
     *
     * Parameters   : int nIndex
     *
     * Return       : int
     *
     * Notes        : N/A
     *
     *****************************************************************************/
    int GetColumnWidthAt(int nIndex)
    {
        int nWidth = 0;

        if((nIndex >= 0) && (nIndex < m_nNumColumns))
        {
            nWidth = m_pColumnWidth[nIndex];
        }

        return nWidth;
    }

    int GetColumnOffsetAt(int nIndex)
    {
        int nOffset = 0;

        if((nIndex >= 0) && (nIndex < m_nNumColumns))
        {
            nOffset = m_pColumnOffset[nIndex];
        }

        return nOffset;
    }

    /*****************************************************************************
     * Name         : GetColumnScaleAt
     *
     * Description  : This method gets the column scale (i.e. number of digits at right of decimal point)
     *                for the specified column.
     *
     * Parameters   : int nIndex
     *
     * Return       : int
     *
     * Notes        : Column scales only apply to kColumnDecimalType columns.
     *
     *****************************************************************************/
    int GetColumnScaleAt(int nIndex)
    {
        int nScale = 0;

        if((nIndex >= 0) && (nIndex < m_nNumColumns))
        {
            nScale = m_pColumnScale[nIndex];
        }

        return nScale;
    }


    /*****************************************************************************
     * Name         : GetColumnTypeAt
     *
     * Description  : This method gets the column type for the specified column.
     *
     * Parameters   : int nIndex
     *
     * Return       : eDBFColumnType
     *
     * Notes        : N/A
     *
     *****************************************************************************/
    eDBFColumnType GetColumnTypeAt(int nIndex)
    {
        eDBFColumnType eType = kColumnUnsupportedType;

        if((nIndex >= 0) && (nIndex < m_nNumColumns))
        {
            eType = m_pColumnType[nIndex];
        }

        return eType;
    }

    /*****************************************************************************
     * Name         : GetColumnNameAt
     *
     * Description  : This method gets the column name for the specified column.
     *
     * Parameters   : int nIndex
     *
     * Return       : WCHAR*
     *
     * Notes        : N/A
     *
     *****************************************************************************/
    WCHAR* GetColumnNameAt(int nIndex)
    {
        WCHAR* wszColumnName = NULL;

        if((nIndex >= 0) && (nIndex < m_nNumColumns))
        {
            wszColumnName = m_pwszColumnNames[nIndex];
        }

        return wszColumnName;
    }
};

#endif // COLUMNINFO_H

