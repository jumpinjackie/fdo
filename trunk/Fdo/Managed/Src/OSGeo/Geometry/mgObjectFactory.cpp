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
#include "mgObjectFactory.h"
#include <FdoGeometry.h>
#include <Geometry/EnvelopeImpl.h>

#include "mgGeometryFactoryAbstract.h"

#include "mgIArcSegmentAbstractImp.h"
#include "mgICircularArcSegmentImp.h"
#include "mgICurveAbstractImp.h"
#include "mgICurvePolygonImp.h"
#include "mgICurveSegmentAbstractImp.h"
#include "mgICurveStringImp.h"
#include "mgIDirectPositionImp.h"
#include "mgIEnvelopeImp.h"
#include "mgIGeometricAggregateAbstractImp.h"
#include "mgIGeometryImp.h"
#include "mgILineStringImp.h"
#include "mgILineStringSegmentImp.h"
#include "mgILinearRingImp.h"
#include "mgIMultiCurvePolygonImp.h"
#include "mgIMultiCurveStringImp.h"
#include "mgIMultiGeometryImp.h"
#include "mgIMultiLineStringImp.h"
#include "mgIMultiPointImp.h"
#include "mgIMultiPolygonImp.h"
#include "mgIPointImp.h"
#include "mgIPolygonImp.h"
#include "mgIRingAbstractImp.h"
#include "mgIRingImp.h"
#include "mgISurfaceAbstractImp.h"

#include "mgDirectPositionImpl.h"
#include "mgEnvelopeImpl.h"



template <class geoclas, class clas> 
inline NAMESPACE_OSGEO_RUNTIME::Disposable^ CHECK(FdoIDisposable* p, FdoBoolean autoDispose)
{
    return (dynamic_cast<geoclas *>(p) ?  gcnew clas(IntPtr(p), autoDispose) : nullptr);
}

NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateCurveSegmentCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveSegmentAbstract(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoICurveSegmentAbstract* p = static_cast<FdoICurveSegmentAbstract*>(ptr.ToPointer());

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	switch(p->GetDerivedType())
	{
		case FdoGeometryComponentType_LinearRing:
		{
			// Note:
			// We don't know which class will return this type
			return nullptr;
		}
		case FdoGeometryComponentType_CircularArcSegment:
		{	
			return gcnew NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegmentImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryComponentType_LineStringSegment:
		{
			return gcnew NAMESPACE_OSGEO_GEOMETRY::ILineStringSegmentImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryComponentType_Ring:
		{
			// Note:
			// We don't know which class will return this type
			return nullptr;
		}
		default:
		{
			// Note:
			// We don't know which type these classes belong to.
			if(wrap = CHECK<FdoILineStringSegment, NAMESPACE_OSGEO_GEOMETRY::ILineStringSegmentImp>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp^>(wrap);
			if(wrap = CHECK<FdoIArcSegmentAbstract, NAMESPACE_OSGEO_GEOMETRY::IArcSegmentAbstractImp>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp^>(wrap);

			return gcnew NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp(ptr, autoDispose);
		}
	}
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IPolygon^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPolygon(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::IPolygonImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IPoint^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPoint(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::IPointImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IRing^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIRing(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::IRingImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ILineString^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineString(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::ILineStringImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
	    return nullptr;

	FdoIGeometry* p = static_cast<FdoIGeometry*>(ptr.ToPointer());

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	switch(p->GetDerivedType())
	{
		case FdoGeometryType_None:
		{
			// Note:
			// We don't know which type these classes belong to.
			if(wrap = CHECK<FdoISurfaceAbstract, NAMESPACE_OSGEO_GEOMETRY::ISurfaceAbstractImp>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(wrap);
			if(wrap = CHECK<FdoIGeometricAggregateAbstract, NAMESPACE_OSGEO_GEOMETRY::IGeometricAggregateAbstractImp>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(wrap);
			if(wrap = CHECK<FdoICurveAbstract, NAMESPACE_OSGEO_GEOMETRY::ICurveAbstractImp>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(wrap);

			return gcnew NAMESPACE_OSGEO_GEOMETRY::IGeometryImp(ptr, autoDispose);
		}
		case FdoGeometryType_Point:
		{	
			return gcnew NAMESPACE_OSGEO_GEOMETRY::IPointImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryType_LineString:
		{
			return gcnew NAMESPACE_OSGEO_GEOMETRY::ILineStringImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryType_Polygon:
		{
			return gcnew NAMESPACE_OSGEO_GEOMETRY::IPolygonImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryType_MultiPoint:
		{
			return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiPointImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryType_MultiLineString:
		{
			return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiLineStringImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryType_MultiPolygon:
		{
			return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiPolygonImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryType_MultiGeometry:
		{
			return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiGeometryImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryType_CurveString:
		{
			return gcnew NAMESPACE_OSGEO_GEOMETRY::ICurveStringImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryType_CurvePolygon:
		{
			return gcnew NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryType_MultiCurveString:
		{
			return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiCurveStringImp(IntPtr(p), autoDispose);
		}
		case FdoGeometryType_MultiCurvePolygon:
		{
			return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiCurvePolygonImp(IntPtr(p), autoDispose);
		}
		default:
			return nullptr;
	}
}

NAMESPACE_OSGEO_GEOMETRY::ICurveString^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveString(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::ICurveStringImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurvePolygon(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ILineStringSegment^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineStringSegment(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::ILineStringSegmentImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiCurvePolygon^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiCurvePolygon(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiCurvePolygonImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiGeometry^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiGeometry(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiGeometryImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiPoint^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiPoint(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiPointImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiPolygon^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiPolygon(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiPolygonImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiLineString^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiLineString(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiLineStringImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiCurveString^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiCurveString(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::IMultiCurveStringImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegment^ NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICircularArcSegment(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegmentImp(ptr, autoDispose);
}
