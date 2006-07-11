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
            delete buffer;
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
			FdoStringP  pLocale = FdoStringP( locale );
			mCodePageESRI = L"";

			// setlocale() is supposed to return: "lang[_country_region[.code_page]]
			if ( !pLocale.Contains(L"." ) )
				pLocale = FdoStringP(setlocale(LC_ALL, ""));
			mCodePageESRI = pLocale.Right(L".");
#ifndef _WIN32
			if ( pLocale.Contains(L"@") )
				mCodePageESRI = pLocale.Left(L"@");

			if ( pLocale.Contains(L"iso") )
				mCodePageESRI = pLocale.Right(L"iso");
			else if ( pLocale.Contains(L"cp") )
				mCodePageESRI = pLocale.Right(L"cp");
			else if ( pLocale.Contains(L"utf8") )
				mCodePageESRI = L"UTF-8";
			else if ( pLocale.Contains(L"euc") )
				mCodePageESRI = L"EUC";
			else if ( pLocale.Contains(L"big5") )
				mCodePageESRI = L"BIG5";  // zh_TW.big5
			else if ( pLocale.Contains(L"ujis") )
				mCodePageESRI = L"SJIS";  // ja_JP.ujis
			
#endif
			// ESRI ISO codes are shifted by 50000
			if (mCodePageESRI.IsNumber())
			{
				ULONG   cpg = mCodePageESRI.ToLong();
				if ( cpg >= 28591 && cpg <= 28605)
					cpg += 60000;
				mCodePageESRI = FdoStringP::Format(L"%ld", cpg);
			}

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
FdoStringP ShapeCPG::GetCodePage()
{
    return mCodePageESRI;
}

void ShapeCPG::SetCodePage( FdoString* codePage )
{
    mCodePageESRI = FdoStringP( codePage );
}

