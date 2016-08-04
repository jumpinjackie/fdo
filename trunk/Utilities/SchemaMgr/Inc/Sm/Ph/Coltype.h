#ifndef FDOSMPHCOLTYPE_H
#define FDOSMPHCOLTYPE_H		1
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

// Generic Schema Manager physical column types.
//
enum FdoSmPhColType
{
    FdoSmPhColType_BLOB,
    FdoSmPhColType_Date,
    FdoSmPhColType_Decimal,
    FdoSmPhColType_Single,
    FdoSmPhColType_Double,
    FdoSmPhColType_Geom,
    FdoSmPhColType_Bool,
    FdoSmPhColType_Byte,
    FdoSmPhColType_Int16,
    FdoSmPhColType_Int32,
    FdoSmPhColType_Int64,
    FdoSmPhColType_String,
    FdoSmPhColType_Unknown
};

#endif


