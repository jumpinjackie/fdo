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
 *
 */

#include "stdafx.h"
#include <Sm/Ph/ReadWrite.h>
#include <Sm/Error.h>
#include <FdoCommonStringUtil.h>
#include <limits>       // For quiet_NaN()
#include <math.h>      // For _isnan()
using namespace std;


FdoSmPhReadWrite::FdoSmPhReadWrite(FdoPtr<FdoSmPhReadWrite> subReadWrite) :
    mSubReadWrite(subReadWrite),
    mMgr(NULL)
{
}

FdoSmPhReadWrite::FdoSmPhReadWrite(FdoSmPhMgrP mgr, FdoSmPhRowsP rows) :
	mMgr(mgr),
	mRows(rows)
{
}

FdoSmPhReadWrite::~FdoSmPhReadWrite(void)
{
}

void FdoSmPhReadWrite::SetSubReaderWriter(FdoPtr<FdoSmPhReadWrite> subReaderWriter)
{
    mSubReadWrite = subReaderWriter;
}


FdoSmPhMgrP FdoSmPhReadWrite::GetManager()
{
    return mSubReadWrite ? mSubReadWrite->GetManager() : FdoSmPhMgrP(FDO_SAFE_ADDREF(mMgr));
}

void FdoSmPhReadWrite::Clear()
{
    // Clear field values for any sub Reader/Writer
    if ( mSubReadWrite )
        mSubReadWrite->Clear();

    // Clear field values for this Reader/Writer
    if ( mRows )
        mRows->Clear();
}

FdoStringP FdoSmPhReadWrite::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    // Check the sub ReadWrite first. Get values from the innermost Reader/Writer
    if ( mSubReadWrite && mSubReadWrite->GetField(tableName, fieldName) ) {
        return mSubReadWrite->GetString( tableName, fieldName );
    }
    else {
        // No sub or field not in sub so get from this Reader/Writer
        FdoSmPhFieldP field = mRows ? mRows->GetField(tableName, fieldName) : FdoSmPhFieldP();

        if ( !field ) 
            throw FdoSchemaException::Create (
                FdoSmError::NLSGetMessage(
                    FDO_NLSID(FDOSM_309), 
                    tableName.GetLength() > 0 ?
                        (FdoString*)(tableName + L"." + fieldName) :
                        (FdoString*) fieldName
                )
            );

        return( field->GetFieldValue() );
    }
}

int FdoSmPhReadWrite::GetInteger( FdoStringP tableName, FdoStringP fieldName )
{
	FdoStringP sValue = GetString( tableName, fieldName );

    return( (int) sValue.ToLong() );
}

long FdoSmPhReadWrite::GetLong( FdoStringP tableName, FdoStringP fieldName )
{
	FdoStringP sValue = GetString( tableName, fieldName );

    return( sValue.ToLong() );
}

FdoInt64 FdoSmPhReadWrite::GetInt64( FdoStringP tableName, FdoStringP fieldName )
{
	FdoStringP sValue = GetString( tableName, fieldName );

    return( FdoCommonStringUtil::StringToInt64( sValue ) );
}

double FdoSmPhReadWrite::GetDouble( FdoStringP tableName, FdoStringP fieldName )
{
	FdoStringP sValue = GetString( tableName, fieldName );

    if (wcslen(sValue) == 0)
        return numeric_limits<double>::quiet_NaN();
    else
        return( sValue.ToDouble() );
}

bool FdoSmPhReadWrite::GetBoolean( FdoStringP tableName, FdoStringP fieldName )
{
	FdoStringP sValue = GetString( tableName, fieldName );

    return( sValue.ToBoolean() );
}

FdoSmPhFieldP FdoSmPhReadWrite::GetField( FdoStringP tableName, FdoStringP fieldName )
{
    FdoSmPhFieldP field;

    // Check the sub ReadWrite first. Get fields from the innermost Reader/Writer
    if ( mSubReadWrite ) {
        field = mSubReadWrite->GetField( tableName, fieldName );
    }

    if ( mRows && !field ) {
        // No sub or not in sub so get field from this Reader/Writer
        field = mRows->GetField( tableName, fieldName );
    }

    return( field );
}

void FdoSmPhReadWrite::SetString( FdoStringP tableName, FdoStringP fieldName, FdoStringP sValue )
{
    // Check the sub ReadWrite first. Get fields from the innermost Reader/Writer
    if ( mSubReadWrite && mSubReadWrite->GetField(tableName, fieldName) ) {
        return mSubReadWrite->SetString( tableName, fieldName, sValue );
    }
    else {
        // No sub or not in sub so set field in this Reader/Writer
        FdoSmPhFieldP field = mRows ? mRows->GetField(tableName, fieldName) : FdoSmPhFieldP();

        if ( !field ) 
            throw FdoSchemaException::Create (
                FdoSmError::NLSGetMessage(
                    FDO_NLSID(FDOSM_309), 
                    tableName.GetLength() > 0 ?
                        (FdoString*)(tableName + L"." + fieldName) :
                        (FdoString*) fieldName
                )
            );

        field->SetFieldValue( sValue );
    }
}

void FdoSmPhReadWrite::SetInteger( FdoStringP tableName, FdoStringP fieldName, int iValue )
{
    SetString( tableName, fieldName, FdoStringP::Format(L"%d", iValue) );
}

void FdoSmPhReadWrite::SetLong( FdoStringP tableName, FdoStringP fieldName, long lValue )
{
    SetString( tableName, fieldName, FdoStringP::Format(L"%d", lValue) );
}

void FdoSmPhReadWrite::SetInt64( FdoStringP tableName, FdoStringP fieldName, FdoInt64 i64Value )
{
    SetString( tableName, fieldName, FdoCommonStringUtil::Int64ToString(i64Value) );
}

void FdoSmPhReadWrite::SetDouble( FdoStringP tableName, FdoStringP fieldName, double dValue )
{
#ifdef _WIN32
    bool isNan = _isnan(dValue) ? true : false;
#else
	bool isNan =  isnan(dValue) ? true : false;
#endif

    if (isNan)
        SetString( tableName, fieldName, FdoStringP::mEmptyString );
    else
        SetString( tableName, fieldName, FdoStringP::Format(L"%lf", dValue) );
}

void FdoSmPhReadWrite::SetBoolean( FdoStringP tableName, FdoStringP fieldName, bool bValue )
{
    SetString( tableName, fieldName, bValue ? L"1" : L"0" );
}

