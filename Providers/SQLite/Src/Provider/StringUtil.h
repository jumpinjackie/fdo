// 
//  
//  Copyright (C) 2009 Autodesk Inc.
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

#ifndef SLTSTRINGUTIL_H
#define SLTSTRINGUTIL_H

#include "SltConversionUtils.h"

//Maintains a list of strings, using minimal number of memory allocations.
//Used to build up SQL query strings.
class StringList
{
public:
    StringList(size_t len = 256)
    {
        _next = 0;
        _len = len;
        _buf = new char[_len];
    }

    ~StringList()
    {
        delete [] _buf;
    }

    void Reserve(size_t numprops)
    {
        _offsets.reserve(numprops);
    }

    void Add(const wchar_t* ws)
    {
        size_t wlen = wcslen(ws);
        size_t clen = 4*wlen+1;
        char* mbs = (char*)alloca(clen);
        size_t mbslen = W2A_FAST(mbs, clen, ws, wlen);
        Add(mbs, mbslen);
    }

    void Add(const char* s, size_t len)
    {
        size_t len0 = len + 1; //+1 for the last null-terminating byte
        MakeRoom(len0);

        memcpy(&_buf[_next], s, len0); 
        _offsets.push_back(_next);
        _next += len0;
    }

    inline int Count()
    {
        return (int)_offsets.size();
    }

    inline const char* Get(int i)
    {
        return &_buf[_offsets[i]];
    }

private:

    void MakeRoom(size_t len)
    {
        if (_next + len >= _len)
        {
            size_t nlen = std::max<size_t>(2*_len, _next + len);
            char* nbuf = new char[nlen];
            memcpy(nbuf, _buf, _next);
            delete[] _buf;
            _buf = nbuf;
            _len = nlen;
        }
    }

    size_t _next;
    char* _buf;
    size_t _len;
    std::vector<size_t> _offsets;
};


//Does exactly what its name implies, but faster than STL's string appending,
//at least for our use case
class StringBuffer
{
public:
    StringBuffer(size_t len = 256)
    {
        _len = len;
        if (len)
        {
            _buf = new char[len];
            *_buf = 0; //initalize to empty string
        }
        else
            _buf = NULL;

        _pos = 0;
    }

    StringBuffer(const char* initial)
    {
        size_t len = strlen(initial);
        _len = len + 1;
        _buf = new char[_len];
        *_buf = 0; //initalize to empty string
        _pos = 0;

        Append(initial, len);
    }

    ~StringBuffer()
    {
        delete[] _buf;
    }

    void Reset()
    {
        _pos = 0;
        *_buf = 0;
    }

    size_t Length()
    {
        return _pos;
    }

    //append a string of known length
    void Append(const char* str, size_t len)
    {
        size_t len0 = len + 1; //length inluding zero terminator
        MakeRoom(len0);
        memcpy(&_buf[_pos], str, len);
        *(&_buf[_pos] + len) = '\0';
        _pos += len;
    }

    void Append(const wchar_t* ws)
    {
        size_t wlen = wcslen(ws);
        size_t clen = 4*wlen+1;
        char* mbs = (char*)alloca(clen);
        size_t mbslen = W2A_FAST(mbs, clen, ws, wlen);
        Append(mbs, mbslen);
    }

    //Appends the string representation of the given integer
    //to the buffer
    void Append(int value)
    {
        char tmp[32];
        _snprintf(tmp, 32, "%d", value);
        Append(tmp);
    }

    //append a string of unknown length
    inline void Append(const char* str)
    {
        Append(str, strlen(str));
    }

    void AppendSQuoted(const char* str)
    {
        Append("'", 1);
        Append(str);
        Append("'", 1);
    }

    void AppendSQuotedHandleNull(const wchar_t* str)
    {
        if (str)
            AppendSQuoted(str);
        else
            Append("null");
    }

    void AppendSQuoted(const wchar_t* str)
    {
        Append("'", 1);
        Append(str);
        Append("'", 1);
    }

    void AppendDQuoted(const char* str)
    {
        Append("\"", 1);
        Append(str);
        Append("\"", 1);
    }

    void AppendDQuoted(const wchar_t* str)
    {
        Append("\"", 1);
        Append(str);
        Append("\"", 1);
    }

    inline char* Data()
    {
        return _buf ? _buf : (char*)"";
    }

private:

    void MakeRoom(size_t len) //len is raw bytes, includes null terminator
    {
        if (_pos + len > _len)
        {
            size_t nlen = std::max<size_t>(_len*2, _pos + len);
            char* nbuf = new char[nlen];
            if (_buf) 
                memcpy(nbuf, _buf, _pos+1);
            delete[] _buf;
            _buf = nbuf;
            _len = nlen;
        }
    }

    char* _buf;
    size_t _len;
    size_t _pos; //position of current zero terminator (= position of next write)
};

#endif



