// $Id$
//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_GETSPATIALCONTEXTCOMMAND_H_INCLUDED
#define FDOPOSTGIS_GETSPATIALCONTEXTCOMMAND_H_INCLUDED

#include "Command.h"

//
// Forward declarations
//
class FdoISpatialContextReader;

namespace fdo { namespace postgis {

// Forward declarations of PostGIS classes
class Connection;

/// Implementation of interface of command which enumerates set of existing
/// spatial contexts.
///
class GetSpatialContextsCommand : public Command<FdoIGetSpatialContexts>
{
public:

    /// Constructor create new command retrieving spatial context available
    /// through current connection.
    GetSpatialContextsCommand(Connection* connection);

    //
    // FdoIGetSpatialContexts interface
    //

    /// Get flag indicating if the command will return only active spatial
    /// context or all spatial contexts.
    /// \return
    /// False indicates that all spatial contexts will be returned.
    ///
    const bool GetActiveOnly();

    /// Set value for flag indicating if the command will return only active
    /// spatial context or all spatial contexts.
    /// \param
    /// activeOnly [in] - active context retrival indicator.
    ///
    void SetActiveOnly(bool const activeOnly);

    /// Execute the command and create spatial contexts reader.
    /// \return
    /// Interface to new instance of FdoISpatialContextReader type.
    ///
    FdoISpatialContextReader* Execute();

protected:

    /// Destructor.
    virtual ~GetSpatialContextsCommand();    

private:
	
    //
    // Private data members
    //

    bool mActiveOnly;
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_GETSPATIALCONTEXTCOMMAND_H_INCLUDED
