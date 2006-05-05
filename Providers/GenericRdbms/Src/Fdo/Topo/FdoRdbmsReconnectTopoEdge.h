#ifndef _FDORDBMSRECONNECTTOPOEDGE_H_
#define _FDORDBMSRECONNECTTOPOEDGE_H_
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
#include "Fdo/Commands/Topology/IReconnectTopoEdge.h"


/// <summary>
/// The FdoIReconnectTopoEdge interface defines the ReconnectTopoEdge command,
/// which modifies an edge's connection(s) to a node to move to a different node, and
/// allows the geometry of the edge to change simultaneously.
/// Other changes to topology are not allowed by this command.
/// Any features that have topological relationships to the edge will be affected.
/// </summary>
class FdoRdbmsReconnectTopoEdge : public FdoIReconnectTopoEdge
{
public:
    /// <summary>Gets the name of the topology to affect.  This is the accessor
    /// for the value that the caller sets via SetTopologyName.</summary>
    /// <returns>Returns the identifier of the node to be moved.</returns>
    FDORDBMS_API virtual FdoString * GetTopologyName();

    /// <summary>Sets the name of the topology to affect.</summary>
    /// <param name="topologyName">Input the name of the topology to affect.</param>
    FDORDBMS_API virtual void SetTopologyName(FdoString * topologyName);

    /// <summary>Gets the identifier of the edge to be reconnected.  This is the accessor
    /// for the value that the caller sets via SetEdgeId.</summary>
    /// <returns>Returns the identifier of the edge to be moved.</returns>
    FDORDBMS_API virtual FdoPropertyValueCollection * GetEdgeId();

    /// <summary>Sets the identifier of the edge to be moved.</summary>
    /// <param name="edgeId">Input the identifier of the edge to be moved.</param>
    FDORDBMS_API virtual void SetEdgeId(FdoPropertyValueCollection * edgeId);

    /// <summary>Gets the properties of the edge to be moved.  The caller would be expected
    /// to assign a new value for the "Geometry" property, as well as assigning a
    /// new value for one distinct node -- either "StartNode" or "EndNode", or both if they
    /// were the same previously.  Normally, the StartNode and EndNode properties are read-only
    /// and system-maintained.  They are caller-updateable for the ReconnectTopoEdge command.
    /// </summary>
    /// <returns>Returns the list of the edge's properties and their values.</returns>
    FDORDBMS_API virtual FdoPropertyValueCollection* GetEdgePropertyValues();

    /// <summary>Executes the MoveTopoEdge command and, if FdoIActivateTopologyInCommandResults
    /// is in effect, returns a reader with the affected primitives and features.</summary>
    /// <returns>Returns a reader with affected primitives and features.</returns>
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
