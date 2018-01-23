// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "PhysName.h"

PhysName::PhysName( FdoString* wPrefix, FdoString* wName, bool isUTF8 ) :
    mIsUTF8(isUTF8),
    mWName(NULL),
    mSName(NULL)
{
    SetWName( wPrefix, wName );

    if ( isUTF8 )
        mFdoName = mWName;
}

PhysName::PhysName( const char* sPrefix, const char* sName, bool isUTF8 ) :
    mIsUTF8(isUTF8),
    mWName(NULL),
    mSName(NULL)
{
    SetSName( sPrefix, sName );

    if ( isUTF8 )
        mFdoName = mSName;
}

PhysName::PhysName(const PhysName& oName)
{
    mIsUTF8 = oName.mIsUTF8;

    if (mIsUTF8 ) {
        mFdoName = oName.mFdoName;
    }
    else {
        SetWName( L"", oName.mWName );
        SetSName( "", oName.mSName );
    }
}

PhysName::~PhysName()
{
    if ( mWName ) 
        delete[] mWName;

    if ( mSName ) 
        delete[] mSName;
}

bool PhysName::IsUTF8()
{
    return mIsUTF8;
}

PhysName::operator FdoString*( )
{
    if ( IsUTF8() ) 
        return (FdoString*) mFdoName;

    if ( !mWName ) {
        if ( mSName ) {
            size_t len = strlen(mSName);
            mWName = new wchar_t[len + 1];
			mWName[0] = '\0';
            mbstowcs( mWName, mSName, len );
        }
    }

    return mWName;
}

PhysName::operator const char*( )
{
    if ( IsUTF8() ) 
        return (const char*) mFdoName;

    if ( !mSName ) {
        if ( mWName ) {
            size_t len = wcslen(mWName) * 6;
            mSName = new char[len + 1];
			mSName[0] = '\0';
            wcstombs( mSName, mWName, len );
        }
    }

    return mSName;
}

void PhysName::SetWName( FdoString* wPrefix, FdoString* wName )
{
    if ( mWName ) 
        delete[] mWName;

    if ( wName ) {
        mWName = new wchar_t[wcslen(wPrefix) + wcslen(wName) + 1];
        wcscpy( mWName, wPrefix );
        wcscat( mWName, wName );
    }
}

void PhysName::SetSName( const char* sPrefix, const char* sName )
{
    if ( mSName ) 
        delete[] mSName;

    if ( sName ) {
        mSName = new char[strlen(sPrefix) + strlen(sName) + 1];
        strcpy( mSName, sPrefix );
        strcat( mSName, sName );
    }
}

