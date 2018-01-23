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

#ifndef SHPSPATIALINDEXHANDLE_H
#define SHPSPATIALINDEXHANDLE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpSpatialIndex.h"
#include "ShpSpatialIndexNode.h"

#ifdef _WIN32
#pragma pack(push)
#pragma pack(4)
#endif

/// <summary>Mimic a pointer to a ShpSpatialIndexNode.</summary>
/// <remarks>The handle class maintains the reference count 
/// of the associated node in order to prevent the node from being
/// accidentally removed from the LRU cache while it is still in use.</remarks>
struct ShpSpatialIndexHandle {
    ShpSpatialIndexHandle ();
    ShpSpatialIndexHandle (ShpSpatialIndexNode *node);
    ShpSpatialIndexHandle (const ShpSpatialIndexHandle &nodeHandle);
    ~ShpSpatialIndexHandle ();

    ShpSpatialIndexHandle &operator= (ShpSpatialIndexNode *node);
    ShpSpatialIndexHandle &operator= (const ShpSpatialIndexHandle &nodeHandle);
    ShpSpatialIndexNode *operator-> () {return m_node;}
    operator ShpSpatialIndexNode * () {return m_node;}
    void DetachNode ();

private:
    ShpSpatialIndexNode *m_node;

    void AttachNode ();
};

#ifdef _WIN32
#pragma pack(pop)
#endif

#endif // SHPSPATIALINDEXHANDLE_H

