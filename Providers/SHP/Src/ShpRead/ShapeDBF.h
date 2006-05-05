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

#ifndef SHAPEDBF_H
#define SHAPEDBF_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ColumnInfo.h"
#include <FdoCommonFile.h>
#include "Constants.h"

class RowData;

typedef struct {
    BYTE cDBFFormat;
    BYTE cUpdateDate[3];
    int nRecords;
    WORD wHeaderSize;
    WORD wRecordSize;
    BYTE cReserved1[3];
    BYTE cReserved2[13];
    BYTE cReserved3[4];
} DBFHeader;

typedef struct {
    BYTE cFieldName[11];
    BYTE cFieldType;
    BYTE cFieldDataAddress[4];
    BYTE cFieldLength;
    BYTE cFieldDecimalCount;
    BYTE cReserved1[2];
    BYTE cWorkAreaID;
    BYTE cReserved2[2];
    BYTE cSetFieldsFlag;
    BYTE cReserved3[8];
} TableFieldDescriptor;


class ShapeDBF : public FdoCommonFile
{
    // Member data
    DBFHeader m_DBFHeader;
    ColumnInfo* m_pColumnInfo;
    int m_nRecordStart;
 //   bool mReadOnly;
    bool mHeaderDirty;

    // Cache Info
    BYTE* m_szRowBuffer;
    int m_nFirstCacheRecordId;
    int m_nLastCacheRecordId;

public:
    ShapeDBF (const WCHAR* wszFilename);
    ShapeDBF (const WCHAR* wszFilename, ColumnInfo* info);

	void Reopen(OpenFlags openFlags);

    virtual ~ShapeDBF();

 //   bool isReadOnly ()                  { return (mReadOnly); }
    ColumnInfo* GetColumnInfo()        { return m_pColumnInfo; }
    int GetNumRecords()                 { return m_DBFHeader.nRecords; }
    int GetRecordSize()                 { return m_DBFHeader.wRecordSize; }

    RowData* NewRow (void* buffer = NULL);
    ColumnInfo* NewColumnInfo ();
    static ColumnInfo* NewColumnInfo (int nColumns);
    ColumnInfo* NewColumnInfo (ColumnInfo* info);
    RowData* GetRowAt(int nRecord);
    void SetRowAt (RowData*data, int nRecord, bool batch = false);
    void DeleteRowAt (int nRecord);
    void PutFileHeaderDetails ();
    void SetHeaderDirty (bool dirty) { mHeaderDirty = dirty; }
    bool IsHeaderDirty ()            { return (mHeaderDirty); }

private:
    void GetFileHeaderDetails (int& nColumns);
    void CheckDBFFormat ();
    bool WriteColumnDef (ColumnInfo* info, int column);
    RowData* CreateNewDataRowFromBuffer (BYTE* szRowBuffer);
    RowData* GetRowDataFromCache (int nRecord);
};

#endif // SHAPEDBF_H

