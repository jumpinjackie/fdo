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

#ifndef _FDORDBMSACTIVATETOPOLOGYAREA_H_
#define _FDORDBMSACTIVATETOPOLOGYAREA_H_

#include "FdoRdbms.h"
#include "FdoRdbmsCommand.h"
#include "Fdo/Commands/Topology/IActivateTopologyArea.h"

/// <summary>
/// Using this command, the client application can control which topology
/// data is in an area of interest.  This is an optional performance hint
/// whose effects will vary by provider.
/// </summary>
class FdoRdbmsActivateTopologyArea : public FdoIActivateTopologyArea
{
    public:

    /// <summary>Gets the name of the topology to activate.  This is the accessor
    /// for the value that the caller sets via SetName.
    /// </summary>
    /// <returns>Returns the name of the topology to activate.</returns>
    FDORDBMS_API virtual FdoString * GetName();

    /// <summary> Sets the name of the topology to activate.
    /// </summary>
    /// <returns>Returns nothing.</returns>
    FDORDBMS_API virtual void SetName(
        FdoString * name
    );

    /// <summary>Gets the area in which to work.  This is the accessor
    /// for the value that the caller sets via SetArea.
    /// </summary>
    /// <returns>Returns the area in which to work.</returns>
    FDORDBMS_API virtual FdoByteArray * GetArea();

    /// <summary> Sets the area in which to work.  This allows the caller
    /// to tell FDO where feature edits are expected to happen.
    /// The only supported area is an extent, given as a rectangular geometry.
    /// To acquire such a geometry, use:
    ///   FdoGeometryFactoryAbstract::CreateGeometry(FdoIEnvelope *);
    /// </summary>
    /// <param name="area">Input the area in which to work.</param>
    /// <returns>Returns nothing.</returns>
    FDORDBMS_API virtual void SetArea(
        FdoByteArray * area
    );

    /// <summary>Executes the command.</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void Execute();
};



#endif /* _FDORDBMSACTIVATETOPOLOGYAREA_H_ */
