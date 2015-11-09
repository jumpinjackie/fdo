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

#include "stdafx.h"
#include "c_KgInfSpatialContextReader.h"


c_KgInfSpatialContextReader::c_KgInfSpatialContextReader (c_KgInfSpatialContextCollection*  spatialContextColl) :
	m_SpatialContextColl (spatialContextColl),
	m_CurrIndex (0)
{
	FDO_SAFE_ADDREF(m_SpatialContextColl.p);
}

c_KgInfSpatialContextReader::~c_KgInfSpatialContextReader (void)
{
}

void c_KgInfSpatialContextReader::Dispose ()
{
    delete this;
}


FdoString* c_KgInfSpatialContextReader::GetName ()
{
    
    return (m_Context->GetName ());
}

FdoString* c_KgInfSpatialContextReader::GetDescription ()
{
    return (m_Context->GetDescription ());
}

FdoString* c_KgInfSpatialContextReader::GetCoordinateSystem ()
{
    return (m_Context->GetCoordinateSystem ());
}


FdoString* c_KgInfSpatialContextReader::GetCoordinateSystemWkt ()
{
    return (m_Context->GetCoordinateSystemWkt ());
}

FdoSpatialContextExtentType c_KgInfSpatialContextReader::GetExtentType ()
{  
    return (m_Context->GetExtentType ());
}

FdoByteArray* c_KgInfSpatialContextReader::GetExtent ()
{
    return (m_Context->GetExtent ());
}

const double c_KgInfSpatialContextReader::GetXYTolerance ()
{
    return (m_Context->GetXYTolerance ());
}

const double c_KgInfSpatialContextReader::GetZTolerance ()
{
    return (m_Context->GetZTolerance ());
}

const bool c_KgInfSpatialContextReader::IsActive ()
{
    return (true); 
}

bool c_KgInfSpatialContextReader::ReadNext ()
{
	bool	rc = false;
	if ( m_CurrIndex < m_SpatialContextColl->GetCount())
	{
		m_Context = m_SpatialContextColl->GetItem( m_CurrIndex++ );
		rc = true;
	}
  return ( rc );
}//end of c_KgInfSpatialContextReader::ReadNext


