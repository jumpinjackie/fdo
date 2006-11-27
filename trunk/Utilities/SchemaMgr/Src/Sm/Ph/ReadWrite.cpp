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
    FdoInt32 cidx1, cidx2;

    if ( mSubReadWrite ) {
        mSubReadWrite->Clear();
    }
    else {
        if ( mRows ) {
            for ( cidx1 = 0; cidx1 < mRows->GetCount(); cidx1++ ) {
                FdoSmPhRowP row = mRows->GetItem(cidx1);
                FdoSmPhFieldsP	pFields = row->GetFields();

                // Clear each field.
                for ( cidx2 = 0; cidx2 < pFields->GetCount(); cidx2++ ) {
                    FdoSmPhFieldP field = pFields->GetItem(cidx2);
                    field->Clear();
                }
            }
        }
    }
}

FdoStringP FdoSmPhReadWrite::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    if ( mSubReadWrite ) {
        return mSubReadWrite->GetString( tableName, fieldName );
    }
    else {
        FdoSmPhFieldP field = GetField(tableName, fieldName);

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

    if ( mSubReadWrite ) {
        field = mSubReadWrite->GetField( tableName, fieldName );
    }
    else {
        if ( mRows ) {
            field = mRows->GetField( tableName, fieldName );
        }
    }

    return( field );
}

void FdoSmPhReadWrite::SetString( FdoStringP tableName, FdoStringP fieldName, FdoStringP sValue )
{
    if ( mSubReadWrite ) {
        return mSubReadWrite->SetString( tableName, fieldName, sValue );
    }
    else {
        FdoSmPhFieldP field = GetField(tableName, fieldName);

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

