//
// Copyright (C) 2006 Refractions Research, Inc. 
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
#ifndef FDORDBMSPOSTGISCOMMANDCAPABILITIES_H
#define FDORDBMSPOSTGISCOMMANDCAPABILITIES_H

#include "../../Fdo/Capability/FdoRdbmsCommandCapabilities.h"

/// \brief
/// Implements interface to query supported
/// commands by PostGIS provider.
/// 
/// \sa FdoICommandCapabilities - declaration of interface
/// implemented by following class.
///
class FdoRdbmsPostGisCommandCapabilities :
    public FdoRdbmsCommandCapabilities
{
public:

    /// Default constructor.
	FdoRdbmsPostGisCommandCapabilities();

    /// Gets list of commands supported types by PostGIS provider.
    /// The list consists elements of type of FdoCommandType enumeration.
    ///
    /// @param size [out] - number of elements in returned list.
    /// @return List of supported commands.
    ///
	virtual int* GetCommands(int& size);

protected:

    /// Destructor.
    ~FdoRdbmsPostGisCommandCapabilities();

};

#endif // FDORDBMSPOSTGISCOMMANDCAPABILITIES_H
