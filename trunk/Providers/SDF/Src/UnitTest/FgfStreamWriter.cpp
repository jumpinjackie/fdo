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
#include "stdafx.h"
#include "FgfStreamWriter.h"

#define INITIAL_LENGTH 200
#define ENLARGE_FACTOR 1.5

FgfStreamWriter::FgfStreamWriter()
{
    m_begin_data = (unsigned char*)malloc(INITIAL_LENGTH);
    m_data = m_begin_data;
    m_end_data = (m_begin_data + (INITIAL_LENGTH -1));
}

FgfStreamWriter::~FgfStreamWriter()
{
    if (m_begin_data)
        free(m_begin_data);
}

long FgfStreamWriter::getLength()
{
    return (long)(m_data - m_begin_data + 1);
}

const unsigned char* FgfStreamWriter::getData()
{
    return m_begin_data;
}

void FgfStreamWriter::WriteGeometryType(FgfGeometryType value)
{
    WriteInt((int)value);
}

void FgfStreamWriter::WriteCoordinateType(FgfCoordinateType value)
{
    WriteInt((int)value);
}

/// <summary>
/// Writes an integer to the stream.
/// </summary>
/// <param name="value">The value written to the stream.</param>
void FgfStreamWriter::WriteInt(int value)
{
    EnsureCapacity(sizeof(int));
    *(int*)(m_data) = value;
    m_data += sizeof(int);
}

void FgfStreamWriter::WriteDouble(double value)
{
    EnsureCapacity(sizeof(double));
    *(double*)(m_data) = value;
    m_data += sizeof(double);
}

void FgfStreamWriter::WriteDoubles(double* src, int count)
{
    int size = count*sizeof(double);
    EnsureCapacity(size);
    memcpy(m_data,(const unsigned char*)src,size);
    m_data += size;
}

void FgfStreamWriter::WriteByte(unsigned char value)
{
    EnsureCapacity(sizeof(char));
    *(m_data++) = value;
}

void FgfStreamWriter::WriteChar(const wchar_t value)
{
    EnsureCapacity(sizeof(const wchar_t));
    *(wchar_t*)m_data = value;        
    m_data += sizeof(value);
}


// Frees the byte array and resets the stream writer to its initial state.
void FgfStreamWriter::Clear()
{
    if (m_begin_data)
        free(m_begin_data);
    m_begin_data= (unsigned char*)malloc(INITIAL_LENGTH);
    m_data = m_begin_data;
    m_end_data = (m_begin_data + (INITIAL_LENGTH -1));
}

// makes sure the byte array is big enough
void FgfStreamWriter::EnsureCapacity(int requiredCapacity)
{
    if (m_data + requiredCapacity > m_end_data)
    {
        unsigned char* temp;
        int size, oldsize, offset;
        oldsize = (int)(m_end_data - m_begin_data + 1);
        size = (int)(oldsize*ENLARGE_FACTOR);
        size = (size > (requiredCapacity + oldsize)) ? size : requiredCapacity + oldsize;
        offset = (int)(m_data - m_begin_data);
        temp = (unsigned char*)malloc(size);
        memcpy(temp,(const unsigned char*)m_begin_data,oldsize);
        free(m_begin_data);
        m_begin_data = temp;
        m_data = m_begin_data + offset;
        m_end_data = m_begin_data + size - 1;
    }
}
