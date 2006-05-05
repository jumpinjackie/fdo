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

#ifndef _FDORDBMSDEACTIVATETOPOLOGYINCOMMANDRESULTS_H_
#define _FDORDBMSDEACTIVATETOPOLOGYINCOMMANDRESULTS_H_

#include "FdoRdbms.h"
#include "FdoRdbmsCommand.h"
#include "Fdo/Commands/Topology/IDeactivateTopologyInCommandResults.h"


/// <summary>
/// This command deactivates the return of extra topology information if it was
/// activated by a call to IActivateTopologyInCommandResults.
/// </summary>
class FdoRdbmsDeactivateTopologyInCommandResults : public FdoIDeactivateTopologyInCommandResults
{
    public:

    /// <summary>Executes the command.</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void Execute();
};



#endif /* _FDORDBMSDEACTIVATETOPOLOGYINCOMMANDRESULTS_H_ */
