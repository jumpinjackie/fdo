/*
 * 
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
* 
 */

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#ifdef _WIN32
#include <share.h>
#include <io.h>
#else
#include <wchar.h>
#include <fcntl.h>
#include <unistd.h>
#endif
#include "ShpSpatialIndexFile.h"


//-------------------------------------------------------------------------
//
// METHOD: Constructor.
//
// PURPOSE: Initialize a instance of the ShpSpatialIndexFile object. This constructor
//          is accessible only to derived classes; initialization should be
//          completed by calling the OpenIndexFile() member function.
//
// PARAMETERS:
//
//     None.
//
//-------------------------------------------------------------------------

ShpSpatialIndexFile::ShpSpatialIndexFile()
{
} // end: Constructor


//-------------------------------------------------------------------------
//
// METHOD: Destructor.
//
// PURPOSE: Close the index file and release all dynamically allocated 
//          resources.
//
//-------------------------------------------------------------------------

ShpSpatialIndexFile::~ShpSpatialIndexFile()
{
} // end: Destructor

//-------------------------------------------------------------------------
//
// METHOD: Safe_tcsncpy().
//
// PURPOSE: Copy up to a specified number of bytes (single/double byte char),
//          or characters (Unicode).
//
// PARAMETERS:
//
//     Input:
//
//         source  - passes the source buffer.
//         nToCopy - passes the number of bytes/characters to copy.
//
//     Output:
//
//         dest    - desintation buffer to copy to.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndexFile::Safe_tcsncpy(LPTSTR dest, LPCTSTR source, unsigned nToCopy) const
{
    wcsncpy (dest, source, nToCopy);
} // end: Safe_tcsncpy()


//-------------------------------------------------------------------------
//
// METHOD: GCD().
//
// PURPOSE: Compute the greatest common divisor of two unsigned integers.
//
// PARAMETERS:
//
//     Input:
//
//         x, y - pass the values whose GCD is to be computed for.
//
//     Output:
//
//         None.
//
// RETURNS: Returns the greatest common divisor of x and y.
//
//-------------------------------------------------------------------------

unsigned ShpSpatialIndexFile::GCD(unsigned x, unsigned y) const
{
    // compute the greatest common divisor of the internal and leaf node
    // sizes. The GCD is computed using an iterative version of Euclid's 
    // algorithm

    unsigned r = x % y;

    while (r != 0) {
        x = y;
        y = r;
        r = x % y;
    }

    return y;

} // end: GCD()


//-------------------------------------------------------------------------
//
// METHOD: EncodeUI().
//
// PURPOSE: Method used to encode an unsigned integer into the specified
//          buffer. The integer is encoded as specified in the SDF spec-
//          ification.
//
// PARAMETERS:
//
//     Input
//
//         value     - passes the unsigned integer value.
//         precision - passes the precision (number of bits) in 'value'.
//
//     Output:
//
//         buffer    - passes a pointer to the buffer the encoded version
//                     of 'value' is to be copied to.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndexFile::EncodeUI(unsigned long value, unsigned precision, 
    unsigned char *buffer) const
{
    if (precision == 16) {
        buffer[0] = (unsigned char)((value & 0x0000FF00UL) >> 8);
        buffer[1] = (unsigned char)(value & 0x000000FFUL);
    }
    else {
        buffer[0] = (unsigned char)((value & 0xFF000000UL) >> 24);
        buffer[1] = (unsigned char)((value & 0x00FF0000UL) >> 16);
        buffer[2] = (unsigned char)((value & 0x0000FF00UL) >> 8);
        buffer[3] = (unsigned char)(value & 0x000000FFUL);
    }

} // end: EncodeUI()


//-------------------------------------------------------------------------
//
// METHOD: DecodeUI().
//
// PURPOSE: Method used to decode an unsigned integer from the specified
//          buffer. The integer is encoded as specified in the SDF spec-
//          ification.
//
// PARAMETERS:
//
//     Input:
//
//         buffer    - passes a pointer to the buffer containing the 
//                     encoded unsigned integer.
//         precision - passes the precision (number of bits) of the
//                     encoded integer.
//
//     Output:
//
//         None.
//
// RETURNS: Returns the decoded unsigned integer as an unsigned long.
//
//-------------------------------------------------------------------------

unsigned long ShpSpatialIndexFile::DecodeUI(unsigned char *buffer, unsigned precision)
    const
{
    unsigned long value;

    if (precision == 16)
        value = ((unsigned long)buffer[0] << 8) + 
                 (unsigned long)buffer[1];
    else
        value = ((unsigned long)buffer[0] << 24) + 
                ((unsigned long)buffer[1] << 16) + 
                ((unsigned long)buffer[2] << 8)  + 
                 (unsigned long)buffer[3];

    return value;

} // end: DecodeUI()


//-------------------------------------------------------------------------
//
// METHOD: EncodeString().
//
// PURPOSE: Method used to encode a string into the specified buffer. The
//          string is encoded as specified in the SDF specification.
//
// PARAMETERS:
//
//     Input
//
//         str    - passes a pointer to the null-terminated string.
//
//     Output:
//
//         buffer - passes a pointer to the buffer the encoded version of
//                  the string is to be copied to.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndexFile::EncodeString(LPCTSTR str, unsigned char *buffer) const
{
    size_t strLength = ::wcslen(str) * sizeof(TCHAR);
    int i;

    if (strLength < 0xFF) {
        buffer[0] = (unsigned char)strLength;
        i = 1;
    }
    else {
        buffer[0] = 0xFF;
        EncodeUI((unsigned long)strLength, 16, &buffer[1]);
        i = 3;
    }

    unsigned char *tmpPtr = (unsigned char *)str;

    for (size_t j = 0; j < strLength; i++, j++) 
        buffer[i] = tmpPtr[j];

} // end: EncodeString()


