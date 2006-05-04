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
#include <string.h>
#include <limits.h>
#include <Common.h>

FdoIoMemoryStream::FdoIoMemoryStream( FdoSize bufferSize ) :
    mBufferSize(bufferSize),
    mLength(0),
    mPos(0)
{
    mBuffers = Buffers::Create();
}

FdoIoMemoryStream::~FdoIoMemoryStream(void)
{
}

FdoIoMemoryStream* FdoIoMemoryStream::Create( FdoSize bufferSize )
{
    return new FdoIoMemoryStream( bufferSize);
}

FdoSize FdoIoMemoryStream::Read( FdoByte* buffer, FdoSize count )
{
    // Validate the parameters.

    if ( !buffer ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"buffer",
                L"(NULL)",
                L"FdoIoMemoryStream::Read"
            )
        );

    // The data to read can include parts of or complete buffers. It can also
    // span buffers.

    // Create a temporary buffer for reading the data from each internal buffer.
    FdoIoBufferStreamP   bufIter = FdoIoBufferStream::Create( buffer, count );
    FdoSize              bufNum = 0;
    FdoSize              bufOffset = 0;

    // Given the current position, get the corresponding internal buffer and 
    // offset with the buffer. Reading will start in this buffer.
    GetBufPosn( bufNum, bufOffset );

    // Read until end of this stream is reached or we have enough bytes.
    while ( (bufNum < ((FdoSize) mBuffers->GetCount())) && (bufIter->GetIndex() < bufIter->GetLength() ) ) {

        // Get the current internal buffer.
        FdoIoBufferStreamP intBuf = mBuffers->GetItem(bufNum);

        // Position the buffer to the current offset.
        intBuf->Reset();
        intBuf->Skip( bufOffset );

        // Write the remainder of the internal buffer to the temporary buffer or
        // the number of bytes left to read, whichever is less.
        bufIter->Write( intBuf, (FdoSize)(bufIter->GetLength() - bufIter->GetIndex()) );

        // Increment to read from the next internal buffer.
        bufNum++;
        bufOffset = 0;
    }

    // Update the current position.
    mPos += bufIter->GetIndex();

    return (FdoSize) bufIter->GetIndex();
}

void FdoIoMemoryStream::Write( FdoByte* buffer, FdoSize count )
{
    if ( !buffer ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"buffer",
                L"(NULL)",
                L"FdoIoMemoryStream::Write"
            )
        );

    // Wrap a stream around the buffer and delegate to write from stream.
    FdoIoBufferStreamP  bufStream = FdoIoBufferStream::Create( buffer, count );

    Write( bufStream, 0 ); 
}

void FdoIoMemoryStream::Write( FdoIoStream* stream, FdoSize count )
{
    // Validate the parameters.

    if ( !stream ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"stream",
                L"(NULL)",
                L"FdoIoMemoryStream::Write"
            )
        );

    // Find out how many bytes are after the input streams position.
    FdoInt64 streamRemaining = stream->GetLength() - stream->GetIndex();

    // Amount to write is the lesser of the remaining bytes and the given limit.
    FdoInt64 lCount = (count > streamRemaining || count == 0) ? streamRemaining : count;
    FdoSize bufNum = 0;
    FdoSize bufOffset = 0;

    // Convert this stream's position to position in the right internal buffer.
    GetBufPosn( bufNum, bufOffset );

    // Write the data to this internal and its following buffers, until all bytes
    // have been written.
    while ( lCount > 0 ) {
        // Our buffer array can't have more than LONG_MAX members
        if ( bufNum >= LONG_MAX )
            throw FdoException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(FDO_34_MEMORYSTREAMOVERFLOW),
                    L"FdoIoMemoryStream::Write",
                    L"FdoIoMemoryStream",
                    mBufferSize
                )
            );

        // Create another internal buffer if at end of this stream.
        while ( bufNum >= ((FdoSize) mBuffers->GetCount()) ) {
            FdoIoBufferStreamP buffer = FdoIoBufferStream::Create( mBufferSize );
            mBuffers->Add( buffer );
            bufOffset = 0;
        }

        // Get the current internal buffer
        FdoIoBufferStreamP buffer = mBuffers->GetItem(bufNum);

        // Position buffer at its offset.
        buffer->Reset();
        buffer->Skip( bufOffset );

        // figure out the amount to write to this internal buffer.
        // lessor of amount remaining to write and amount remaining in this buffer.
        FdoSize writeCount = mBufferSize - bufOffset;
        writeCount = (lCount < writeCount) ? ((FdoSize)lCount) : writeCount;

        // Write to the internal buffer
        buffer->Write( stream, writeCount );

        // Bump down bytes remaining to write by amount written
        lCount -= writeCount;

        // Update current stream position
        mPos += writeCount;

        // Move on to the next internal buffer.
        bufNum++;
        bufOffset = 0;
    }

    // Update the total stream length.
    mLength = (mPos > mLength) ? mPos : mLength;
}

void FdoIoMemoryStream::SetLength( FdoInt64 length )
{
    FdoSize              bufNum = 0;
    FdoSize              bufOffset = 0;

    mLength = (length > mLength) ? mLength : length;
    mPos = ( mPos > mLength ) ? mLength : mPos;

    // Convert this stream's position to position in the right internal buffer.
    GetBufPosn( bufNum, bufOffset );

    // Empty out any buffers after the current buffer. 
    for ( FdoSize i = bufNum; i < (FdoSize) mBuffers->GetCount(); i++ ) {
        FdoIoBufferStreamP buffer = mBuffers->GetItem(i);
        buffer->SetLength( bufOffset );
        bufOffset = 0;
    }
}


void FdoIoMemoryStream::Skip( FdoInt64 offset )
{
    mPos += offset;
    mPos = ( mPos < 0 ) ? 0 : mPos;
    mPos = ( mPos > mLength ) ? mLength : mPos;
}

void FdoIoMemoryStream::Reset()
{
    mPos = 0;
}

void FdoIoMemoryStream::GetBufPosn( FdoSize& bufNum, FdoSize& bufOffset )
{
    bufNum = (FdoSize)(mPos / mBufferSize);
    bufOffset = (FdoSize)(mPos % mBufferSize);
}


