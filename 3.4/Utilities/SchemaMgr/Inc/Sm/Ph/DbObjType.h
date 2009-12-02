#ifndef FDOSMPHDBOBJTYPE_H
#define FDOSMPHDBOBJTYPE_H		1
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
 * 
 */

// This enumeration lists the different types of database objects supported
// by the Generic Schema Manager.

enum FdoSmPhDbObjType
{
    FdoSmPhDbObjType_Table,         // a concrete table.
    FdoSmPhDbObjType_View,          // view on another database object (or objects).
    FdoSmPhDbObjType_Index,         // table index
    FdoSmPhDbObjType_Temp,          // temporary object (for queries).
    FdoSmPhDbObjType_Provider,      // Provider-specific type
    FdoSmPhDbObjType_Unknown        // Don't know what this database object is.
};

#endif


