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

#include "stdafx.h"
#include "c_KgOraSpatialContextReader.h"


c_KgOraSpatialContextReader::c_KgOraSpatialContextReader (c_KgOraSpatialContextCollection*  spatialContextColl) :
	m_SpatialContextColl (spatialContextColl),
	m_CurrIndex (0)
{
	FDO_SAFE_ADDREF(m_SpatialContextColl.p);
}

c_KgOraSpatialContextReader::~c_KgOraSpatialContextReader (void)
{
}

void c_KgOraSpatialContextReader::Dispose ()
{
    delete this;
}


FdoString* c_KgOraSpatialContextReader::GetName ()
{
    
    return (m_Context->GetName ());
}

FdoString* c_KgOraSpatialContextReader::GetDescription ()
{
    return (m_Context->GetDescription ());
}

FdoString* c_KgOraSpatialContextReader::GetCoordinateSystem ()
{
    return (m_Context->GetCoordinateSystem ());
}


FdoString* c_KgOraSpatialContextReader::GetCoordinateSystemWkt ()
{
    return (m_Context->GetCoordinateSystemWkt ());
}

FdoSpatialContextExtentType c_KgOraSpatialContextReader::GetExtentType ()
{  
    return (m_Context->GetExtentType ());
}

FdoByteArray* c_KgOraSpatialContextReader::GetExtent ()
{
    return (m_Context->GetExtent ());
}

const double c_KgOraSpatialContextReader::GetXYTolerance ()
{
    return (m_Context->GetXYTolerance ());
}

const double c_KgOraSpatialContextReader::GetZTolerance ()
{
    return (m_Context->GetZTolerance ());
}

const bool c_KgOraSpatialContextReader::IsActive ()
{
    return (true); 
}

bool c_KgOraSpatialContextReader::ReadNext ()
{
	bool	rc = false;
	if ( m_CurrIndex < m_SpatialContextColl->GetCount())
	{
		m_Context = m_SpatialContextColl->GetItem( m_CurrIndex++ );
		rc = true;
	}
  return ( rc );
}//end of c_KgOraSpatialContextReader::ReadNext


