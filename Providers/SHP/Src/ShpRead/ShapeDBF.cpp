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
 * Revision Control Modification History
 *
 *         $Id: //providers_reliant_sp2/Shp/src/ShpRead/ShapeDBF.cpp#1 $
 *     $Author: lee $
 *   $DateTime: 2006/06/02 11:06:52 $
 *     $Change: 13046 $
 *
 */

#include "stdafx.h"
#include "ShapeDBF.h"
#include "Constants.h"
#include "ColumnInfo.h"
#include "RowData.h"

#include <FdoCommonOSUtil.h>

#include <wctype.h>
#include <time.h>
#include <locale.h>

static ColumnInfo NoColumns (0);
/**
 * Remove blanks from the two ends of a string.
 */
static wchar_t* trim (wchar_t *str)
{
    wchar_t *p;
    wchar_t *q;

    p = str;
    q = str;
    while (iswspace (*p))
        p++;
    if (p != str)
    {
        while ('\0' != *p)
            *q++ = *p++;
        *q = *p;
    }
    else
        while ('\0' != *q)
            q++;
    // q now at terminating 0 of str
    if (q > str)
        while (iswspace (*(q - 1)) && ((q - 1) >= str))
            q--;
    *q = '\0';

    return (str);
}

/*****************************************************************************
 * Name         : ShapeDBF
 *
 * Description  : Constructor
 *
 * Parameters   : const WCHAR* name
 *                int&         nStatus
 *
 * Return       : N/A
 *
 * Notes        : N/A
 *
 *****************************************************************************/
ShapeDBF::ShapeDBF (const WCHAR* name, FdoString* codepageCPG) :
    m_pColumnInfo (&NoColumns),
    m_nRecordStart (0),
    mHeaderDirty (false),
    m_szRowBuffer (NULL),
    m_nFirstCacheRecordId (0),
    m_nLastCacheRecordId (-1)
{
    ErrorCode status;
    int nNumColumns;
    int coloff;
    eDBFColumnType eType;
    unsigned long offset;

    _FDORPT0(0, "Creating ShapeDBF object\n");

    // Clear the DBF Header info
    memset(&m_DBFHeader,0,sizeof(DBFHeader));

    if (OpenFile (name, IDF_OPEN_READ, status))
    {
        // Read header info
        GetFileHeaderDetails (nNumColumns);
        // Initialize column info
        m_pColumnInfo = NewColumnInfo (nNumColumns);
        // Read the column info
        TableFieldDescriptor* pTableFieldDescriptorArray = new TableFieldDescriptor[nNumColumns];
        if (!ReadFile (pTableFieldDescriptorArray, sizeof(TableFieldDescriptor) * nNumColumns))
        {
            FdoException*  ex = FdoCommonFile::LastErrorToException (L"ShapeDBF::ShapeDBF(ReadTableFieldDescriptorArray)");
            if ( ex )
			    throw ex;
		    else
			    throw FdoException::Create (NlsMsgGet(SHP_READ_FILE_ERROR, "Error occured reading file '%1$ls'.", FileName() ));
        }

		// Get the OEM code page either from the header or CPG
		FdoStringP	codepageESRI = (mCodePageESRI == L"") ? codepageCPG : (FdoString *)mCodePageESRI;

#ifdef _WIN32
		ULONG codePage = RowData::ConvertCodePageWin((WCHAR*)(FdoString *)codepageESRI);
#else
		// This doesn't compile because ConvertCodePageLinux() is static. TO BE FIXED.
//		const char* codePage = RowData::ConvertCodePageLinux((WCHAR*)(FdoString *)codepageESRI);
		const char* codePage = "";
#endif

        // Loop through the columns
        coloff = 1;
        for(int i = 0; i < nNumColumns; i++)
        {
            // Now we will have to convert the OEM characters in the DBF file to
            // ANSI or Wide character (UNICODE) string.
            char name[nDBF_COLNAME_LENGTH+1];
            WCHAR* wszColumnName;
            strncpy (name, (char*)pTableFieldDescriptorArray[i].cFieldName, nDBF_COLNAME_LENGTH);
            name[nDBF_COLNAME_LENGTH] = '\0';

#ifdef _WIN32
			multibyte_to_wide_cpg (wszColumnName, name, codePage);
#else
			// TO BE REMOVED when ConvertCodePageLinux() is fixed.
			multibyte_to_wide(wszColumnName, name);
#endif
            // Trim trailing and leading spaces and tabs
            trim (wszColumnName);

            switch (pTableFieldDescriptorArray[i].cFieldType)
            {
                case 'C':
                case 'c':
                    eType = kColumnCharType;
                    break;
                case 'D':
                case 'd':
                    eType = kColumnDateType;
                    break;
                case 'L':
                case 'l':
                    eType = kColumnLogicalType;
                    break;
                default:
                    eType = kColumnDecimalType;
                    break;
            }

            // Save column info
            m_pColumnInfo->SetColumnType(i,eType);
            m_pColumnInfo->SetColumnName(i,wszColumnName);
            m_pColumnInfo->SetColumnWidth(i,pTableFieldDescriptorArray[i].cFieldLength);
            m_pColumnInfo->SetColumnOffset(i, coloff);
            coloff += pTableFieldDescriptorArray[i].cFieldLength;
            m_pColumnInfo->SetColumnScale(i,pTableFieldDescriptorArray[i].cFieldDecimalCount);

            _FDORPT4(0, "Column: %d  Name:'%s'  Type:%d  Length:%d\n",i,wszColumnName,eType,pTableFieldDescriptorArray[i].cFieldLength);
        }

        // Save current file pointer location
		FdoInt64   offset64;

        if (!GetFilePointer64 (offset64))
            throw FdoCommonFile::LastErrorToException (L"ShapeDBF::ShapeDBF(GetFilePointer64)");

		offset = (ULONG)offset64;
        m_nRecordStart = (int)offset + 1;  // Need to skip ODh end of header bytes

        delete [] pTableFieldDescriptorArray;
        pTableFieldDescriptorArray = NULL;
    }
    else
        throw FdoCommonFile::ErrorCodeToException (status, name, IDF_OPEN_READ);
}

