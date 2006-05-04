#ifndef _GEOMETRYIMPL_H_
#define _GEOMETRYIMPL_H_
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
*/

#ifdef _WIN32
#pragma once
#endif

#include <Common.h>
#include <Geometry/IGeometry.h>
#include "GeometryUtility.h"

// This template exists to provide some common code among all implementations
// of descendants of FdoIGeometry, while avoiding "diamond inheritance" between
// FdoIGeometry and partial implementation code.

template <class FDO_GEOMETRY_CLASS> class FdoGeometryImpl: public FDO_GEOMETRY_CLASS
{

public:

    virtual FdoString* GetText()
    {
        if ( 0 == m_fgft )
        {
            m_fgft = GeometryUtility::CreateGeometryText(this);
        }
        return m_fgft;
    }

    FdoString* m_fgft;   // Cached FGFT string representation of the geometry.

protected:
    // Protect default constructor so that a factory must be used.
    FdoGeometryImpl() : m_fgft(0) {};

    // Destructor
	virtual ~FdoGeometryImpl()
    {
        if (0 != m_fgft)
        {
            delete [] m_fgft;
            m_fgft = 0;
        }
    }
};


#endif

