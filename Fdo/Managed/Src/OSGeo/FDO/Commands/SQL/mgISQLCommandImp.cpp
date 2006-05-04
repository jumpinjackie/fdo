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

// The following header files sequence is very important
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Expression\LOBValue.h>
#include <Fdo\Commands\SQL\ISQLCommand.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\SQL\mgISQLCommandImp.h"

#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\SQL\mgISQLDataReaderImp.h"

FdoISQLCommand* NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLCommandImp::GetImpObj()
{
    return static_cast<FdoISQLCommand*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLCommandImp::get_SQLStatement()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSQLStatement())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLCommandImp::set_SQLStatement(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSQLStatement(StringToUni(value)))
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLCommandImp::ExecuteNonQuery()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->ExecuteNonQuery())

	return result;
}

NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReader* NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLCommandImp::ExecuteReader()
{
	FdoISQLDataReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->ExecuteReader())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateISQLDataReader(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLCommandImp::Dispose()
{
	NAMESPACE_OSGEO_RUNTIME::Disposable::Dispose();
}
