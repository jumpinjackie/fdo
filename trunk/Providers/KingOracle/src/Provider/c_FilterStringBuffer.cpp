/*
* Copyright (C) 2006  SL-King d.o.o
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

#include "stdafx.h"
#include "c_FilterStringBuffer.h"

#define D_FILTER_MEM_BLOCK_ALLOC_SIZE    256

c_FilterStringBuffer::c_FilterStringBuffer(void)
{
  m_SqlFilterText =  NULL;
  m_SqlBuffSize = 0;
  m_FirstBuffIndex = 0;
  m_NextBuffIndex = 0;  
}

c_FilterStringBuffer::~c_FilterStringBuffer(void)
{
   if( m_SqlFilterText )
        delete[] m_SqlFilterText;
}

void c_FilterStringBuffer::ClearBuffer(void)
{
  if( m_SqlFilterText )
    delete[] m_SqlFilterText;
        
  m_SqlFilterText =  NULL;
  m_SqlBuffSize = 0;
  m_FirstBuffIndex = 0;
  m_NextBuffIndex = 0;          
}

const char* c_FilterStringBuffer::GetString() 
{ 
  if( !m_SqlFilterText ) return NULL;
  
  return &m_SqlFilterText[m_FirstBuffIndex]; 
}

//
// Make sure that the buffer is large enought for an extra size chars.
void c_FilterStringBuffer::ReallocBuffer( size_t  Size, bool AtEnd )
{
    size_t extraSize;

    if( AtEnd && (m_NextBuffIndex + Size) < m_SqlBuffSize )
        return;

    if( !AtEnd && Size < m_FirstBuffIndex )
        return;

    extraSize = Size * 2; // Need to append or prepend enough at both ends of the buffer

    if( m_SqlFilterText == NULL )
    {
        m_SqlBuffSize = ( extraSize < D_FILTER_MEM_BLOCK_ALLOC_SIZE )?D_FILTER_MEM_BLOCK_ALLOC_SIZE:extraSize;
        m_SqlFilterText = new char[m_SqlBuffSize];
        if( m_SqlFilterText == NULL )  throw FdoFilterException::Create(NlsMsgGetKgOra(M_KGORA_OUT_OF_MEMORY_ERROR, "Memory error"));
        m_FirstBuffIndex = m_NextBuffIndex = m_SqlBuffSize/2;
        m_SqlFilterText[m_NextBuffIndex] = '\0';
    }
    else
    {
        m_SqlBuffSize += (( extraSize < D_FILTER_MEM_BLOCK_ALLOC_SIZE )?D_FILTER_MEM_BLOCK_ALLOC_SIZE:extraSize);
        char  *tmp = new char[m_SqlBuffSize];
        if( tmp == NULL )
            throw FdoFilterException::Create(NlsMsgGetKgOra(M_KGORA_OUT_OF_MEMORY_ERROR, "Memory error"));
        size_t currentSize = strlen( &m_SqlFilterText[m_FirstBuffIndex] );
        strncpy( &tmp[(m_SqlBuffSize-currentSize)/2], &m_SqlFilterText[m_FirstBuffIndex], currentSize );
        m_FirstBuffIndex = (m_SqlBuffSize-currentSize)/2;
        m_NextBuffIndex = m_FirstBuffIndex + currentSize;
        tmp[m_NextBuffIndex] = '\0';

        delete[] m_SqlFilterText;
        m_SqlFilterText = tmp;
    }

}//end of c_FilterStringBuffer::ReallocBuffer

//
// Add a string to the end of the buffer
void c_FilterStringBuffer::AppendString(const char *Str)
{
  size_t slen = strlen( Str ); 
  ReallocBuffer( slen + 1, true );
  strcpy( &m_SqlFilterText[m_NextBuffIndex], Str );
  m_NextBuffIndex += slen;
}

//
// Add a string to the biginning of the buffer
void c_FilterStringBuffer::PrependString(const char *Str)
{
  size_t slen = strlen( Str ); 
  ReallocBuffer( slen + 1, false );
  m_FirstBuffIndex -= slen;
  strncpy(&m_SqlFilterText[m_FirstBuffIndex], Str, slen );    
}
