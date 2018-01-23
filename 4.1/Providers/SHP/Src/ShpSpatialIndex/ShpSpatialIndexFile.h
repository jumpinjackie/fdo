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

#ifndef SHPSPATIALINDEXFILE_H
#define SHPSPATIALINDEXFILE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <FdoCommonFile.h>

//-------------------------------------------------------------------------
//
// PURPOSE: Declaration of the ShpSpatialIndexFile class. The ShpSpatialIndexFile class serves
//          as a base class for both the SpatialIndexFile and KeyIndexFile
//          classes, and implements methods that are common to both.
//
//-------------------------------------------------------------------------
class ShpSpatialIndexFile : public FdoCommonFile
{

protected:
    // constructor/destructor

    ShpSpatialIndexFile();
    virtual ~ShpSpatialIndexFile();

public:

    // method to perform a "safe" _tcsncpy(), avoiding copying partial double-
    // byte characters at the end of the buffer
    
    void Safe_tcsncpy(LPTSTR dest, LPCTSTR source, unsigned nToCopy) const;

    // methods to encode/decode integers, strings, and floating point numbers

    void EncodeUI(unsigned long value, unsigned precision, unsigned char *buffer) const;
    unsigned long DecodeUI(unsigned char *buffer, unsigned precision) const;
    void EncodeString(LPCTSTR str, unsigned char *buffer) const ;
    void DecodeString(unsigned char *buffer, LPTSTR str) const;
    void EncodeFloat(double value, unsigned precision, unsigned char *buffer) const;
    double DecodeFloat(unsigned char *buffer, unsigned precision) const;

    // method to compute the greatest common divisor of two integers

    unsigned GCD(unsigned x, unsigned y) const;
};    

#endif // SHPSPATIALINDEXFILE_H