ShapeDBF::ShapeDBF (const WCHAR* name, ColumnInfo* info, BYTE ldid) :
    m_pColumnInfo (NewColumnInfo (info)),
    m_nRecordStart (0),
    mHeaderDirty (false),
    m_szRowBuffer (NULL),
    m_nFirstCacheRecordId (0),
    m_nLastCacheRecordId (-1)
{
    char byte[2];
    ErrorCode status;

    _FDORPT0(0, "Creating new ShapeDBF object\n");

    // Clear the DBF Header info
    memset (&m_DBFHeader, 0, sizeof(DBFHeader));

    try
    {
        if (OpenFile (name, (OpenFlags)(IDF_CREATE_NEW | IDF_OPEN_UPDATE), status))
        {
            m_DBFHeader.cDBFFormat = cDBASE_III_PLUS_NO_MEMO;
            SetHeaderDirty (true);
            m_DBFHeader.nRecords = 0;
            int count = info->GetNumColumns ();
            m_DBFHeader.wHeaderSize = (WORD)(sizeof (DBFHeader) + count * sizeof (TableFieldDescriptor) + 1);
            int size = 1; // deleted flag
            for (int i = 0; i < count; i++)
                size += info->GetColumnWidthAt (i);
            m_DBFHeader.wRecordSize = size;

			m_DBFHeader.cLDID[0] = (ldid == 0)? GetLDIDFromLocale() : ldid;

            PutFileHeaderDetails ();
            // write the fields
            for (int i = 0; i < count; i++)
                if (!WriteColumnDef (info, i))
                    throw FdoCommonFile::LastErrorToException (L"ShapeDBF::ShapeDBF(WriteField)");
            byte[0] = END_OF_HEADER;
            byte[1] = END_OF_FILE;
            if (!WriteFile (byte, sizeof (byte)))
                throw FdoCommonFile::LastErrorToException (L"ShapeDBF::ShapeDBF(WriteEOF)");
            CloseFile ();
        }
        else
            throw FdoCommonFile::ErrorCodeToException (status, name, (OpenFlags)(IDF_CREATE_NEW | IDF_OPEN_UPDATE));
    }
    catch (...)
    {
        delete m_pColumnInfo;
        throw;
    }
}

void ShapeDBF::Reopen(OpenFlags openFlags)
{
    ErrorCode status;

    _FDORPT0(0, "Reopening a DBF file without recreating the object\n");

	if (!OpenFile( FileName(), openFlags, status))
        throw FdoCommonFile::ErrorCodeToException (status, FileName(), openFlags);
}

