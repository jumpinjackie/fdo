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
#include <Rdbms.h>
#include <ISpatialIndexReader.h>
#include <IGetSpatialIndexes.h>
#include <SpatialIndexType.h>
#include <SpatialIndexDimensionType.h>
#include <ISpatialIndexReader.h>

#include "mgSpatialIndexType.h"
#include "mgSpatialIndexDimensionType.h"

#include "mgObjectFactory.h"
#include "mgISpatialIndexReaderImp.h"
#include "mgIGetSpatialIndexesImp.h"

FdoIGetSpatialIndexes* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IGetSpatialIndexesImp::GetImpObj()
{
	return static_cast<FdoIGetSpatialIndexes*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IGetSpatialIndexesImp::IGetSpatialIndexesImp(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
{

}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IGetSpatialIndexesImp::get_ActiveOnly()
{
	bool ret;

	EXCEPTION_HANDLER(ret = !!GetImpObj()->GetActiveOnly())

	return ret;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IGetSpatialIndexesImp::set_ActiveOnly(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetActiveOnly(value))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ISpatialIndexReader* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IGetSpatialIndexesImp::Execute()
{
	FdoISpatialIndexReader * unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->Execute())

	return ObjectFactory::CreateISpatialIndexReader(unobj, true);
}

