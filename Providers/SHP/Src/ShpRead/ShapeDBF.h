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
 * Revision Control Modification History
 *
 *         $Id: //providers_reliant_sp2/Shp/src/ShpRead/ShapeDBF.h#1 $
 *     $Author: lee $
 *   $DateTime: 2006/06/02 11:06:52 $
 *     $Change: 13046 $
 *
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
    BYTE cReserved1[2];		// 12-13	| Reserved
	BYTE cTransaction[1];	// 14		| Flag indicating incomplete transaction
	BYTE cEncryption[1];	// 15		| Encryption flag
    BYTE cReserved2[12];	// 16-27	| Reserved for dBASE IV
	BYTE cMDX[1];			// 28		| Production MDX flag
	BYTE cLDID[1];			// 29		| Language driver ID
    BYTE cReserved3[2];		// 30-31	| Reserved; fill with 0.
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

typedef struct {
	BYTE	cLDID;	
	int		codePage;
} EsriCodePageMap;

// From <www.esrinl.com/content/file.asp?id=307>
// (*) Secondary codepage
static EsriCodePageMap EsriCodePageMapper[] = {
	{0x01, 437},	// U.S. MS–DOS
	{0x02, 850},	// International MS–DOS
	{0x03, 1252},	// Windows ANSI
	{0x08, 865},	// Danish OEM
	{0x09, 437},	// Dutch OEM
	{0x0A, 850},	// Dutch OEM*
	{0x0B, 437},	// Finnish OEM
	{0x0D, 437},	// French OEM
	{0x0E, 850},	// French OEM*
	{0x0F, 437},	// German OEM
	{0x10, 850},	// German OEM*
	{0x11, 437},	// Italian OEM
	{0x12, 850},	// Italian OEM*
	{0x13, 932},	// Japanese Shift-JIS
	{0x14, 850},	// Spanish OEM*
	{0x15, 437},	// Swedish OEM
	{0x16, 850},	// Swedish OEM*
	{0x17, 865},	// Norwegian OEM
	{0x18, 437},	// Spanish OEM
	{0x19, 437},	// English OEM (Britain)
	{0x1A, 850},	// English OEM (Britain)*
	{0x1B, 437},	// English OEM (U.S.)
	{0x1C, 863},	// French OEM (Canada)
	{0x1D, 850},	// French OEM*
	{0x1F, 852},	// Czech OEM
	{0x22, 852},	// Hungarian OEM
	{0x23, 852},	// Polish OEM
	{0x24, 860},	// Portuguese OEM
	{0x25, 850},	// Portuguese OEM*
	{0x26, 866},	// Russian OEM
	{0x37, 850},	// English OEM (U.S.)*
	{0x40, 852},	// Romanian OEM
	{0x4D, 936},	// Chinese GBK (PRC)
	{0x4E, 949},	// Korean (ANSI/OEM)
	{0x4F, 950},	// Chinese Big5 (Taiwan)
	{0x50, 874},	// Thai (ANSI/OEM)
	{0x57, 1252},	// ANSI
	{0x58, 1252},	// Western European ANSI
	{0x59, 1252},	// Spanish ANSI
	{0x64, 852},	// Eastern European MS–DOS
	{0x65, 866},	// Russian MS–DOS
	{0x66, 865},	// Nordic MS–DOS
	{0x67, 861},	// Icelandic MS–DOS
	{0x6A, 737},	// Greek MS–DOS (437G)
	{0x6B, 857},	// Turkish MS–DOS
	{0x6C, 863},	// French–Canadian MS–DOS
	{0x78, 950},	// Taiwan Big 5
	{0x79, 949},	// Hangul (Wansung)
	{0x7A, 936},	// PRC GBK
	{0x7B, 932},	// Japanese Shift-JIS
	{0x7C, 874},	// Thai Windows/MS–DOS
	{0x86, 737},	// Greek OEM
	{0x87, 852},	// Slovenian OEM
	{0x88, 857},	// Turkish OEM
	{0xC8, 1250},	// Eastern European Windows
	{0xC9, 1251},	// Russian Windows
	{0xCA, 1254},	// Turkish Windows
	{0xCB, 1253},	// Greek Windows
	{0xCC, 1257}	// Baltic Windows
};

class ShapeDBF : public FdoCommonFile
{
private:
    // Member data
    DBFHeader m_DBFHeader;
    ColumnInfo* m_pColumnInfo;
    int m_nRecordStart;
    bool mHeaderDirty;

    // Cache Info
    BYTE* m_szRowBuffer;
    int m_nFirstCacheRecordId;
    int m_nLastCacheRecordId;

	FdoStringP  mCodePageESRI;

public:
    ShapeDBF (const WCHAR* wszFilename, FdoString* codepageCPG = L"");
    ShapeDBF (const WCHAR* wszFilename, ColumnInfo* info, BYTE ldid = 0);

	void Reopen(OpenFlags openFlags);

    virtual ~ShapeDBF();

    ColumnInfo* GetColumnInfo()        { return m_pColumnInfo; }
    int GetNumRecords()                 { return m_DBFHeader.nRecords; }
    int GetRecordSize()                 { return m_DBFHeader.wRecordSize; }
	BYTE GetLDID()						{ return m_DBFHeader.cLDID[0]; }

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

	FdoStringP GetCodePage();

private:
    void GetFileHeaderDetails (int& nColumns);
    void CheckDBFFormat ();
    bool WriteColumnDef (ColumnInfo* info, int column);
    RowData* CreateNewDataRowFromBuffer (BYTE* szRowBuffer);
    RowData* GetRowDataFromCache (int nRecord);
	BYTE GetLDIDFromLocale();
	void SetCodePage();
};

#endif // SHAPEDBF_H

