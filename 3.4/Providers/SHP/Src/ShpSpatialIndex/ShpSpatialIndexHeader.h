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

#ifndef SHPSPATIALINDEXHEADER_H
#define SHPSPATIALINDEXHEADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpSpatialIndex.h"

const int SHP_SPATIAL_INDEX_VERSION = 2;                          // SSI version number

// dimension masks
#define Z_VALID 1
#define M_VALID 2

#ifdef _WIN32
#pragma pack(push)
#pragma pack(4)
#endif

/// <summary>Used to store the information contained in the header block of a shape spatial index.</summary>
struct ShpSpatialIndexHeader
{
    ShpSpatialIndexHeader ();
    void ComputeNodeSizes ();
    bool Valid_Z ();
    bool Valid_M ();
    void SetValid_Z (bool valid);
    void SetValid_M (bool valid);

    unsigned m_version;                     // SSI version number

    // The two following pieces of info are used in synchronizing SHP and SSI files
    FILETIME m_ftSHPWriteTime;              // The time when the corresponding SHP was last written
    unsigned long m_ulSHPSize;              // The size of the corresponding SHP file

    unsigned m_maxEntriesPerNode;           // maximum entries per R-Tree node
    unsigned m_minEntriesPerNode;           // minimum entries per R-Tree node
    unsigned m_rTreeHeight;                 // height of the R-Tree
    unsigned m_floatPrecision;              // floating point precision (in bits)
    unsigned long m_rootNodeOffset;         // file offset of root node
    unsigned long m_freeInternalNode;       // file offset of free internal node
    unsigned long m_freeLeafNode;           // file offset of free leaf node
    unsigned long m_type;                   // type of object in the index
    unsigned m_dimensions;                  // flags for Z and M
    unsigned long m_totalObjects;           // total number of objects in the R-Tree
    TCHAR m_description[256/sizeof(TCHAR)]; // SSI description
    unsigned m_internalNodeSize;            // size of internal node in bytes
    unsigned m_leafNodeSize;                // size of leaf node in bytes
};

#ifdef _WIN32
#pragma pack(pop)
#endif

#endif // SHPSPATIALINDEXHEADER_H

