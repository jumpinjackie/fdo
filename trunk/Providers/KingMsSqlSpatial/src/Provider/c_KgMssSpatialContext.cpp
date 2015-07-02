/*
* Copyright (C) 2006  Haris Kurtagic
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
*/


#include <stdafx.h>



c_KgMssSpatialContext::c_KgMssSpatialContext () :
    m_Name (D_SPATIALCONTEXT_DEFAULT_NAME),
    m_Description (L""),
    m_CoordSysName (D_SPATIALCONTEXT_CSYS_DEFAULT_NAME),
    m_Wkt (D_SPATIALCONTEXT_CSYS_DEFAULT_WKT),
    m_ExtentType (FdoSpatialContextExtentType_Static),
    m_XYTolerance (D_SPATIALCONTEXT_DEFAULT_XY_TOLERANCE),
    m_ZTolerance (D_SPATIALCONTEXT_DEFAULT_Z_TOLERANCE)
{
  m_CoordSysName = "LOCAL_CS[\"*XY-MT*\",LOCAL_DATUM[\"*X-Y*\",10000],UNIT[\"Meter\", 1],AXIS[\"X\",EAST],AXIS[\"Y\",NORTH]]";
  m_Wkt = "LOCAL_CS[\"*XY-MT*\",LOCAL_DATUM[\"*X-Y*\",10000],UNIT[\"Meter\", 1],AXIS[\"X\",EAST],AXIS[\"Y\",NORTH]]";
  
    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
  
    m_ExtentEnvelope = FdoEnvelopeImpl::Create();
  
    m_IsExtentUpdated = true;
    
  
}

c_KgMssSpatialContext::~c_KgMssSpatialContext ()
{
}

FdoString* c_KgMssSpatialContext::GetName ()
{
    return (m_Name);
}

void c_KgMssSpatialContext::SetName (FdoString* name)
{
    m_Name = name;
}

FdoString* c_KgMssSpatialContext::GetDescription ()
{
    return (m_Description);
}

void c_KgMssSpatialContext::SetDescription (FdoString* desc)
{
    m_Description = desc;
}

FdoString* c_KgMssSpatialContext::GetCoordinateSystem ()
{
    return (m_CoordSysName);
}

void c_KgMssSpatialContext::SetCoordSysName (FdoString * csName)
{
    m_CoordSysName = csName;
}

FdoString* c_KgMssSpatialContext::GetCoordinateSystemWkt ()
{
    return (m_Wkt);
}

void c_KgMssSpatialContext::SetCoordinateSystemWkt (FdoString* wkt)
{
    m_Wkt = wkt;
}

FdoSpatialContextExtentType c_KgMssSpatialContext::GetExtentType ()
{
    return (m_ExtentType);
}

void c_KgMssSpatialContext::SetExtentType (FdoSpatialContextExtentType type)
{
    m_ExtentType = type;
}

FdoByteArray* c_KgMssSpatialContext::GetExtent ()
{
  FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
  if( m_ExtentEnvelope->GetIsEmpty() )
  {
      FdoPtr<FdoIEnvelope> envelope = factory->CreateEnvelopeXY (
        D_SPATIALCONTEXT_DEFAULT_MINX
        ,D_SPATIALCONTEXT_DEFAULT_MINY
        ,D_SPATIALCONTEXT_DEFAULT_MAXX
        ,D_SPATIALCONTEXT_DEFAULT_MAXY
        );
        
    FdoPtr<FdoIGeometry> geometry = factory->CreateGeometry (envelope);
    return factory->GetFgf (geometry);        
  }
  else
  {
  
    
    FdoPtr<FdoIGeometry> geometry = factory->CreateGeometry (m_ExtentEnvelope);
    return factory->GetFgf (geometry);
  }
  
}

FdoIEnvelope* c_KgMssSpatialContext::GetExtentEnvelope ()
{
    return (FDO_SAFE_ADDREF(m_ExtentEnvelope.p));
}



void c_KgMssSpatialContext::ExpandExtent (FdoIEnvelope * Envelope)
{
    m_ExtentEnvelope->Expand(Envelope);
}


double c_KgMssSpatialContext::GetXYTolerance ()
{
    return (m_XYTolerance);
}

void c_KgMssSpatialContext::SetXYTolerance (double tol)
{
    m_XYTolerance = tol;
}

double c_KgMssSpatialContext::GetZTolerance ()
{
    return (m_ZTolerance);
}

void c_KgMssSpatialContext::SetZTolerance (double tol)
{
    m_ZTolerance = tol;
}

bool c_KgMssSpatialContext::GetIsExtentUpdated()
{
	return m_IsExtentUpdated;
}
void c_KgMssSpatialContext::SetIsExtentUpdated( bool value )
{
	m_IsExtentUpdated = value;
}

const c_KgMssSridDesc& c_KgMssSpatialContext::GetOraSridDesc()
{
  return m_OraSridDesc;
}//end of c_KgMssSpatialContext::GetOraSridDesc

void c_KgMssSpatialContext::SetOraSridDesc(const c_KgMssSridDesc& SridDesc)
{
  m_OraSridDesc = SridDesc;
}//end of c_KgMssSpatialContext::SetOraSridDesc