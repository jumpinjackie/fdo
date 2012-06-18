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

#ifndef SHAPEINDEX_H
#define SHAPEINDEX_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "Shape.h"
#include "ShapeFileBase.h"
#include "ShapeFile.h"
#include "Constants.h"

class ShapeIndex : public ShapeFileBase
{
public:
    ShapeIndex (const WCHAR* wszFilename);
    ShapeIndex (const WCHAR* wszFilename, ShapeFile* shp, FdoString *tempDir = NULL);
    virtual ~ShapeIndex();

    // Data access methods
    // note, these offsets and lengths are bytes not words
    void GetObjectAt (int nObjectNum, ULONG& nOffset, int& nContentLength);
    void SetObjectAt (int nObjectNum, ULONG nOffset, int nContentLength, bool batch = false);
    int GetNumObjects ();

protected:
    // note, these offsets and lengths are words not bytes
    void ReadRecordHeader (ULONG ulIndexOffset, ULONG& nOffset, int& nContentLength);
    void WriteRecordHeader (ULONG ulIndexOffset, ULONG nOffset, int nContentLength);

private:
    // Cache Info
    SHPIndexRecordHeader    m_szRowBuffer[SHP_FILE_READ_CACHE_SIZE];
    int                     m_nFirstCacheRecordId;
    int                     m_nLastCacheRecordId;

private:
    bool GetRowIndexFromCache(int nRecord, ULONG& nOffset, int& nContentLength);
    void ClearRowIndexCache();
};

#endif // SHAPEINDEX_H