bool ShapeDBF::WriteColumnDef (ColumnInfo* info, int column)
{
    TableFieldDescriptor descriptor;
    char* name;

    memset (&descriptor, 0, sizeof(TableFieldDescriptor));
    wide_to_multibyte (name, info->GetColumnNameAt (column));
    strncpy ((char*)descriptor.cFieldName, name, strlen (name));
    switch (info->GetColumnTypeAt (column))
    {
        case kColumnCharType:
            descriptor.cFieldType = 'C';
            break;
        case kColumnDateType:
            descriptor.cFieldType = 'D';
            break;
        case kColumnLogicalType:
            descriptor.cFieldType = 'L';
            break;
        case kColumnDecimalType:
            descriptor.cFieldType = 'N';
            descriptor.cFieldDecimalCount = info->GetColumnScaleAt (column);
            break;
    }
    descriptor.cFieldLength = info->GetColumnWidthAt (column);
    return (WriteFile (&descriptor, sizeof(TableFieldDescriptor)));
}

/*****************************************************************************
 * Name         : ~ShapeDBF
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
ShapeDBF::~ShapeDBF()
{
    _FDORPT0(0, "Deleting ShapeDBF object\n");

    if (&NoColumns != m_pColumnInfo)
    {
        delete m_pColumnInfo;
        m_pColumnInfo = &NoColumns;
    }

    if ( m_szRowBuffer != NULL )
        delete[] m_szRowBuffer;

	if ( !IsReadOnly() )
		Reopen( IDF_OPEN_READ );
}

/*****************************************************************************
 * Name         : GetFileHeaderDetails
 *
 * Description  : This method gets the file header details.
 *
 * Parameters   : int& nColumns
 *
 * Return       : 
 *
 * Notes        : N/A
 *
 *****************************************************************************/
void ShapeDBF::GetFileHeaderDetails(int& nColumns)
{
    _FDORPT0(0, "GetFileHeaderDetails()\n");

    // Read the header block into a DBF header structure
    if (!ReadFile(&m_DBFHeader,sizeof(DBFHeader)))
        throw FdoCommonFile::LastErrorToException (L"ShapeDBF::GetFileHeaderDetails");
    // Check to see if the given DBF is supported
    CheckDBFFormat ();
    // This is a supported DBF
    nColumns = ((m_DBFHeader.wHeaderSize - 1) / nFIELD_DESCRIPTOR_SIZE ) - 1;

	// Set the ESRI code page based on m_DBFHeader.cLDID
	SetCodePage();
}

// it is assumed that everything is updated except for the date
void ShapeDBF::PutFileHeaderDetails ()
{
    time_t now;
    struct tm *today;

    time (&now);
    today = localtime (&now);
    m_DBFHeader.cUpdateDate[0] = (today->tm_year + 1900) % 100;
    m_DBFHeader.cUpdateDate[1] = today->tm_mon + 1;
    m_DBFHeader.cUpdateDate[2] = today->tm_mday;
    if (!SetFilePointer64 ((FdoInt64)0))
        throw FdoCommonFile::LastErrorToException (L"ShapeDBF::PutFileHeaderDetails(SetFilePointer64)");
    if (!WriteFile (&m_DBFHeader, sizeof(DBFHeader)))
        throw FdoCommonFile::LastErrorToException (L"ShapeDBF::PutFileHeaderDetails(WriteHeader)");
    SetHeaderDirty (false);
}

/*****************************************************************************
 * Name         : CheckDBFFormat
 *
 * Description  : This method checks the file DBF format.
 *
 * Parameters   :
 *
 * Return       : 
 *
 * Notes        : N/A
 *
 *****************************************************************************/
void ShapeDBF::CheckDBFFormat ()
{
    _FDORPT0(0, "CheckDBFFormat()\n");

    // Check the DBF format against the supported versions
    if ((m_DBFHeader.cDBFFormat != cDBASE_III_PLUS_NO_MEMO) && (m_DBFHeader.cDBFFormat != cDBASE_III_PLUS_MEMO))
        throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_FILE_FORMAT_ERROR, "The file '%1$ls' has an unsupported format (%2$ld).", FileName (), m_DBFHeader.cDBFFormat));
}

RowData* ShapeDBF::NewRow (void* buffer)
{
    return (new (m_pColumnInfo, buffer) RowData (m_pColumnInfo, buffer));
}

