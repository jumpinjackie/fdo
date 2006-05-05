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

#ifndef _FDORDBMSACTIVATETOPOLOGYINCOMMANDRESULTS_H_
#define _FDORDBMSACTIVATETOPOLOGYINCOMMANDRESULTS_H_

#include "FdoRdbms.h"
#include "FdoRdbmsCommand.h"
#include "Fdo/Commands/Topology/IActivateTopologyInCommandResults.h"

/// <summary>
/// This command controls how topology is handled in subsequent commands that
/// affect features.  The affected commands are: FdoIInsert, FdoIUpdate,
/// FdoIDelete, FdoIDelete.  The contents of the FdoIFeatureReader interface
/// that is returned from the Execute() method on these commands will be
/// enriched with extra information for topology-aware client applications.
/// Acquiring this extra information may increase execution time.
/// </summary>
class FdoRdbmsActivateTopologyInCommandResults : public FdoIActivateTopologyInCommandResults
{
    public:

    /// <summary>Executes the command.</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void Execute();
};



#endif /* _FDORDBMSACTIVATETOPOLOGYINCOMMANDRESULTS_H_ */
