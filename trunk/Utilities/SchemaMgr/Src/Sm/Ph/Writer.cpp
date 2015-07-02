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
#include <Sm/Ph/Writer.h>
#include <Sm/Error.h>

FdoSmPhWriter::FdoSmPhWriter(FdoPtr<FdoSmPhWriter> subWriter) :
	FdoSmPhReadWrite(subWriter == NULL ? NULL : subWriter->SmartCast<FdoSmPhReadWrite>() ),
    mSubWriter(subWriter)
{
}

FdoSmPhWriter::FdoSmPhWriter(FdoSmPhMgrP mgr, FdoSmPhRowP row) :
    FdoSmPhReadWrite(mgr, MakeCollection(row))
{
}

FdoSmPhRowP FdoSmPhWriter::GetRow()
{
    FdoSmPhRowP row;
    FdoSmPhRowsP rows = this->GetRows();

    if ( rows->GetCount() == 1 ) 
        row = rows->GetItem(0);

    return row;
}

void FdoSmPhWriter::Add()
{
    if ( mSubWriter ) {
        mSubWriter->Add();
    }
    else {
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_25), 
                L"Add()"
            )
        );
    }
}

void FdoSmPhWriter::Modify( FdoStringP sClauses )
{
    if ( mSubWriter ) {
        mSubWriter->Modify( sClauses);
    }
    else {
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_25), 
                L"Modify()"
            )
        );
    }
}

void FdoSmPhWriter::Delete( FdoStringP sClauses )
{
    if ( mSubWriter ) {
        mSubWriter->Delete( sClauses );
    }
    else {
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_25), 
                L"Delete()"
            )
        );
    }
}

FdoSmPhRowsP FdoSmPhWriter::MakeCollection(FdoSmPhRowP row)
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();
    rows->Add(row);
    return(rows);
}


void FdoSmPhWriter::SetSubWriter(FdoPtr<FdoSmPhWriter> subWriter)
{
    mSubWriter = subWriter;
    FdoSmPhReadWrite::SetSubReaderWriter(FDO_SAFE_ADDREF(subWriter.p));
}
