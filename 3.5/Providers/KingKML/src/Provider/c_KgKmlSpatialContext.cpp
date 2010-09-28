/*
* Copyright (C) 2006  SL-King d.o.o
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



c_KgKmlSpatialContext::c_KgKmlSpatialContext () :
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

c_KgKmlSpatialContext::~c_KgKmlSpatialContext ()
{
}

FdoString* c_KgKmlSpatialContext::GetName ()
{
    return (m_Name);
}

void c_KgKmlSpatialContext::SetName (FdoString* name)
{
    m_Name = name;
}

FdoString* c_KgKmlSpatialContext::GetDescription ()
{
    return (m_Description);
}

void c_KgKmlSpatialContext::SetDescription (FdoString* desc)
{
    m_Description = desc;
}

FdoString* c_KgKmlSpatialContext::GetCoordinateSystem ()
{
    return (m_CoordSysName);
}

void c_KgKmlSpatialContext::SetCoordSysName (FdoString * csName)
{
    m_CoordSysName = csName;
}

FdoString* c_KgKmlSpatialContext::GetCoordinateSystemWkt ()
{
    return (m_Wkt);
}

void c_KgKmlSpatialContext::SetCoordinateSystemWkt (FdoString* wkt)
{
    m_Wkt = wkt;
}

FdoSpatialContextExtentType c_KgKmlSpatialContext::GetExtentType ()
{
    return (m_ExtentType);
}

void c_KgKmlSpatialContext::SetExtentType (FdoSpatialContextExtentType type)
{
    m_ExtentType = type;
}

FdoByteArray* c_KgKmlSpatialContext::GetExtent ()
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

FdoIEnvelope* c_KgKmlSpatialContext::GetExtentEnvelope ()
{
    return (FDO_SAFE_ADDREF(m_ExtentEnvelope.p));
}



void c_KgKmlSpatialContext::ExpandExtent (FdoIEnvelope * Envelope)
{
    m_ExtentEnvelope->Expand(Envelope);
}


double c_KgKmlSpatialContext::GetXYTolerance ()
{
    return (m_XYTolerance);
}

void c_KgKmlSpatialContext::SetXYTolerance (double tol)
{
    m_XYTolerance = tol;
}

double c_KgKmlSpatialContext::GetZTolerance ()
{
    return (m_ZTolerance);
}

void c_KgKmlSpatialContext::SetZTolerance (double tol)
{
    m_ZTolerance = tol;
}

bool c_KgKmlSpatialContext::GetIsExtentUpdated()
{
	return m_IsExtentUpdated;
}
void c_KgKmlSpatialContext::SetIsExtentUpdated( bool value )
{
	m_IsExtentUpdated = value;
}

const c_KgKmlSridDesc& c_KgKmlSpatialContext::GetOraSridDesc()
{
  return m_OraSridDesc;
}//end of c_KgKmlSpatialContext::GetOraSridDesc

void c_KgKmlSpatialContext::SetOraSridDesc(const c_KgKmlSridDesc& SridDesc)
{
  m_OraSridDesc = SridDesc;
}//end of c_KgKmlSpatialContext::SetOraSridDesc