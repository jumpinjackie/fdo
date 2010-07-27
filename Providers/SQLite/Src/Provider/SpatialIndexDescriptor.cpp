// 
//  
//  Copyright (C) 2008 Autodesk Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include "stdafx.h"
#include "StringUtil.h"
#include "SltProvider.h"
#ifndef _MSC_VER
#include "SpatialIndex.h"
#else
#include "DiskSpatialIndex.h"
#endif
#include "SpatialIndexDescriptor.h"

SpatialIndexDescriptor::SpatialIndexDescriptor(const char* tableName, SpatialIndex* spIndex, bool bAutoDelSi)
    : m_spIndex(spIndex),
    m_tablename(tableName),
    m_refCount(1),
    m_bHasUpdates(false),
    m_IsReleased(false),
    m_bAutoDelSi(bAutoDelSi),
    m_xyTolerance(0.0),
    m_zTolerance(0.0)
{
}

SpatialIndexDescriptor::~SpatialIndexDescriptor()
{
    if (m_bAutoDelSi)
        delete m_spIndex;
}
