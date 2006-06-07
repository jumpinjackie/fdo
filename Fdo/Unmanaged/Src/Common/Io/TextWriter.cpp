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
#include <FdoCommon.h>
#include <stdio.h>
#include <string.h>

FdoIoTextWriter::FdoIoTextWriter(FdoIoStream* stream)
{
    stream->AddRef();
    mStream = stream;
}

FdoIoTextWriter::~FdoIoTextWriter(void)
{
}

FdoIoTextWriter* FdoIoTextWriter::Create( FdoString* fileName )
{
    // Open file for (re)create.
    FdoIoStreamP stream = FdoIoFileStream::Create(fileName, L"w+t" );

    return( FdoIoTextWriter::Create(stream) );
}

FdoIoTextWriter* FdoIoTextWriter::Create( FdoIoStream* stream )
{
    if ( !stream ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"stream",
                L"(NULL)",
                L"FdoIoTextWriter::Create"
            )
        );

    return( new FdoIoTextWriter(stream) );
}

FdoIoStream* FdoIoTextWriter::GetStream()
{
    FdoIoStream* pStream = mStream;
    pStream->AddRef();

    return pStream;
}


void FdoIoTextWriter::Write( FdoString* string )
{
    if ( string ) {
        FdoStringP wrapper(string);

        // Write the string in UTF8 format.
        FdoIoStreamP stream = GetStream();
        stream->Write((FdoByte*)(const char*) wrapper, strlen((const char*) wrapper));
    }
}

void FdoIoTextWriter::WriteLine( FdoString* string )
{
    if ( string ) {
        Write(string);
        //Append the carriage return
        Write(L"\n");
    }
}



