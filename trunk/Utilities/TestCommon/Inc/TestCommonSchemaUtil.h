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

#ifndef TESTCOMMONSCHEMAUTIL___H
#define TESTCOMMONSCHEMAUTIL___H	1

#ifdef _WIN32
#pragma once
#endif


//
// Schema-based test utilities common to many providers
class TestCommonSchemaUtil
{
public:
    // Delete a class (if bDeleteRowsOnly==false) or delete the rows of a class (if bDeleteRowsOnly==true):
    static void CleanUpClass(FdoIConnection *connection, const wchar_t* schema_name, const wchar_t* class_name, bool bDeleteRowsOnly=false);
};

#endif // TESTCOMMONSCHEMAUTIL___H

