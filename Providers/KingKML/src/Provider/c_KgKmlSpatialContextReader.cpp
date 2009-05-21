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
#include "c_KgKmlSpatialContextReader.h"


c_KgKmlSpatialContextReader::c_KgKmlSpatialContextReader (c_KgKmlSpatialContextCollection*  spatialContextColl) :
	m_SpatialContextColl (spatialContextColl),
	m_CurrIndex (0)
{
	FDO_SAFE_ADDREF(m_SpatialContextColl.p);
}

c_KgKmlSpatialContextReader::~c_KgKmlSpatialContextReader (void)
{
}

void c_KgKmlSpatialContextReader::Dispose ()
{
    delete this;
}


FdoString* c_KgKmlSpatialContextReader::GetName ()
{
    
    return (m_Context->GetName ());
}

FdoString* c_KgKmlSpatialContextReader::GetDescription ()
{
    return (m_Context->GetDescription ());
}

FdoString* c_KgKmlSpatialContextReader::GetCoordinateSystem ()
{
    return (m_Context->GetCoordinateSystem ());
}


FdoString* c_KgKmlSpatialContextReader::GetCoordinateSystemWkt ()
{
    return (m_Context->GetCoordinateSystemWkt ());
}

FdoSpatialContextExtentType c_KgKmlSpatialContextReader::GetExtentType ()
{  
    return (m_Context->GetExtentType ());
}

FdoByteArray* c_KgKmlSpatialContextReader::GetExtent ()
{
    return (m_Context->GetExtent ());
}

const double c_KgKmlSpatialContextReader::GetXYTolerance ()
{
    return (m_Context->GetXYTolerance ());
}

const double c_KgKmlSpatialContextReader::GetZTolerance ()
{
    return (m_Context->GetZTolerance ());
}

const bool c_KgKmlSpatialContextReader::IsActive ()
{
    return (true); 
}

bool c_KgKmlSpatialContextReader::ReadNext ()
{
	bool	rc = false;
	if ( m_CurrIndex < m_SpatialContextColl->GetCount())
	{
		m_Context = m_SpatialContextColl->GetItem( m_CurrIndex++ );
		rc = true;
	}
  return ( rc );
}//end of c_KgKmlSpatialContextReader::ReadNext


