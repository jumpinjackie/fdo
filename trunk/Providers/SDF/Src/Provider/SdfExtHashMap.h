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
#ifndef SDF_EXT_HASH_MAP_H
#define SDF_EXT_HASH_MAP_H
// 

#ifdef WIN32
#include <hash_map>
#else
#include <ext/hash_map>
namespace stdext = ::__gnu_cxx;
using namespace std;

namespace __gnu_cxx
{
    template<> struct hash< void* >
    {
        size_t operator()( void* const& x ) const
        {
            return hash< int >()( (int)x );
        }
    };
}

#endif

#endif