//-------------------------------------------------------------------------
//
// METHOD: DecodeString().
//
// PURPOSE: Method used to decode a string from a buffer. The string is 
//          encoded as specified in the SDF specification.
//
// PARAMETERS:
//
//     Input:
//
//         buffer - passes a pointer to the buffer containing the 
//                  encoded string.
//
//     Output:
//
//         str    - pointer to a buffer the decoded (and null-terminated)
//                  string is to be copied to. The buffer is assumed to
//                  be large enough.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndexFile::DecodeString(unsigned char *buffer, LPTSTR str) const
{
    unsigned strLength = (unsigned)buffer[0];
    unsigned i;
    unsigned j;

    if (strLength == 0xFF) {
        strLength = unsigned(DecodeUI(&buffer[1], 16));
        i = 3;
    }
    else 
        i = 1;

    unsigned char *tmpPtr = (unsigned char *)str;

    for (j = 0; j < strLength; i++, j++) 
        tmpPtr[j] = buffer[i];
    tmpPtr[j] = L'\0';

} // end: DecodeString()


//-------------------------------------------------------------------------
//
// METHOD: EncodeFloat().
//
// PURPOSE: Method used to encode a floating point value into the specified
//          buffer. The floating point value is encoded as specified in the
//          SDF specification.
//
// PARAMETERS:
//
//     Input
//
//         value     - passes the floating point value.
//         precision - passes the precision (number of bits) in 'value'.
//
//     Output:
//
//         buffer    - passes a pointer to the buffer the encoded version
//                     of 'value' is to be copied to.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndexFile::EncodeFloat(double value, unsigned precision, 
    unsigned char *buffer) const
{
    if (precision == 32) {
        union {
            float f;
            unsigned long ul;
        } floatToLong;

        floatToLong.f = float(value);
        buffer[0] = (unsigned char)((floatToLong.ul & 0xFF000000) >> 24);
        buffer[1] = (unsigned char)((floatToLong.ul & 0x00FF0000) >> 16);
        buffer[2] = (unsigned char)((floatToLong.ul & 0x0000FF00) >> 8);
        buffer[3] = (unsigned char)(floatToLong.ul  & 0x000000FF);
    }
    else {
        union {
            double d;
            unsigned long ul[2];
        } doubleToLong;

        doubleToLong.d = value;
        buffer[0] = (unsigned char)((doubleToLong.ul[1] & 0xFF000000) >> 24);
        buffer[1] = (unsigned char)((doubleToLong.ul[1] & 0x00FF0000) >> 16);
        buffer[2] = (unsigned char)((doubleToLong.ul[1] & 0x0000FF00) >> 8);
        buffer[3] = (unsigned char)(doubleToLong.ul[1]  & 0x000000FF);
        buffer[4] = (unsigned char)((doubleToLong.ul[0] & 0xFF000000) >> 24);
        buffer[5] = (unsigned char)((doubleToLong.ul[0] & 0x00FF0000) >> 16);
        buffer[6] = (unsigned char)((doubleToLong.ul[0] & 0x0000FF00) >> 8);
        buffer[7] = (unsigned char)(doubleToLong.ul[0]  & 0x000000FF);
    }

} // end: EncodeFloat()


//-------------------------------------------------------------------------
//
// METHOD: DecodeFloat().
//
// PURPOSE: Method used to decode a floating point value from the specified
//          buffer. The floating point value is encoded as specified in the
//          SDF specification.
//
// PARAMETERS:
//
//     Input:
//
//         buffer    - passes a pointer to the buffer containing the 
//                     encoded floating point value.
//         precision - passes the precision (number of bits) of the
//                     encoded value.
//
//     Output:
//
//         None.
//
// RETURNS: Returns the decoded floating point value as a double.
//
//-------------------------------------------------------------------------

double ShpSpatialIndexFile::DecodeFloat(unsigned char *buffer, unsigned precision) const
{
    if (precision == 32) {
        union { 
            float f;
            unsigned long ul;
        } longToFloat;

        longToFloat.ul = buffer[0];
        longToFloat.ul = (longToFloat.ul << 8) | buffer[1];
        longToFloat.ul = (longToFloat.ul << 8) | buffer[2];
        longToFloat.ul = (longToFloat.ul << 8) | buffer[3];

        return longToFloat.f;
    }
    else {
        union { 
            double d;
            unsigned long ul[2];
        } longToDouble;

        longToDouble.ul[1] = buffer[0];
        longToDouble.ul[1] = (longToDouble.ul[1] << 8) | buffer[1];
        longToDouble.ul[1] = (longToDouble.ul[1] << 8) | buffer[2];
        longToDouble.ul[1] = (longToDouble.ul[1] << 8) | buffer[3];
        longToDouble.ul[0] = buffer[4];
        longToDouble.ul[0] = (longToDouble.ul[0] << 8) | buffer[5];
        longToDouble.ul[0] = (longToDouble.ul[0] << 8) | buffer[6];
        longToDouble.ul[0] = (longToDouble.ul[0] << 8) | buffer[7];

        return longToDouble.d;
   }

} // end: DecodeFloat()


