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
#ifndef FDOPOSTGIS_PGUTILITY_H_INCLUDED
#define FDOPOSTGIS_PGUTILITY_H_INCLUDED

#include <Fdo/Schema/DataType.h>
// std
#include <string>
#include <vector>
#include <utility>
// boost
#include <boost/tuple/tuple.hpp>

namespace fdo { namespace postgis {
    
/// \brief
/// Namespace for various implementation details, internal definitions.
///
///
namespace details {

/// Type of Septuple (7-elements tuple) for connection info parameters used to
/// establish connection with PostgreSQL database server.
///
typedef boost::tuples::tuple
    <
        std::string, std::string, std::string, std::string, std::string, std::string, std::string
    >
pgconn_params_t;

/// Type of collection for parameters passed to command execution procedures.
/// Every element of the collection is a pair of character representation o
/// data value and NULL indicator (0 = non-null; 1 or higher = null value).
/// If NULL indicator is set with a non-Zero value, then the data compund is ignored,
/// and should be set to empty string.
///
typedef std::vector
    <
        std::pair<std::string, int>
    >
pgexec_params_t;

/// Convert PostgreSQL type of given name to FDO type enumerator.
/// \param typeName [in] - name of PostgreSQL data type.
/// \return FDO type enumerator mapped from PostgreSQL type.
///
FdoDataType FdoTypeFromPgTypeName(std::string const& typeName);

/// Get value of type size from PQfmod mask.
/// This function does only work for CHARACTER VARYING and TEXT types if length
/// was specified during table creation.
/// Caller is supposed to check type and decide about calling this function or not.
int GetTypeMaxSize(int const& modifier);

/// Get value of NUMERIC type precision from PQfmod mask.
/// This function does only work for NUMERIC (DECIMAL) type.
/// Caller is supposed to check type and decide about calling this function or not.
int GetTypePrecision(int const& modifier);

/// Get value of NUMERIC type scale from PQfmod mask.
/// This function does only work for NUMERIC (DECIMAL) type.
/// Caller is supposed to check type and decide about calling this function or not.
int GetTypeScale(int const& modifier);

}}} // namespace fdo::postgis::details

#endif // FDOPOSTGIS_PGUTILITY_H_INCLUDED
