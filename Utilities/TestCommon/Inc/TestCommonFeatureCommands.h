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
 *         $Id: //providers_reliant/TestCommon/Inc/TestCommonFeatureCommands.h#1 $
 *     $Author: dalcoup $
 *   $DateTime: 2006/02/24 15:47:54 $
 *     $Change: 11018 $
 * 
 */

#ifndef TESTCOMMONFEATURECOMMANDS___H
#define TESTCOMMONFEATURECOMMANDS___H	1

#ifdef _WIN32
#pragma once
#endif

//
// This class contains common tests against basic feature commands (select, select aggregates, insert, update, delete)
class TestCommonFeatureCommands
{
public:
    // Tests selecting from a class using a computed identifier and non-computed identifier of the same name.
    //   conn: a connection that is already open
    //   className: the name of an existing class
    //   numericPropName: the name of a numeric property on the existing class
    static void duplicateComputedIdTest (FdoIConnection* conn, FdoString* className, FdoString* numericPropName);

    // Tests selecting both a computed id and a non-computed id of different names but on the same underlying property.
    // This exercises defect 815921 and 817232.
    //   conn: a connection that is already open
    //   className: the name of an existing class
    //   numericPropName: the name of a numeric property on the existing class
    static void secondComputedIdTest (FdoIConnection* conn, FdoString* className, FdoString* numericPropName);
};

#endif // TESTCOMMONFEATURECOMMANDS___H


