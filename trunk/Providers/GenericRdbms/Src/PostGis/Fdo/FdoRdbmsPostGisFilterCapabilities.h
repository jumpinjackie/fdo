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
#ifndef FDORDBMSPOSTGISFILTERCAPABILITIES_H
#define FDORDBMSPOSTGISFILTERCAPABILITIES_H

#include "../../Fdo/Capability/FdoRdbmsFilterCapabilities.h"

/// \brief
/// Implementats interface to query PostGIS provider capabilities 
/// regarding support of FdoFilter classes.
/// 
/// \sa FdoIFilterCapabilities - declaration of interface
/// implemented by following class.
///
class FdoRdbmsPostGisFilterCapabilities :
    public FdoRdbmsFilterCapabilities
{
public:

    /// Default constructor.
	FdoRdbmsPostGisFilterCapabilities();

public:

    /// Destructor.
	virtual ~FdoRdbmsPostGisFilterCapabilities();

    /// Fetch list of FdoSpatialOperation objects supported by
    /// PostGIS provider.
    /// 
    /// @param length [in] - number of spatial operations in returned list.
    /// @return List of spatial operations and number of elements in the list.
    /// 
	virtual FdoSpatialOperations *GetSpatialOperations(int& length);

    /// Fetch list of FdoDistanceOperation objects supported by
    /// PostGIS provider.
    /// 
    /// @param length [in] - number of distance operations in returned list.
    /// @return List of distance operations and number of elements in the list.
    /// 
	virtual FdoDistanceOperations* GetDistanceOperations(int& length);

};

#endif // FDORDBMSPOSTGISFILTERCAPABILITIES_H
