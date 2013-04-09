/*
* Copyright (C) 2007  Haris Kurtagic
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

#ifndef _c_FilterStringBuffer_h
#define _c_FilterStringBuffer_h

class c_FilterStringBuffer
{
public:
  c_FilterStringBuffer(void);
public:
  ~c_FilterStringBuffer(void);
  
protected:
    char* m_SqlFilterText;  // Buffer used to encode the filter SQL conversion
    size_t m_SqlBuffSize;    // The size of the SQL buffer
    size_t m_FirstBuffIndex;  // The index of the start of the sql string
    size_t m_NextBuffIndex;   // The index of the next empty string    
    
public:
    const char* GetString();  
    void AppendString(const char *Str);

    void PrependString(const char *Str);    
    
    void ClearBuffer(void);
      
protected:
  void ReallocBuffer( size_t  Size , bool AtEnd );
    
};

#endif