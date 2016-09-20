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

#include "stdafx.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsBLOBStreamReader.h"
#include "FdoRdbmsException.h"

///////////////////////////////////////////////////////////////////////////////
FdoRdbmsBLOBStreamReader* FdoRdbmsBLOBStreamReader::Create( FdoIConnection *conn, GdbiQueryResult* gdbiResult, void* value, const FdoInt32 blockSize )
{
    return new FdoRdbmsBLOBStreamReader( conn, gdbiResult, value, blockSize );
}

///////////////////////////////////////////////////////////////////////////////
FdoRdbmsBLOBStreamReader::FdoRdbmsBLOBStreamReader( FdoIConnection *conn, GdbiQueryResult* gdbiResult, void *value, const FdoInt32 blockSize )
{
    if (conn == NULL || gdbiResult == NULL || value == NULL || blockSize <= 0 )
    {
        throw FdoRdbmsException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    }
    m_DbiConn = NULL;
    mFdoConnection = dynamic_cast<FdoRdbmsConnection*>(conn);
    if( mFdoConnection )
    {
        mFdoConnection->AddRef();
        m_DbiConn = mFdoConnection->GetDbiConnection();
    }
    m_BlockSize = blockSize;
    m_GdbiResult = gdbiResult;
    m_LOBlocator = value;
    m_EndOfLOB = FALSE;
    m_LOBLength = -1;
    m_CurrentIndex = 0;
}

///////////////////////////////////////////////////////////////////////////////
FdoRdbmsBLOBStreamReader::~FdoRdbmsBLOBStreamReader()
{
    if ( m_DbiConn != NULL && m_GdbiResult != NULL && m_LOBlocator != NULL )
    {
        // Needs more investigation
        //gdbiResult->lob_destroy_ref( m_LOBlocator );
    }
    if( mFdoConnection )
        mFdoConnection->Release();
}

///////////////////////////////////////////////////////////////////////////////
void FdoRdbmsBLOBStreamReader::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
void  FdoRdbmsBLOBStreamReader::Skip( const FdoInt32 offset )
{
    if ( offset < 0 )
    {
        throw FdoRdbmsException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    }
    else if ( offset > 0 )
    {
        ReadNext( NULL, 0, offset );
    }
    else
    {
        // do nothing
    }
}

///////////////////////////////////////////////////////////////////////////////
FdoInt64  FdoRdbmsBLOBStreamReader::GetIndex()
{
    return m_CurrentIndex;
}

/////////////////////////////////////////////////////////////////////////////
void   FdoRdbmsBLOBStreamReader::Reset()
{
    // First read till the end of the stream -- counts on Oracle's undocumented behaviour that
    // loops to beginning of blob after reading off the end
    if ( m_CurrentIndex != 0 && !m_EndOfLOB )
    {
        FdoInt64    num_blocks = ( m_LOBLength - m_CurrentIndex ) / m_BlockSize;
        FdoInt32    remainder = (FdoInt32) (m_LOBLength - num_blocks * m_BlockSize);

        for ( FdoInt64 i = 0; i < num_blocks; i++ )
            Skip( m_BlockSize );

        Skip( remainder );
    }

    // Reset members
    m_EndOfLOB = false;
    m_CurrentIndex = 0;
}

///////////////////////////////////////////////////////////////////////////////
FdoInt64 FdoRdbmsBLOBStreamReader::GetLength()
{
    if ( m_LOBLength == -1 )
    {
        unsigned int size;
        m_GdbiResult->LobGetSize( m_LOBlocator, (unsigned int *)&size);
        m_LOBLength = (FdoInt64) size;
    }
    return m_LOBLength;
}

///////////////////////////////////////////////////////////////////////////////
FdoInt32  FdoRdbmsBLOBStreamReader::ReadNext(   FdoByte * buffer,
                                                const FdoInt32 offset,
                                                const FdoInt32 size )
{
    FdoInt32        chunk_size;
    FdoInt32        chunk_size_out = 0;
    FdoByte         *buffer2 = buffer;

    // Quick return if done
    if ( m_EndOfLOB )
        return 0;

    chunk_size = ( size == -1 ) ? (unsigned int) GetLength() : size;

    // Allow NULL input to accomodate Skip(). Allocate a temporary buffer to read in
    if ( buffer2 == NULL )
        buffer2 = new FdoByte[chunk_size];

    m_GdbiResult->LobReadNext(
                        m_LOBlocator,
                        RDBI_BLOB,
                        chunk_size,
                        (char *) &buffer2[offset],
                        (unsigned int*) &chunk_size_out,
                        (int *) &m_EndOfLOB );

    if ( buffer == NULL )
        delete[] buffer2;

    m_CurrentIndex += chunk_size_out;

    return chunk_size_out;
}

///////////////////////////////////////////////////////////////////////////////
FdoInt32  FdoRdbmsBLOBStreamReader::ReadNext(   FdoByteArray * &value,
                                                const FdoInt32 offset,
                                                const FdoInt32 size )
{
    FdoInt32        chunk_size;
    FdoInt32        chunk_size_out;
    FdoInt32        currSize;
    FdoInt32        newSize;
    FdoInt32        readHere = 0;
    FdoByte         *buffer = NULL;

    // Validate input parameters
    if ( ( size <= 0 && size != -1 ) || ( offset < 0 ) )
    {
        throw FdoRdbmsException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    }

    // Quick return if done
    if ( m_EndOfLOB )
        return FALSE;

    chunk_size = ( size == -1 ) ? (unsigned int) GetLength() : size;

    // The byte array cannot be NULL (it is a reference)
    currSize = value->GetCount();

    // Overwrite the buffer, but the offset should be within current buffer.
    if ( offset > currSize  )
    {
        throw FdoRdbmsException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    }
    newSize = offset + chunk_size;
    readHere = offset * sizeof(FdoByte);

    // Enlarge the array if necessary
    value = FdoByteArray::SetSize( value, newSize );

    // Buffer to read in
    buffer = value->GetData();

    // Read from LOB
    m_GdbiResult->LobReadNext(
                        m_LOBlocator,
                        RDBI_BLOB,
                        chunk_size,
                        (char *) &buffer[readHere],
                        (unsigned int*) &chunk_size_out,
                        (int *) &m_EndOfLOB );

    // Adjust the size of the array with the returned chunk size
    if ( offset + chunk_size_out < currSize )
    {
        // The size of the array is smaller than the initial one.
        // The array should never shrink in order to allow overwriting.
        // Reset the array to its original size.
        value = FdoByteArray::SetSize( value, currSize );
    }
    else
    {
        int  diff = chunk_size - chunk_size_out;
        value = FdoByteArray::SetSize( value, newSize - diff );
    }

    m_CurrentIndex += chunk_size_out;

    // m_EndOfLOB has been set and it will be checked at the next read.
    return chunk_size_out;
}
