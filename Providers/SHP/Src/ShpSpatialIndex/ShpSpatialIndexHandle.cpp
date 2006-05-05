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

#include "ShpSpatialIndexHandle.h"

ShpSpatialIndexHandle::ShpSpatialIndexHandle () :
    m_node(NULL)
{
}


ShpSpatialIndexHandle::ShpSpatialIndexHandle (ShpSpatialIndexNode *node) :
    m_node (node)
{
    AttachNode ();
}


ShpSpatialIndexHandle::ShpSpatialIndexHandle (const ShpSpatialIndexHandle &nodeHandle) :
    m_node (nodeHandle.m_node)
{
    AttachNode ();
}


ShpSpatialIndexHandle::~ShpSpatialIndexHandle ()
{
    DetachNode ();
}


ShpSpatialIndexHandle &ShpSpatialIndexHandle::operator= (ShpSpatialIndexNode *newNode)
{
    DetachNode ();
    m_node = newNode;
    AttachNode ();
    return *this;
} 


ShpSpatialIndexHandle &ShpSpatialIndexHandle::operator= (const ShpSpatialIndexHandle &nodeHandle)
{
    DetachNode ();
    m_node = nodeHandle.m_node;
    AttachNode ();
    return *this;
} 


void ShpSpatialIndexHandle::DetachNode ()
{
    if (m_node)
    {
        m_node->m_refCount--;
        m_node = NULL;
    }
}

void ShpSpatialIndexHandle::AttachNode ()
{
    if (m_node)
        m_node->m_refCount++;
}

