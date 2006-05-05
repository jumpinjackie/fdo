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

#pragma once

#include "stdafx.h"
#include <Rdbms.h>
#include <SpatialIndexType.h>
#include <SpatialIndexDimensionType.h>
#include <ISpatialIndexReader.h>

#include "mgSpatialIndexType.h"
#include "mgSpatialIndexDimensionType.h"
#include "mgISpatialIndexReaderImp.h"

FdoISpatialIndexReader* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ISpatialIndexReaderImp::GetImpObj()
{
	return static_cast<FdoISpatialIndexReader*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ISpatialIndexReaderImp::ISpatialIndexReaderImp(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}


System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ISpatialIndexReaderImp::get_Name()
{
	const wchar_t* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetName())

	return unobj;
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ISpatialIndexReaderImp::get_SpatialContextName()
{
	const wchar_t* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpatialContextName())

		return unobj;
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ISpatialIndexReaderImp::get_RdbmsSpatialIndexType()
{
	SpatialIndexType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpatialIndexType())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType>(unobj);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ISpatialIndexReaderImp::get_NumDimensions()
{
	SpatialIndexDimensionType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetNumDimensions())

		return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType>(unobj);
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ISpatialIndexReaderImp::ReadNext()
{
	bool exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->ReadNext())

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ISpatialIndexReaderImp::Dispose()
{
	EXCEPTION_HANDLER(GetImpObj()->Dispose())
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ISpatialIndexReaderImp::Dispose(System::Boolean disposing)
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
