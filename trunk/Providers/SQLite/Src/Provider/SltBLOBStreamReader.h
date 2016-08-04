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
 */

#ifndef STLBLOBSTREAMREADER_H
#define STLBLOBSTREAMREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "Fdo/Expression/BLOBStreamReader.h"

class SltBLOBStreamReader:
    public FdoBLOBStreamReader
{
public:
    SltBLOBStreamReader(FdoLOBValue* val);
    ~SltBLOBStreamReader(void);
    void Dispose(void)  { delete this; };
    /// <summary>Get the size of a stream.</summary>
    /// <returns>Returns the size of the data source in number of items </returns> 
    virtual FdoInt64   GetLength();

    /// <summary> Skips a number of items </summary>
    /// <param name="offset">Number of items to skip. Must be a positive number </param> 
    /// <returns>Returns nothing </returns> 
    virtual void       Skip(const FdoInt32 offset);

    /// <summary> Get the current index in the stream </summary>
    /// <returns>Returns the current index in the stream in number of items </returns> 
    virtual FdoInt64   GetIndex();

    /// <summary> Reset the current index in the stream. Allows re-reading </summary>
    /// <returns>Returns nothing </returns> 
    virtual void       Reset();

    /// <summary> Reads in the next block of items. Use ReadNext( buffer) to read in 
    /// the entire stream. </summary>
    /// <param name="buffer"> Buffer holding data read in </param> 
    /// <param name="offset"> Index in the buffer indicating the beginning of the output buffer. 
    /// The caller is responsable for allocating a buffer of a proper size. </param> 
    /// <param name="count">  Number of items to be read in. If -1 read the entire stream.
    /// Throws "invalid parameter"  exception if not a positive value or -1.</param> 
    /// <returns> Returns the number of items actually read in. When 0 (zero) then the 
    /// end-of-stream was reached. </returns> 

    virtual FdoInt32   ReadNext( FdoByte* buffer, 
                                        const FdoInt32 offset = 0,
                                        const FdoInt32 count = -1);

    /// </summary> Reads in the next block of items. Use ReadNext( buffer) to read in 
    /// the entire stream. The caller is responsible to create/dispose the FdoArray. </summary>
    /// <param name="buffer"> FdoArray object holding data read in. Cannot be NULL </param> 
    /// <param name="offset"> Index in the array indicating the beginning of the output buffer. 
    /// If the end of the buffer, then read in appending mode (store at the end). 
    /// Otherwise, overwrite the buffer. Throws "invalid parameter" exception if not in this range.</param> 
    /// <param name="count">  Number of items to be read in. If -1 read the entire stream.
    ///  Throws "invalid parameter"  exception if not a positive value or -1.</param> 
    /// <returns> Returns number of items read.</returns> 
    virtual FdoInt32    ReadNext(  FdoArray<FdoByte> * &buffer, 
                                            const FdoInt32 offset = 0,
                                            const FdoInt32 count = -1 );
private:
    FdoInt32      m_BlobIndex;
    FdoByteArray* m_Blob;
};

#endif //STLBLOBSTREAMREADER_H