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
#ifndef FDOCOMMONBINARYREADER_H
#define FDOCOMMONBINARYREADER_H

#include <FdoCommonPropertyIndex.h>

#define USE_INTERNALHASH 1

#ifndef USE_INTERNALHASH
#include <map>
#else
#include <FdoCommonHash.h>
#endif

#ifndef _WIN32
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

class FdoCommonBinaryReader : public FdoDisposable
{
    class CachedBuffer
    {
        wchar_t* m_buffer;
        size_t m_length;
        size_t m_capacity;
    public:
        CachedBuffer()
        {
            m_buffer = NULL;
            m_length = m_capacity = 0;
        }
        CachedBuffer(size_t size)
        {
            m_buffer = new wchar_t[size];
            m_capacity = size;
            m_length = 0;
        }
        ~CachedBuffer()
        {
            delete [] m_buffer;
        }
    
        wchar_t* Data() { return m_buffer; }
        size_t Length() { return m_length; }
        size_t Capacity() { return m_capacity; }
        void SetLength(size_t val)
        {
            m_length = val;
        }
        void Reset()
        {
            m_length = 0;
        }
        void EnsureSize(size_t size)
        {
            if (m_capacity < size)
            {
                delete [] m_buffer;
                m_buffer = new wchar_t[size];
                m_capacity = size;
                m_length = 0;
            }
        }
    };

#ifndef USE_INTERNALHASH
    typedef std::map <int, CachedBuffer*> CachedBufferMap;
    typedef std::pair <int, CachedBuffer*> CachedBufferPair;
    typedef CachedBufferMap::iterator CachedBufferIterator;
#else
    typedef FdoCommonTypes::hash<int, CachedBuffer*> CachedBufferMap;
    typedef FdoCommonTypes::pair<int, CachedBuffer*> CachedBufferPair;
    typedef CachedBufferMap::iterator CachedBufferIterator;
#endif

    class CachedBufferManager
    {
        CachedBufferMap m_mapPosition;
        CachedBuffer** m_buffers;
        size_t m_count;
        size_t m_firstFree;
        size_t m_capacity;
    
    private:
        void resize()
        {
            size_t old_capacity = m_capacity;
            m_capacity = (m_capacity == 0) ? 8 : (size_t)(1.4 * m_capacity);
            CachedBuffer** newArray = new CachedBuffer*[m_capacity];
            for (size_t i = 0; i < old_capacity; i++)
                newArray[i] = m_buffers[i];

            delete[] m_buffers;
            m_buffers = newArray;
        }
    public:
        CachedBufferManager()
        {
            m_firstFree = m_count = m_capacity = 0;
            m_buffers = NULL;
        }
        ~CachedBufferManager()
        {
            for (size_t i = 0; i < m_count; i++)
                delete m_buffers[i];
            delete[] m_buffers;
        }
        void Reset()
        {
            for (size_t i = 0; i < m_firstFree; i++)
                m_buffers[i]->Reset();
            m_mapPosition.clear();
            m_firstFree = 0;
        }

        CachedBuffer* GetBuffer(int pos)
        {
            CachedBufferIterator it = m_mapPosition.find(pos);
            return (it == m_mapPosition.end()) ? NULL : it->second;
        }

        CachedBuffer* GetFreeBuffer(int pos, size_t size)
        {
            if (m_firstFree < m_count)
            {
                CachedBuffer* buff = m_buffers[m_firstFree];
                buff->EnsureSize(size);
                m_firstFree++;
                m_mapPosition[pos] = buff;
                return buff;
            }
            if (m_count >= m_capacity)
                resize();
            CachedBuffer* buff = new CachedBuffer(size);
            m_buffers[m_count] = buff;
            m_firstFree++;
            m_count++;
            m_mapPosition[pos] = buff;
            return buff;
        }
    };

public:
    FdoCommonBinaryReader(void) {};   // 0-arg constructor to please FdoPtr::operator->
    FdoCommonBinaryReader(unsigned char* data, int len);

    void Reset(unsigned char* data, int len);
    void SetPosition(int offset);
    int GetPosition();
    unsigned char* GetDataAtCurrentPosition();
    unsigned GetDataLen();
    int PositionReader(int recordIndex, FdoCommonPropertyIndex *propIndex);

    double ReadDouble();
    float ReadSingle();
    short ReadInt16();
    unsigned short ReadUInt16();
    int ReadInt32();
    unsigned ReadUInt32();
    FdoInt64 ReadInt64();
    unsigned char ReadByte();
    char ReadChar();
    wchar_t* ReadString(); 
    wchar_t* ReadRawString(unsigned mbstrlen);
    FdoDateTime ReadDateTime();
    //add more as needed...

protected:

    virtual ~FdoCommonBinaryReader();

private:

    unsigned char* m_data;
    unsigned m_len;
    unsigned m_pos;

    CachedBufferManager m_bufferCache;
};

#endif // FDOCOMMONBINARYREADER_H