ColumnInfo* ShapeDBF::NewColumnInfo ()
{
    return (new (GetColumnInfo ()->GetNumColumns ()) ColumnInfo (*GetColumnInfo ()));
}

ColumnInfo* ShapeDBF::NewColumnInfo (int nColumns)
{
    return (new (nColumns) ColumnInfo (nColumns));
}

ColumnInfo* ShapeDBF::NewColumnInfo (ColumnInfo* info)
{
    return (new (info->GetNumColumns ()) ColumnInfo (*info));
}

/*****************************************************************************
 * Name         : GetRowAt
 *
 * Description  : This method gets the specified record #.
 *
 * Parameters   : int nRecord
 *
 * Return       : RowData*
 *
 * Notes        : N/A
 *
 *****************************************************************************/
RowData* ShapeDBF::GetRowAt(int nRecord)
{
    int nStatus = SHP_OK;
    RowData* pRowData = NULL;

    _FDORPT1(0, "GetRowAt(%d)\n",nRecord);

	// Quick return if the requested row doesn't exist.
    if ( (nRecord < 0) || (nRecord >= m_DBFHeader.nRecords) )
		return NULL;

    // Check the cache
    pRowData = GetRowDataFromCache(nRecord);

    if ( NULL != pRowData )
        return pRowData;

    // Not found, seek to the given record #
    if ( !SetFilePointer64((FdoInt64)(m_nRecordStart + (nRecord * m_DBFHeader.wRecordSize))) )
        throw FdoException::Create (NlsMsgGet(SHP_READ_FILE_ERROR, "Error occured reading file '%1$ls'.", FileName() ));

    // Read the entire row of data into a buffer and then parse the buffer
    int nRowLength = m_DBFHeader.wRecordSize;
    long nRowsLengthRead = 0;

    // Buffer read: Allocate one more byte than necessary so we can zero-terminate
    // the field in the last column position of the last record.
    if ( m_szRowBuffer == NULL )
    {
        m_szRowBuffer = new BYTE[(SHP_FILE_READ_CACHE_SIZE * m_DBFHeader.wRecordSize + 1)];
        if ( m_szRowBuffer == NULL )
            throw FdoException::Create (NlsMsgGet(SHP_OUT_OF_MEMORY_ERROR, "Out of memory."));
    }

    if ( !ReadFile(m_szRowBuffer, SHP_FILE_READ_CACHE_SIZE * nRowLength, &nRowsLengthRead))
        throw FdoException::Create (NlsMsgGet(SHP_READ_FILE_ERROR, "Error occured reading file '%1$ls'.", FileName() ));

    int nNumNewRows = nRowsLengthRead / (sizeof (BYTE) * nRowLength);

    // Refresh the cache
    m_nFirstCacheRecordId = nRecord;
    m_nLastCacheRecordId = nRecord + nNumNewRows - 1;

    // Check again the cache. The record is in the first position.
    pRowData = GetRowDataFromCache(nRecord);

    return pRowData;
}

/*****************************************************************************
 * Name         : SetRowAt
 *
 * Description  : This method sets the data for the record # specified.
 *
 * Parameters   : int nRecord
 *
 * Return       : 
 *
 * Notes        : N/A
 *
 *****************************************************************************/
void ShapeDBF::SetRowAt (RowData* row, int nRecord, bool batch)
{
    int length;

    _FDORPT1(0, "SetRowAt(%d)\n",nRecord);

    if ((nRecord >= 0) && (nRecord < m_DBFHeader.nRecords + 1)) // can add one more to the end
    {
        // seek to given record #
        if (!SetFilePointer64 ((FdoInt64)(m_nRecordStart + (nRecord * m_DBFHeader.wRecordSize))))
            throw FdoCommonFile::LastErrorToException (L"ShapeDBF::SetRowAt(SetFilePointer64)");

        length = m_DBFHeader.wRecordSize;
		if (!WriteFile (row->mBuffer, length))
            throw FdoCommonFile::LastErrorToException (L"ShapeDBF::SetRowAt(WriteBuffer)");

        // add an end of file marker if we are writing a new record
		if (nRecord >= m_DBFHeader.nRecords)
        {
            char eof = END_OF_FILE;
			if (!WriteFile (&eof, 1))
				throw FdoCommonFile::LastErrorToException (L"ShapeDBF::SetRowAt(WriteBuffer)");
		}

        if (nRecord >= m_DBFHeader.nRecords)
        {
            m_DBFHeader.nRecords++;
            SetHeaderDirty (true);
        }
        if (!batch)
            PutFileHeaderDetails ();
    }
    else
        throw FdoException::Create (NlsMsgGet(SHP_INVALID_RECORD_NUMBER_ERROR, "Invalid record number %1$ld for file '%2$ls'.", nRecord, FileName ()));

    // Avoid the cache getting stale
    m_nLastCacheRecordId = -1;
}

