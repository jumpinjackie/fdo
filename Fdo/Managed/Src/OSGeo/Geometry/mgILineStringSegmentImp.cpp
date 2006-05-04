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
#include "mgILineStringSegmentImp.h"
#include <Geometry.h>
#include "mgIDirectPositionImp.h"
#include "mgObjectFactory.h"

FdoILineStringSegment * NAMESPACE_OSGEO_GEOMETRY::ILineStringSegmentImp::GetImpObj()
{
	return static_cast<FdoILineStringSegment *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::ILineStringSegmentImp::ILineStringSegmentImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp(unmanaged, autoDelete)
{
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::ILineStringSegmentImp::get_Count()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *NAMESPACE_OSGEO_GEOMETRY::ILineStringSegmentImp::get_Item(System::Int32 index)
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection *NAMESPACE_OSGEO_GEOMETRY::ILineStringSegmentImp::get_Positions()
{
	FdoDirectPositionCollection *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetPositions())
	return new DirectPositionCollection(ret, true);
}
