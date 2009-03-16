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
#include "ShapePRJ.h"


using namespace std;

ShapePRJ::~ShapePRJ (void)
{
}

ShapePRJ::ShapePRJ (const wchar_t* name, int& status)
{
    mWKT = FdoStringP(L"");
    mTempSCName = L"";
    ErrorCode code;

    if (OpenFile (name, IDF_OPEN_READ, code))
    {
        unsigned long  size;
        
        if (FdoCommonFile::GetFileSize (size))
        {
            char*   buffer = new char[size + 1];

            if ( !FdoCommonFile::ReadFile( buffer, size ) )
                throw FdoCommonFile::LastErrorToException (L"ShapePRJ::ShapePRJ", name);
            else
            {
                buffer[size] = '\0'; // EOF
                mWKT = FdoStringP( buffer );
            }

            delete[] buffer;
        }
        else
            throw FdoCommonFile::LastErrorToException (L"ShapePRJ::ShapePRJ", name);
    }
    else
        throw FdoCommonFile::ErrorCodeToException (code, name, IDF_OPEN_READ);
}

ShapePRJ::ShapePRJ (const WCHAR* name, const WCHAR* wkt) :
    mWKT( wkt )
{
    ErrorCode code; 

    _FDORPT0(0, "Creating new ShapePRJ object\n");

    if (OpenFile (name, (OpenFlags)(IDF_CREATE_NEW | IDF_OPEN_UPDATE), code))
    {
		// Convert to UTF8
		char*	wkt2 = (char *)(const char *)mWKT; 

        if (!WriteFile (wkt2, (long) strlen(wkt2)))
            throw FdoCommonFile::LastErrorToException (L"ShapePRJ::ShapePRJ(wkt)", name);

        CloseFile ();
    }
    else
        throw FdoCommonFile::ErrorCodeToException (code, name, (OpenFlags)(IDF_CREATE_NEW | IDF_OPEN_UPDATE));
 }

FdoStringP ShapePRJ::GetWKT()
{
    return mWKT;
}

void ShapePRJ::SetWKT( FdoString* wkt )
{
    mWKT = FdoStringP( wkt );
}

FdoStringP ShapePRJ::GetCoordSysName()
{
	FdoStringP	right;
	FdoStringP	csName;

    if (mTempSCName != L"")
        return mTempSCName;
	// Parse the WKT. Ignore spaces.
	if ( mWKT.Contains( L"PROJCS" ) )
		right = mWKT.Right( L"PROJCS" );
	else if ( mWKT.Contains( L"GEOGCS" ) )
		right = mWKT.Right( L"GEOGCS" );
	else if ( mWKT.Contains( L"LOCAL_CS" ) )
		right = mWKT.Right( L"LOCAL_CS" );

	if ( right != L"" )
	{
		FdoStringP right2 = right.Right( L"[" );
		FdoStringP right3 = right2.Right( L"\"" );
		csName = right3.Left( L"\"" );
	}

	return FdoStringP( csName );	
}

