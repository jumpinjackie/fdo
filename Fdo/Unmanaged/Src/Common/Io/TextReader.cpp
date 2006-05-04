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
#include <Common.h>

FdoIoTextReader::FdoIoTextReader(FdoIoStream* stream)
{
    stream->AddRef();
    mStream = stream;
}

FdoIoTextReader::~FdoIoTextReader(void)
{
}

FdoIoTextReader* FdoIoTextReader::Create( FdoString* fileName )
{
    //Open file for read-only
    FdoIoStreamP stream = FdoIoFileStream::Create(fileName, L"rt" );

    return( FdoIoTextReader::Create(stream) );
}

FdoIoTextReader* FdoIoTextReader::Create( FdoIoStream* stream )
{
    if ( !stream ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"stream",
                L"(NULL)",
                L"FdoIoTextReader::Create"
            )
        );

    return( new FdoIoTextReader(stream) );
}

FdoIoStream* FdoIoTextReader::GetStream()
{
    FdoIoStream* pStream = mStream;
    pStream->AddRef();

    return pStream;
}

