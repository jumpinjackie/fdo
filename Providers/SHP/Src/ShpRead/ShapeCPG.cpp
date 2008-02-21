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
 *         $Id$
 *     $Author$
 *   $DateTime$
 *     $Change$
 *
 */

#include <locale.h>

#include "stdafx.h"
#include "Constants.h"
#include "ShapeCPG.h"


using namespace std;

ShapeCPG::~ShapeCPG (void)
{
}

ShapeCPG::ShapeCPG (const wchar_t* name, int& status)
{
    mCodePageESRI = FdoStringP(L"");

    ErrorCode code;

    if (OpenFile (name, IDF_OPEN_READ, code))
    {
        unsigned long  size;
        if (FdoCommonFile::GetFileSize (size))
        {
            char*   buffer = new char[size + 1];

            if ( !FdoCommonFile::ReadFile( buffer, size ) )
                throw FdoCommonFile::LastErrorToException (L"ShapeCPG::ShapeCPG");
            else
            {
                buffer[size] = '\0'; // EOF
                mCodePageESRI = FdoStringP( buffer );
            }
            delete [] buffer;
        }
        else
            throw FdoCommonFile::LastErrorToException (L"ShapeCPG::ShapeCPG");
    }
    else
        throw FdoCommonFile::ErrorCodeToException (code, name, IDF_OPEN_READ);
}

ShapeCPG::ShapeCPG (const WCHAR* name, char *locale)
{
    ErrorCode status;

    _FDORPT0(0, "Creating new ShapeCPG object\n");

    try
    {
        if (OpenFile (name, (OpenFlags)(IDF_CREATE_NEW | IDF_OPEN_UPDATE), status))
        {
            SetCodePageESRIFromLocale( locale );

            if (!WriteFile ((void *)(const char*)mCodePageESRI, (long)mCodePageESRI.GetLength()))
                throw FdoCommonFile::LastErrorToException (L"ShapeCPG::ShapeCPG()");
            CloseFile ();
        }
        else
            throw FdoCommonFile::ErrorCodeToException (status, name, (OpenFlags)(IDF_CREATE_NEW | IDF_OPEN_UPDATE));
    }
    catch (...)
    {
        throw;
    }
}

ShapeCPG::ShapeCPG ()
{
    SetCodePageESRIFromLocale( NULL );
}

FdoStringP ShapeCPG::GetCodePage()
{
    return mCodePageESRI;
}

void ShapeCPG::SetCodePage( FdoString* codePage )
{
    mCodePageESRI = FdoStringP( codePage );
}

void ShapeCPG::SetCodePageESRIFromLocale( char *locale )
{
	FdoStringP  pLocale = FdoStringP( locale );
	FdoStringP  codePageESRI = L"";

    // Save current
   	char  localeSave[50];
	strcpy(localeSave, setlocale(LC_ALL, NULL));

    if ( locale == NULL )
    {
        // Just query the locale
        pLocale = FdoStringP(setlocale(LC_ALL, NULL));
    }

	// setlocale() is supposed to return: "lang[_country_region[.code_page]]
	if ( !pLocale.Contains(L"." ) )
		pLocale = FdoStringP(setlocale(LC_ALL, ""));

	codePageESRI = pLocale.Right(L".");
#ifndef _WIN32
	if ( pLocale.Contains(L"@") )
		codePageESRI = pLocale.Left(L"@");

	if ( pLocale.Contains(L"iso") )
		codePageESRI = pLocale.Right(L"iso");
	else if ( pLocale.Contains(L"cp") )
		codePageESRI = pLocale.Right(L"cp");
	else if ( pLocale.Contains(L"utf8") )
		codePageESRI = L"UTF-8";
	else if ( pLocale.Contains(L"euc") )
		codePageESRI = L"EUC";
	else if ( pLocale.Contains(L"big5") )
		codePageESRI = L"BIG5";  // zh_TW.big5
	else if ( pLocale.Contains(L"ujis") )
		codePageESRI = L"SJIS";  // ja_JP.ujis
	
#endif
	// ESRI ISO codes are shifted by 50000
	if (codePageESRI.IsNumber())
	{
		ULONG   cpg = codePageESRI.ToLong();
		if ( cpg >= 28591 && cpg <= 28605)
			cpg += 60000;
		codePageESRI = FdoStringP::Format(L"%ld", cpg);
	}

    // Restore
    if ( !(strlen(localeSave) == 1 && localeSave[0] == 'C' ))
    setlocale(LC_ALL, localeSave);

    mCodePageESRI = codePageESRI;
}

#ifdef _WIN32
ULONG	ShapeCPG::ConvertCodePageWin(WCHAR *codepage)
{
	ULONG	sysCpg = CP_THREAD_ACP;

	if ( codepage ) 
	{
		FdoStringP	sCpg = FdoStringP( codepage );

		if ( sCpg.IsNumber() )
		{
			sysCpg = sCpg.ToLong();

			// ISO 8859-2 is coded by ESRI as 88592 and corresponds to 28592 Windows codepage
			if ( sysCpg >= 88591 && sysCpg <= 88605 )
				sysCpg -= 60000;
		}
		else if ( sCpg.ICompare(L"UTF-8") == 0)
			sysCpg = CP_UTF8;
		else if ( sCpg.ICompare(L"OEM") == 0)
			sysCpg = CP_OEMCP;
		else if ( sCpg.ICompare(L"ANSI") == 0)
			sysCpg = 1252;  // ANSI Latin I
		else if ( sCpg.ICompare(L"EUC") == 0)
			sysCpg = 51936; // Simplified chinese
		else if ( sCpg.ICompare(L"Big5") == 0)
			sysCpg = 10002;
		else if ( sCpg.ICompare(L"SJIS") == 0)
			sysCpg = 932; 
		else if ( sCpg.ICompare(L"ISO") == 0)
			sysCpg = 28591; // ISO 8859-1 Latin I
	}
	return sysCpg;
}

#else

const char	*ShapeCPG::ConvertCodePageLinux(WCHAR *codepage)
{
	FdoStringP	sysCpg = L"";
	FdoStringP	sCpg = FdoStringP( codepage );

	if ( sCpg.IsNumber() )
	{
		long	esriCpg = sCpg.ToLong();

		// ISO 8859-2 is coded by ESRI as 88592 and corresponds to 28592 Windows codepage
		if ( esriCpg >= 88591 && esriCpg <= 88605 )
			sysCpg = FdoStringP::Format(L"ISO8859-%ld", esriCpg-88590);
		else if ( esriCpg >= 437 && esriCpg <= 1258 )
			sysCpg = FdoStringP::Format(L"CP%ld", esriCpg);
	}
	else if ( sCpg.ICompare(L"UTF-8") == 0 )
		sysCpg = L"UTF-8";
	else if ( sCpg.ICompare(L"OEM") == 0)
		sysCpg = L"CP850";   // Multilingual Latin I
	else if ( sCpg.ICompare(L"ANSI") == 0)
		sysCpg = L"CP1252";  // ANSI Latin I
	else if ( sCpg.ICompare(L"EUC") == 0)
		sysCpg = L"EUC-CN"; // Simplified chinese
	else if ( sCpg.ICompare(L"Big5") == 0)
		sysCpg = L"BIG5";
	else if ( sCpg.ICompare(L"SJIS") == 0)
		sysCpg = L"SHIFT-JIS"; 
	else if ( sCpg.ICompare(L"ISO") == 0)
		sysCpg = L"ISO8859-1"; // ISO 8859-1 Latin I

	mLinuxCpg = sysCpg;
	return (const char *)mLinuxCpg;
}
#endif
