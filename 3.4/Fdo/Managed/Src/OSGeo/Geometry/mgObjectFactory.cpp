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
inline NAMESPACE_OSGEO_RUNTIME::Disposable* CHECK(FdoIDisposable* p, FdoBoolean autoDispose)
{
    return (dynamic_cast<geoclas *>(p) ?  new clas(p, autoDispose) : NULL);
}

NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateCurveSegmentCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveSegmentAbstract(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

	FdoICurveSegmentAbstract* p = static_cast<FdoICurveSegmentAbstract*>(ptr.ToPointer());

	NAMESPACE_OSGEO_RUNTIME::Disposable* wrap;

	switch(p->GetDerivedType())
	{
		case FdoGeometryComponentType_LinearRing:
		{
			// Note:
			// We don't know which class will return this type
			return NULL;
		}
		case FdoGeometryComponentType_CircularArcSegment:
		{	
			return new NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegmentImp(p, autoDispose);
		}
		case FdoGeometryComponentType_LineStringSegment:
		{
			return new NAMESPACE_OSGEO_GEOMETRY::ILineStringSegmentImp(p, autoDispose);
		}
		case FdoGeometryComponentType_Ring:
		{
			// Note:
			// We don't know which class will return this type
			return NULL;
		}
		default:
		{
			// Note:
			// We don't know which type these classes belong to.
			if(wrap = CHECK<FdoILineStringSegment, NAMESPACE_OSGEO_GEOMETRY::ILineStringSegmentImp>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp*>(wrap);
			if(wrap = CHECK<FdoIArcSegmentAbstract, NAMESPACE_OSGEO_GEOMETRY::IArcSegmentAbstractImp>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp*>(wrap);

			return new NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp(ptr, autoDispose);
		}
	}
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelope* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IPolygon* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPolygon(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::IPolygonImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IPoint* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPoint(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::IPointImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IRing* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIRing(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::IRingImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ILineString* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineString(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::ILineStringImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
	    return NULL;

	FdoIGeometry* p = static_cast<FdoIGeometry*>(ptr.ToPointer());

	NAMESPACE_OSGEO_RUNTIME::Disposable* wrap;

	switch(p->GetDerivedType())
	{
		case FdoGeometryType_None:
		{
			// Note:
			// We don't know which type these classes belong to.
			if(wrap = CHECK<FdoISurfaceAbstract, NAMESPACE_OSGEO_GEOMETRY::ISurfaceAbstractImp>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp*>(wrap);
			if(wrap = CHECK<FdoIGeometricAggregateAbstract, NAMESPACE_OSGEO_GEOMETRY::IGeometricAggregateAbstractImp>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp*>(wrap);
			if(wrap = CHECK<FdoICurveAbstract, NAMESPACE_OSGEO_GEOMETRY::ICurveAbstractImp>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp*>(wrap);

			return new NAMESPACE_OSGEO_GEOMETRY::IGeometryImp(ptr, autoDispose);
		}
		case FdoGeometryType_Point:
		{	
			return new NAMESPACE_OSGEO_GEOMETRY::IPointImp(p, autoDispose);
		}
		case FdoGeometryType_LineString:
		{
			return new NAMESPACE_OSGEO_GEOMETRY::ILineStringImp(p, autoDispose);
		}
		case FdoGeometryType_Polygon:
		{
			return new NAMESPACE_OSGEO_GEOMETRY::IPolygonImp(p, autoDispose);
		}
		case FdoGeometryType_MultiPoint:
		{
			return new NAMESPACE_OSGEO_GEOMETRY::IMultiPointImp(p, autoDispose);
		}
		case FdoGeometryType_MultiLineString:
		{
			return new NAMESPACE_OSGEO_GEOMETRY::IMultiLineStringImp(p, autoDispose);
		}
		case FdoGeometryType_MultiPolygon:
		{
			return new NAMESPACE_OSGEO_GEOMETRY::IMultiPolygonImp(p, autoDispose);
		}
		case FdoGeometryType_MultiGeometry:
		{
			return new NAMESPACE_OSGEO_GEOMETRY::IMultiGeometryImp(p, autoDispose);
		}
		case FdoGeometryType_CurveString:
		{
			return new NAMESPACE_OSGEO_GEOMETRY::ICurveStringImp(p, autoDispose);
		}
		case FdoGeometryType_CurvePolygon:
		{
			return new NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp(p, autoDispose);
		}
		case FdoGeometryType_MultiCurveString:
		{
			return new NAMESPACE_OSGEO_GEOMETRY::IMultiCurveStringImp(p, autoDispose);
		}
		case FdoGeometryType_MultiCurvePolygon:
		{
			return new NAMESPACE_OSGEO_GEOMETRY::IMultiCurvePolygonImp(p, autoDispose);
		}
		default:
			return NULL;
	}
}

NAMESPACE_OSGEO_GEOMETRY::ICurveString* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveString(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::ICurveStringImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurvePolygon(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ILineStringSegment* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineStringSegment(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::ILineStringSegmentImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiCurvePolygon* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiCurvePolygon(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::IMultiCurvePolygonImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiGeometry* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiGeometry(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::IMultiGeometryImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiPoint* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiPoint(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::IMultiPointImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiPolygon* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiPolygon(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::IMultiPolygonImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiLineString* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiLineString(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::IMultiLineStringImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiCurveString* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiCurveString(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::IMultiCurveStringImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegment* NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICircularArcSegment(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return NULL;

    return new NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegmentImp(ptr, autoDispose);
}
