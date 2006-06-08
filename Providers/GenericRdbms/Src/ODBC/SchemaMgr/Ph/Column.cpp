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
#include "Column.h"
#include "DbObject.h"
#include "../../../SchemaMgr/Ph/Rd/QueryReader.h"


FdoSmPhOdbcColumn::FdoSmPhOdbcColumn(FdoSmPhRdColumnReader* reader)
{
}


FdoSmPhOdbcColumn::~FdoSmPhOdbcColumn(void)
{
}


FdoStringP FdoSmPhOdbcColumn::GetAddSql()
{
    FdoStringP addSql;

    // Get basic name/type/nullability SQL, or computed expression SQL:
    if (m_computedExpression.GetLength() == 0)
        addSql = FdoSmPhColumn::GetAddSql();
    else
        addSql = FdoStringP::Format(L"%ls as %ls", (FdoString*) GetDbName(), (FdoString*)m_computedExpression);
   
    return addSql;
}


FdoStringP FdoSmPhOdbcColumn::GetAutoincrementSql()
{
    if ( FdoSmPhColumn::GetAutoincrement() )
        return L" IDENTITY (1,1) ";
    else
        return L"";
}


