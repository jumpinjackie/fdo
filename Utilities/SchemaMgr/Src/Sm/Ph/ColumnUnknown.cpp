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

#include "stdafx.h"
#include <Sm/Ph/Mgr.h>

FdoSmPhColumnUnknown::FdoSmPhColumnUnknown(
    int length,
    int scale
) :
	mLength(length),
    mScale(scale)
{
}

FdoStringP FdoSmPhColumnUnknown::GetValueSql( FdoStringP val )
{
    // Format as if this is a string column.
    FdoStringP valSyntax = GetNullValueSql();

    if ( val != L"" ) 
        valSyntax = FdoStringP::Format( L"'%ls'", (FdoString*) val );

    return GetManager()->FormatSQLVal( val, FdoSmPhColType_String );
}

FdoStringP FdoSmPhColumnUnknown::GetTypeSql()
{
    if ( GetScale() == 0 )
        // Base function adds length if set, but doesn't handle scale
        return FdoSmPhColumn::GetTypeSql();
    else
        // Handle ourselves when scale specified.
        return FdoStringP::Format( L"%ls (%d, %d)", (FdoString*) GetTypeName(), GetLength(), GetScale() );
}
