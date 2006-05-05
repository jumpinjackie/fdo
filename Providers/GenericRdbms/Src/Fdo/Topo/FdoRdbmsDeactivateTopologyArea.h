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

#ifndef _FDORDBMSDEACTIVATETOPOLOGYAREA_H_
#define _FDORDBMSDEACTIVATETOPOLOGYAREA_H_

#include "FdoRdbms.h"
#include "FdoRdbmsCommand.h"
#include "Fdo/Commands/Topology/IDeactivateTopologyArea.h"


/// <summary>
/// This command deactivates an area that was activated using
/// FdoIActivateTopologyArea.
///
/// Deactivating an area using this FDO command may be a
/// fairly expensive operation on some providers, as it can cause caches to be
/// purged to a data store.
//
/// Any re-loading or re-locking of topological data during a FDO end of
/// transaction will follow the FDO provider’s policy for non-topological data.
/// This may entail an implicit de-activation of the topology area.
/// </summary>
class FdoRdbmsDeactivateTopologyArea : public FdoIDeactivateTopologyArea
{
    public:
    /// <summary>Gets the name of the topology to deactivate.  This is the accessor
    /// for the value that the caller sets via SetName.
    /// </summary>
    /// <returns>Returns the name of the topology to deactivate.</returns>
    FDORDBMS_API virtual FdoString * GetName();

    /// <summary> Sets the name of the topology to deactivate.
    /// </summary>
    /// <returns>Returns nothing.</returns>
    FDORDBMS_API virtual void SetName(
        FdoString * name
    );

    /// <summary>Executes the command.</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void Execute();
};



#endif /* _FDORDBMSDEACTIVATETOPOLOGYAREA_H_ */
