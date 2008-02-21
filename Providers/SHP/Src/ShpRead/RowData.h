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

#ifndef ROWDATA_H
#define ROWDATA_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ColumnInfo.h"

typedef
    struct
    {
        int nYear;
        int nMonth;
        int nDay;
    } Date;


struct ColumnData
{
    union
    {
        int nData;
        double dData;
        wchar_t* wszData;
        Date tData;
        bool bData;
    } value;

    bool bIsNull;
};

class RowData
{
    friend class ShapeDBF;

protected:
    bool mDeleted;
    ColumnInfo* mColumnInfo;
    wchar_t** mStrings; // pointers to wide char equivalents of kColumnCharType data
    void* mBuffer; // file i/o buffer
    bool mReading;

public:
    RowData (ColumnInfo* pColumnInfo, void* buffer);
    void* operator new (size_t nSize, ColumnInfo* pColumnInfo, void* buffer);
    void operator delete (void *pObj);
    void operator delete (void *pObj, ColumnInfo* pColumnInfo, void* buffer);
    virtual ~RowData ();

    bool IsDeleted ()               { return (mDeleted); }
    void SetDeleted (bool deleted)  { mDeleted = deleted; }
    ColumnInfo* GetColumnInfo ()    { return (mColumnInfo); }
    int GetNumColumns ()            { return (mColumnInfo->GetNumColumns ()); }

    /// <summary>Sets the decimal data.</summary>
    /// <param name="index">Input the column index of the data to set.</param> 
    /// <param name="value">Input the value for the column.</param> 
    /// <returns>Returns nothing.</returns> 
    void SetData (int index, bool bIsNull, double value);

    /// <summary>Sets the string data.</summary>
    /// <param name="index">Input the column index of the data to set.</param> 
    /// <param name="value">Input the value for the column.</param> 
    /// <returns>Returns nothing.</returns> 
    void SetData (int index, bool bIsNull, WCHAR* value, WCHAR* codepage = NULL);

    /// <summary>Sets the string data.</summary>
    /// <param name="index">Input the column index of the data to set.</param> 
    /// <param name="value">Input the value for the column.</param> 
    /// <returns>Returns nothing.</returns> 
    void SetData (int index, bool bIsNull, CHAR* value);

    /// <summary>Sets the date data.</summary>
    /// <param name="index">Input the column index of the data to set.</param> 
    /// <param name="value">Input the value for the column.</param> 
    /// <returns>Returns nothing.</returns> 
    void SetData (int index, bool bIsNull, Date value);

    /// <summary>Sets the logical (boolean) data.</summary>
    /// <param name="index">Input the column index of the data to set.</param> 
    /// <param name="value">Input the value for the column.</param> 
    /// <returns>Returns nothing.</returns> 
    void SetData (int index, bool bIsNull, bool value);

    /// <summary>Gets the data for a column.</summary>
    /// <param name="data">Input a pointer to where to put the data.</param> 
    /// <param name="index">Input the column index of the data to get.</param> 
    /// <param name="type">The column type for the column.</param> 
    /// <returns>Returns nothing.</returns> 
    void GetData (ColumnData* data, int index, eDBFColumnType type, WCHAR* codepage = NULL);
};

#endif // ROWDATA_H

