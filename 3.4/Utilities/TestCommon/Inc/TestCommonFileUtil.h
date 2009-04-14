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
 * Revision Control Modification History
 *
 *         $Id:  $
 *     $Author:  $
 *   $DateTime:  $
 *     $Change:  $
 * 
 */

#ifndef TESTCOMMONFILEUTIL___H
#define TESTCOMMONFILEUTIL___H	1

#ifdef _WIN32
#pragma once
#endif


//
// File-based test utilities common to many providers
class TestCommonFileUtil
{
public:
    // Compare the contents of 2 files, and return -1 if they don't match, 0 otherwise
    static FdoInt32 CompareFiles( const char* file1Name, const char* file2Name );

    // Compare the contents of 2 files, and throw a CPPUNIT exception if they do not match
    static void CompareFilesAndThrow( const char* masterFileName, const char* outFileName );

    // Compare the contents of 2 files, with a start byte offset and the number of bytes to compare (-1 to compare all bytes)
    static void CompareFiles (const wchar_t* file1Name, const wchar_t* file2Name, unsigned long startByteOffset = 0L, unsigned long bytesToCompare = (unsigned long) -1L);
};

#endif // TESTCOMMONFILEUTIL___H

