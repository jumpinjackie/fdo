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
#include "FieldArray.h"
#include "../Mgr.h"

FdoSmPhRdGrdFieldArray::FdoSmPhRdGrdFieldArray(
    FdoSmPhFieldP pField,
    GdbiQueryResult* results,
    int posn
):
    FdoSmPhSchemaElement(
        pField->GetName(),
        L"",
        (FdoSmPhMgr*) NULL
    ),
    mpField(pField),
    mPosn(posn),
    mResults(results),
    mNull(0)
{
//    results->GetColumnDesc( posn - 1, mColDesc );
    FdoSmPhColumnP col = pField->GetColumn();
    mValueSize = col->GetDefineSize();

    //TODO: a hack until types are supported
    if ( mValueSize < 50 ) mValueSize = 50;

    // Create a buffer large enough to hold any field value.
    mpBuffer = new FdoByte[mValueSize];
}

FdoSmPhRdGrdFieldArray::~FdoSmPhRdGrdFieldArray(void)
{
    if ( mpBuffer )
        delete[] mpBuffer;
}

FdoStringP FdoSmPhRdGrdFieldArray::GetString()
{
    int ccode;
/* TODO: handle type conversion. Currently GdbiQueryResult fetches
 all fields as strings so no conversion required.

    switch ( mpField->GetColumn()->GetRdbType() ) {
        case RDBI_DOUBLE:
            return FdoStringP::Format(L"%lf", GetDouble());
        case RDBI_LONG:
            return FdoStringP::Format(L"%ld", GetLong());
        case RDBI_STRING:
*/

		const wchar_t* value = mResults->GetString((wchar_t*)(const wchar_t*)FdoStringP( mpField->GetName() ),&mNull,
                &ccode);
        return (mNull != 0) ? FdoStringP(L"") : FdoStringP( value );
//    }

    throw FdoSchemaException::Create(
		NlsMsgGet2(
		    FDORDBMS_141,
            "Field %1$ls has unsupported RDBI type (%2$d)",
           (FdoString*) mpField->GetQName(),
           (int) (mpField->GetColumn()->GetRdbType())
        )
    );
}

int FdoSmPhRdGrdFieldArray::GetInteger()
{
    int ccode;

    // TODO: type conversion
    mResults->GetBinaryValue(
        (const wchar_t*)FdoStringP( mpField->GetName() ),
        mValueSize,
        (char*) mpBuffer,
        &mNull,
        &ccode
    );

    return (mNull == true) ? 0 : *((long*) mpBuffer);
}

long FdoSmPhRdGrdFieldArray::GetLong()
{
    return GetInteger();
}

double FdoSmPhRdGrdFieldArray::GetDouble()
{
    int ccode;

    // TODO: type conversion
    mResults->GetBinaryValue(
        (const wchar_t*)FdoStringP( mpField->GetName() ),
        mValueSize,
        (char*) mpBuffer,
        &mNull,
        &ccode
    );

    return (mNull == true) ? 0 : *((double*) mpBuffer);
}

bool FdoSmPhRdGrdFieldArray::GetBoolean()
{
    return (GetInteger() != 0);
}

