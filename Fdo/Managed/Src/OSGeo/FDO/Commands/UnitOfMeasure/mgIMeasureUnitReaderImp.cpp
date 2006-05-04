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
#include <Fdo\Commands\UnitOfMeasure\IMeasureUnitReader.h>

#include "FDO\Commands\UnitOfMeasure\mgIMeasureUnitReaderImp.h"

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReaderImp::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}

FdoIMeasureUnitReader* NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReaderImp::GetImpObj()
{
    return static_cast<FdoIMeasureUnitReader*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReaderImp::GetAbbreviation()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetAbbreviation())

		return result;
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReaderImp::GetName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

		return result;
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReaderImp::GetDescription()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())

		return result;
}

NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::BaseUnit NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReaderImp::GetBaseUnit()
{
	FdoBaseUnit result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetBaseUnit())

		return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::BaseUnit>(result);;
}

System::Double NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReaderImp::GetScaleFactor()
{
	FdoDouble result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetScaleFactor())

		return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReaderImp::ReadNext()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->ReadNext())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReaderImp::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}
