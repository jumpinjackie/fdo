#ifndef _FDORDBMSMOVETOPONODE_H_
#define _FDORDBMSMOVETOPONODE_H_
//
/ //
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
//

#include "FdoRdbms.h"
#include "FdoRdbmsCommand.h"
#include "Fdo/Commands/Topology/IMoveTopoNode.h"

/// <summary>
/// The FdoIMoveTopoNode interface defines the MoveTopoNode command,
/// which modifies the location of a node in a topology, and connected edges,
/// without changing topological relationships.
/// </summary>
class FdoRdbmsMoveTopoNode : public FdoIMoveTopoNode
{
public:
    /// <summary>Gets the name of the topology to affect.  This is the accessor
    /// for the value that the caller sets via SetTopologyName.</summary>
    /// <returns>Returns the identifier of the node to be moved.</returns>
    FDORDBMS_API virtual FdoString * GetTopologyName();

    /// <summary>Sets the name of the topology to affect.</summary>
    /// <param name="topologyName">Input the name of the topology to affect.</param>
    FDORDBMS_API virtual void SetTopologyName(FdoString * topologyName);

    /// <summary>Gets the identifier of the node to be moved.  This is the accessor
    /// for the value that the caller sets via SetNodeId.</summary>
    /// <returns>Returns the identifier of the node to be moved.</returns>
    FDORDBMS_API virtual FdoPropertyValueCollection * GetNodeId();

    /// <summary>Sets the identifier of the node to be moved.</summary>
    /// <param name="nodeId">Input the identifier of the node to be moved.</param>
    FDORDBMS_API virtual void SetNodeId(FdoPropertyValueCollection * nodeId);

    /// <summary>Gets the properties of the node to be moved.  The caller would be expected
    /// to assign a new value for the "Geometry" property.</summary>
    /// <returns>Returns the list of the node's properties and their values.</returns>
    FDORDBMS_API virtual FdoPropertyValueCollection* GetNodePropertyValues();

    /// <summary>Gets the batch of properties of all the edges to be moved
    /// along with the node.  The caller would be expected to assign new values
    /// for the "Geometry" property of each edge.  The specific edges in the batch
    /// are known from the node identified by SetNodeId.</summary>
    /// <returns>Returns the list of the edges' properties and their values.</returns>
    FDORDBMS_API virtual FdoBatchParameterValueCollection * GetEdgePropertyValues();

    /// <summary>Executes the MoveTopoNode command and, if FdoIActivateTopologyInCommandResults
    /// is in effect, returns a reader with the affected features.</summary>
    /// <returns>Returns a reader with affected features.</returns>
    FDORDBMS_API virtual FdoIFeatureReader * Execute();

    /// <summary> Updating objects might result in lock conflicts if objects
    /// to be updated are not exclusively locked for the user attempting to
    /// update the object. If objects to be updated are not exclusively locked for the
    /// user attempting to update the object, a lock conflict report is generated.
    /// The function GetLockConflicts returns a lock conflict reader providing
    /// access to the list of lock conflicts that occurred during the execution
    /// of the update operation.</summary>
    /// <returns>Returns a lock conflict reader.</returns>
    FDORDBMS_API virtual FdoILockConflictReader* GetLockConflicts();

};
#endif
