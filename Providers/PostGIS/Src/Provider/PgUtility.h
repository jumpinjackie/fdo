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
typedef std::vector
    <
        std::pair<std::string, int>
    >
pgexec_params_t;

}}} // namespace fdo::postgis::details

#endif // FDOPOSTGIS_PGUTILITY_H_INCLUDED
