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
#include "mgEnvelopeImpl.h"
#include <Geometry.h>
#include <Geometry/EnvelopeImpl.h>
#include "mgIEnvelopeImp.h"
#include "mgIDirectPositionImp.h"

FdoEnvelopeImpl* NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::GetImpObj()
{
	return static_cast<FdoEnvelopeImpl *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::EnvelopeImpl() 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoEnvelopeImpl::Create(), true))
}

NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::EnvelopeImpl(
	System::Double minX, System::Double minY,
	System::Double maxX, System::Double maxY) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoEnvelopeImpl::Create(minX, minY, maxX, maxY), true))
}

NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::EnvelopeImpl(
	System::Double minX, System::Double minY, System::Double minZ,
	System::Double maxX, System::Double maxY, System::Double maxZ) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoEnvelopeImpl::Create(minX, minY, minZ, maxX, maxY, maxZ), true))
}

NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::EnvelopeImpl(EnvelopeImpl* envelopeImpl) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoEnvelopeImpl::Create(*(envelopeImpl->GetImpObj())), true))
}

NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::EnvelopeImpl(
	NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *lowerLeft, 
	NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *upperRight) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoEnvelopeImpl::Create(static_cast<IDirectPositionImp *>(lowerLeft)->GetImpObj(), static_cast<IDirectPositionImp *>(upperRight)->GetImpObj()), true))
}

NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::EnvelopeImpl(System::Int32 dimensionType, System::Double ordinates __gc[]) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	FdoDouble __pin *tpordinates = &ordinates[0];
	EXCEPTION_HANDLER(Attach(FdoEnvelopeImpl::Create(dimensionType, tpordinates), true))
}

NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::EnvelopeImpl(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::op_Equality(EnvelopeImpl* left, EnvelopeImpl* right)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = (left->GetImpObj())->operator==(*(right->GetImpObj())))
	return ret;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::op_Inequality(EnvelopeImpl* left, EnvelopeImpl* right)
{
	return !op_Equality(left, right);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::Equals(System::Object* obj)
{
	NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl* envlope = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl*>(obj);
	if (NULL == envlope)
	{
		return false;
	}
	else
	{
		return op_Equality(this, envlope);
	}
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::GetHashCode()
{
	return (int)MinX + (int)MinY + (int)MinZ;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::get_MinX()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMinX())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::set_MinX(System::Double minX)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMinX(minX));
}

System::Double NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::get_MinY()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMinY())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::set_MinY(System::Double minY)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMinY(minY));
}

System::Double NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::get_MinZ()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMinZ())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::set_MinZ(System::Double minZ)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMinZ(minZ));
}

System::Double NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::get_MaxX()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMaxX())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::set_MaxX(System::Double maxX)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMaxX(maxX));
}

System::Double NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::get_MaxY()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMaxY())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::set_MaxY(System::Double maxY)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMaxY(maxY));
}

System::Double NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::get_MaxZ()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMaxZ())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::set_MaxZ(System::Double maxZ)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMaxZ(maxZ));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::get_IsEmpty()
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetIsEmpty())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::Expand(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* position)
{
	FdoIDirectPosition *tpposition = static_cast<IDirectPositionImp *>(position)->GetImpObj();
	EXCEPTION_HANDLER(GetImpObj()->Expand(tpposition));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::Expand(NAMESPACE_OSGEO_GEOMETRY::IEnvelope* envelope)
{
	FdoIEnvelope *tpenvelope = static_cast<IEnvelopeImp *>(envelope)->GetImpObj();
	EXCEPTION_HANDLER(GetImpObj()->Expand(tpenvelope));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::EnvelopeImpl::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
        // Add EnvelopeImpl specific code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}
