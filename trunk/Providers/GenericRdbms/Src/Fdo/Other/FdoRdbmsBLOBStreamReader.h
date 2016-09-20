#ifndef _FDORDBMSBLOBSTREAMREADER_H_
#define _FDORDBMSBLOBSTREAMREADER_H_
//
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
//

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include "FdoRdbms.h"

///<summary>The FdoRdbmsBLOBStreamReader class derives from FdoBLOBStreamReader and helps with
/// reading a BLOB by reference using LOB locators.
///</summary>
class FdoRdbmsBLOBStreamReader : public FdoBLOBStreamReader
{
protected:

    // Constructs an instance of an FdoRdbmsBLOBStreamReader using the specified arguments.
    FdoRdbmsBLOBStreamReader( FdoIConnection *conn, GdbiQueryResult* gdbiResult, void* value, const FdoInt32 blockSize);

    ~FdoRdbmsBLOBStreamReader();

    void Dispose();

public:

    // Constructs an instance of an FdoBLOB Stream Reader using the specified arguments.

    static FdoRdbmsBLOBStreamReader* Create( FdoIConnection *conn, GdbiQueryResult* gdbiResult, void* value, const FdoInt32 blockSize = 2048 );

    // FdoIStreamReader implementation

    // Gets the size of the BLOB in number of bytes.

    FdoInt64      GetLength();

    // Skips a number of bytes

    void          Skip( const FdoInt32 offset);

    // Returns the current index in the BLOB

    FdoInt64      GetIndex();

    // Reset the current index in the stream. Allows re-reading

    void          Reset();

    // Reads in the next block of items. Use ReadNext( buffer) to read in
    // the entire stream. The caller is responsible to allocate a buffer large
    // enough to store data.
    // [out] buffer - array holding data read in.
    // [in]  offset - index in the array indicating the beginning of the output buffer.
    //     The caller is responsible for allocating a buffer large enough.
    // [in]  count - Input number of items to be read in. If -1 read the entire stream.
    // Throws "out-of-bounds"  exception if not a positive value or -1.
    // Returns the number of bytes actually read in. When 0 (zero) then the
    // end-of-stream was reached.

    FdoInt32      ReadNext( FdoByte * buffer,
                            const FdoInt32 offset = 0,
                            const FdoInt32 size = -1 );

    // Reads in the next block of items. Use ReadNext( buffer) to read in
    // the entire stream. The caller is responsible to create/dispose the FdoArray.
    // [out] buffer – FdoArray object holding data read in.
    // [in]  offset - index in the array indicating the beginning of the output buffer.
    // If the size of the array, then read in append mode. If not zero, overwrite existing data.
    // [in]  count - Input number of items to be read in. If -1 read the entire stream.
    //  Throws "out-of-bounds"  exception if not a positive value or -1.
    // Returns number of items read. When 0 (zero) then the end-of-stream was reached.

    FdoInt32      ReadNext( FdoByteArray * &buffer,
                            const FdoInt32 offset = -1,
                            const FdoInt32 size = -1 );

private:
    DbiConnection*      m_DbiConn;
    FdoRdbmsConnection* mFdoConnection;
    void*               m_LOBlocator;
    GdbiQueryResult*    m_GdbiResult;
    bool                m_EndOfLOB;
    FdoInt64            m_LOBLength;
    FdoInt64            m_CurrentIndex;
    FdoInt32            m_BlockSize;
};

#endif

