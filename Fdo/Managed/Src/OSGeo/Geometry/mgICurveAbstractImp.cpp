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
#include "mgICurveAbstractImp.h"
#include <FdoGeometry.h>
#include "mgIDirectPosition.h"
#include "mgObjectFactory.h"

FdoICurveAbstract* NAMESPACE_OSGEO_GEOMETRY::ICurveAbstractImp::GetImpObj()
{
	return static_cast<FdoICurveAbstract*>(UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::ICurveAbstractImp::ICurveAbstractImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::IGeometryImp(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ NAMESPACE_OSGEO_GEOMETRY::ICurveAbstractImp::StartPosition::get()
{
	FdoIDirectPosition* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetStartPosition())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ NAMESPACE_OSGEO_GEOMETRY::ICurveAbstractImp::EndPosition::get()
{
	FdoIDirectPosition* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetEndPosition())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(IntPtr(ret), true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::ICurveAbstractImp::IsClosed::get()
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->GetIsClosed())
	return ret;
}

