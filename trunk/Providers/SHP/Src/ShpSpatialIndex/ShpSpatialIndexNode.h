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

#ifndef SHPSPATIALINDEXNODE_H
#define SHPSPATIALINDEXNODE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpSpatialIndex.h"

#ifdef _WIN32
#pragma pack(push)
#pragma pack(4)
#endif

/// <summary>Used to store the contents of an R-Tree node.</summary>
/// <remarks>The node may either be a leaf or an internal node.
/// NOTE - the level of the node is stored as opposed to the height
/// (the level of the node is measured with respect to the leaves,
/// with the leaves being at level 0). 
/// The advantage of using the level is that it remains fixed even 
/// when the R-Tree grows or shrinks in height.<remarks><remarks>
struct ShpSpatialIndexNode {
    ShpSpatialIndexNode ();

    // bookkeeping information 

    unsigned long m_ssiOffset;   // file offset of the node
    int m_refCount;              // reference count 
    unsigned long m_lruStamp;    // least-recently-used stamp
    unsigned m_currentSubtree;   // current subtree being searched
    unsigned m_nodeLevel;        // level of the node
    BOOL m_nodeModified;         // has the node been modified?
    unsigned m_nEntries;         // number of entries in the node

    // data representing the R-Tree node

    unsigned long m_fileOffset[MAX_NODE_ENTRIES]; // file offset of children
    BoundingBoxEx m_childExt[MAX_NODE_ENTRIES];   // bounding boxes of children
};

#ifdef _WIN32
#pragma pack(pop)
#endif

#endif // SHPSPATIALINDEXNODE_H

