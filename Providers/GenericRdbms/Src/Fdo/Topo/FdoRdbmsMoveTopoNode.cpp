 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdafx.h"

#include "inc/Fdo/Rdbms/FdoRdbmsMoveTopoNode.h"


FdoString * FdoRdbmsMoveTopoNode::GetTopologyName()
{
    return 0;
}

void FdoRdbmsMoveTopoNode::SetTopologyName(FdoString * topologyName)
{
}

FdoPropertyValueCollection * FdoRdbmsMoveTopoNode::GetNodeId()
{
    return 0;
}

void FdoRdbmsMoveTopoNode::SetNodeId(FdoPropertyValueCollection * nodeId)
{
}

FdoPropertyValueCollection* FdoRdbmsMoveTopoNode::GetNodePropertyValues()
{
    return 0;
}

FdoBatchParameterValueCollection * FdoRdbmsMoveTopoNode::GetEdgePropertyValues()
{
    return 0;
}

FdoIFeatureReader * FdoRdbmsMoveTopoNode::Execute()
{
    return 0;
}

FdoILockConflictReader* FdoRdbmsMoveTopoNode::GetLockConflicts()
{
    return 0;
}
