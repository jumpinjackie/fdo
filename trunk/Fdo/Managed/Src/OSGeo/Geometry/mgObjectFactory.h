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

#pragma once

BEGIN_NAMESPACE_OSGEO_GEOMETRY
public __gc class CurveSegmentCollection;
public __gc __interface ICurveSegmentAbstract;
public __gc __interface IEnvelope;
public __gc __interface ILinearRing;
public __gc __interface IPolygon;
public __gc __interface IDirectPosition;
public __gc __interface IPoint;
public __gc __interface IRing;
public __gc __interface ILineString;
public __gc __interface IGeometry;
public __gc __interface ICurveString;
public __gc __interface ICurvePolygon;
public __gc __interface ILineStringSegment;
public __gc __interface IMultiCurvePolygon;
public __gc __interface IMultiGeometry;
public __gc __interface IMultiPoint;
public __gc __interface IMultiPolygon;
public __gc __interface IMultiLineString;
public __gc __interface IMultiCurveString;
public __gc __interface ICircularArcSegment;
public __gc __interface ICurveSegmentAbstract;
public __gc __interface ICurveSegmentAbstract;

public __gc __sealed class ObjectFactory
{
public:
    static NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection* CreateCurveSegmentCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract* CreateICurveSegmentAbstract(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IEnvelope* CreateIEnvelope(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::ILinearRing* CreateILinearRing(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IPolygon* CreateIPolygon(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* CreateIDirectPosition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IPoint* CreateIPoint(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IRing* CreateIRing(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::ILineString* CreateILineString(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IGeometry* CreateIGeometry(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::ICurveString* CreateICurveString(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon* CreateICurvePolygon(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::ILineStringSegment* CreateILineStringSegment(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IMultiCurvePolygon* CreateIMultiCurvePolygon(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IMultiGeometry* CreateIMultiGeometry(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IMultiPoint* CreateIMultiPoint(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IMultiPolygon* CreateIMultiPolygon(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IMultiLineString* CreateIMultiLineString(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::IMultiCurveString* CreateIMultiCurveString(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegment* CreateICircularArcSegment(System::IntPtr ptr, System::Boolean autoDispose);

private:
	ObjectFactory(){};
};

END_NAMESPACE_OSGEO_GEOMETRY


