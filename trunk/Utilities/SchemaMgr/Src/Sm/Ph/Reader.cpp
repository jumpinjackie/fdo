#include "stdafx.h"
#include <Sm/Ph/Reader.h>
#include <Sm/Error.h>

 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

FdoSmPhReader::FdoSmPhReader() :
    mSubReader((FdoSmPhReader*) NULL),
	mbBOF(true),
	mbEOF(false)
{
}

FdoSmPhReader::FdoSmPhReader(FdoPtr<FdoSmPhReader> subReader) :
    FdoSmPhReadWrite(FdoSmPhReadWriteP(FDO_SAFE_ADDREF(subReader.p))),
    mSubReader(subReader),
	mbBOF(true),
	mbEOF(false)
{
}

FdoSmPhReader::FdoSmPhReader(FdoSmPhMgrP mgr, FdoSmPhRowsP pRow) :
    FdoSmPhReadWrite(mgr, pRow),    
	mbBOF(true),
	mbEOF(false)
{
}

FdoSmPhReader::~FdoSmPhReader(void)
{
}

void FdoSmPhReader::SetSubReader(FdoPtr<FdoSmPhReader> subReader)
{
    mSubReader = subReader;
    FdoSmPhReadWrite::SetSubReaderWriter(FDO_SAFE_ADDREF(subReader.p));
}


// The following functions delegate to the inner reader if this
// is a telescoped reader.

bool FdoSmPhReader::ReadNext()
{
    if ( mSubReader ) {
        return( mSubReader->ReadNext() );
    }
    else {
        SetEOF(true);
        return false;
    }
}

bool FdoSmPhReader::IsBOF()
{
    return mSubReader ? mSubReader->IsBOF() : mbBOF;
}

bool FdoSmPhReader::IsEOF()
{
	return mSubReader ? mSubReader->IsEOF() : mbEOF;
}

void FdoSmPhReader::SetBOF(bool bBOF)
{
    if ( mSubReader ) 
        mSubReader->SetBOF(bBOF);
    else
    	mbBOF = bBOF;
}

void FdoSmPhReader::SetEOF(bool bEOF)
{
    if ( mSubReader ) 
        mSubReader->SetEOF(bEOF);
    else
    	mbEOF = bEOF;
}

void FdoSmPhReader::CheckGet()
{
	if ( IsBOF() || IsEOF() ) 
		throw FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_211)
            )
		);
}

void FdoSmPhReader::EndSelect()
{
	if ( mSubReader )
		mSubReader->EndSelect();
}