void ShapeDBF::DeleteRowAt (int nRecord)
{
    BYTE c;

    if (!SetFilePointer64 ((FdoInt64)(m_nRecordStart + (nRecord * m_DBFHeader.wRecordSize))))
        throw FdoCommonFile::LastErrorToException (L"ShapeDBF::DeleteRowAt(SetFilePointer64)");
    // set Record as deleted
    c = cDELETED_RECORD_ID;
    if (!WriteFile (&c, 1))
        throw FdoCommonFile::LastErrorToException (L"ShapeDBF::DeleteRowAt(WriteMark)");

    // Avoid the cache getting stale
    m_nLastCacheRecordId = -1;
}

/*****************************************************************************
 * Name         : GetRowDataFromCache
 *
 * Description  : This method gets the specified record # data from the cache
 *                if it is there.
 *
 * Parameters   : int nRecord
 *
 * Return       : RowData*
 *
 * Notes        : Records start from 0
 *
 *****************************************************************************/
RowData* ShapeDBF::GetRowDataFromCache(int nRecord)
{
    RowData* pRowData = NULL;

    // If the record in the current read buffer, then create the object.
    if ( nRecord >= m_nFirstCacheRecordId && nRecord <= m_nLastCacheRecordId )
    {
        long rawOffset = (nRecord - m_nFirstCacheRecordId) * m_DBFHeader.wRecordSize;
        pRowData = NewRow (&m_szRowBuffer[rawOffset]);
    }

    return pRowData;
}

FdoStringP ShapeDBF::GetCodePage()
{
	return mCodePageESRI;
}

void ShapeDBF::SetCodePage()
{
	BYTE	ldid = m_DBFHeader.cLDID[0];

	// Find the mapping LDID->codepage
	if ( ldid > 0 )
	{
		bool	found = false;
		int		numLDIDs = sizeof(EsriCodePageMapper)/sizeof(EsriCodePageMap);
		for ( int i = 0; i < numLDIDs && !found; i++ )
		{
			found = ( EsriCodePageMapper[i].cLDID == ldid );
			if ( found )
				mCodePageESRI = FdoStringP::Format(L"%ld", EsriCodePageMapper[i].codePage);
		}	
	}
}

BYTE ShapeDBF::GetLDIDFromLocale()
{
	BYTE		ldid = 0;
	ULONG		cpg = 0;

	// Get the locale and convert to codepage
	FdoStringP  pLocale = FdoStringP( setlocale(LC_ALL, NULL) );

	mCodePageESRI = pLocale.Right(L".");
#ifndef _WIN32
	if ( pLocale.Contains(L"@") )
		mCodePageESRI = pLocale.Left(L"@");

	if ( pLocale.Contains(L"iso") )
		mCodePageESRI = pLocale.Right(L"iso");
	else if ( pLocale.Contains(L"cp") )
		mCodePageESRI = pLocale.Right(L"cp");
	else if ( pLocale.Contains(L"big5") )
		mCodePageESRI = L"950";  // zh_TW.big5
	else if ( pLocale.Contains(L"ujis") )
		mCodePageESRI = L"932";  // ja_JP.ujis		
#endif
	if (mCodePageESRI.IsNumber())
	{
		cpg = mCodePageESRI.ToLong();

		// Find the mapping codepage->LDID
		int	 numLDIDs = sizeof(EsriCodePageMapper)/sizeof(EsriCodePageMap);
		for ( int i = 0; i < numLDIDs && (ldid == 0); i++ )
		{
			if ( EsriCodePageMapper[i].codePage == cpg )
				ldid = EsriCodePageMapper[i].cLDID;
		}
	}
	else
	{
		// No mapping. LDID = 0;
		mCodePageESRI = L"";
	}

	return ldid;
}
