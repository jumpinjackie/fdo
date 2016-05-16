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

#include "ShpSpatialIndexHeader.h"

ShpSpatialIndexHeader::ShpSpatialIndexHeader () :
    m_version (SHP_SPATIAL_INDEX_VERSION),
    m_ulSHPSize (0),
    m_maxEntriesPerNode (MAX_NODE_ENTRIES),
    m_minEntriesPerNode (MAX_NODE_ENTRIES / 2),
    m_rTreeHeight (0),
    m_floatPrecision (32),
    m_rootNodeOffset (0),
    m_freeInternalNode (0),
    m_freeLeafNode (0),
    m_type (0),
    m_dimensions (0),
    m_totalObjects (0),
    m_internalNodeSize (0),
    m_leafNodeSize (0)
{
#ifdef _WIN32
    m_ftSHPWriteTime.dwHighDateTime = 0;
    m_ftSHPWriteTime.dwLowDateTime = 0;
#else
    m_ftSHPWriteTime = (time_t)0;
#endif
    m_description[0] = L'\0';
}

void ShpSpatialIndexHeader::ComputeNodeSizes ()
{
    m_internalNodeSize = (4 + 4 * (m_floatPrecision >> 3)) * m_maxEntriesPerNode;
    if (Valid_M ())
        m_internalNodeSize += 2 * (m_floatPrecision >> 3) * m_maxEntriesPerNode;
    if (Valid_Z ())
        m_internalNodeSize += 2 * (m_floatPrecision >> 3) * m_maxEntriesPerNode;
    m_leafNodeSize = m_internalNodeSize;
} 

bool ShpSpatialIndexHeader::Valid_Z ()
{
    return (0 != (m_dimensions && Z_VALID));
}

bool ShpSpatialIndexHeader::Valid_M ()
{
    return (0 != (m_dimensions && M_VALID));
}

void ShpSpatialIndexHeader::SetValid_Z (bool valid)
{
    if (valid)
        m_dimensions |= Z_VALID;
    else
        m_dimensions &= ~Z_VALID;
}

void ShpSpatialIndexHeader::SetValid_M (bool valid)
{
    if (valid)
        m_dimensions |= M_VALID;
    else
        m_dimensions &= ~M_VALID;
}

