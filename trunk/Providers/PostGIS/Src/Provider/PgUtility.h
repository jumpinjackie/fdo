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


enum PgTypeProperties
{
    ePgTypeTextMaxSize = 65536,
    ePgTypeSmallintMax = 32767,
    ePgTypeIntegerMax  = 2147483647
    // Out of range:
    // ePgTypeBigintMax = 9223372036854775807
};

// Add double quotes to PostgreSQL database object name.
// For more details, refer to PostgreSQL manual, 4.1.1. Identifiers and Key Words.
// Example output for given input string content:
// abc   -> "abc"
// "abc" -> "abc"
// ""    -> ""
// <empty> -> <empty>
template <typename T>
inline T QuoteSqlName(T const& name)
{
    if (!name.empty()
        && name[0] != '\"' && name[name.size() - 1]  != '\"')
        return ("\"" + name + "\"");
    else
        return name;
}

// Add single quotes with SQL value of string type.
template <typename T>
inline T QuoteSqlValue(T const& value)
{
    if (!value.empty()
        && value[0] != '\'' && value[value.size() - 1]  != '\'')
        return ("\'" + value + "\'");
    else
        return value;
}

/// Convert PostgreSQL type of given name to FDO type enumerator.
/// \param typeName [in] - name of PostgreSQL data type.
/// \return FDO type enumerator mapped from PostgreSQL type.
///
/// \note Some types like BLOB, CLOB are not supported yet (TODO)
/// and they also are not returned in SchemaCapabilities::GetDataTypes()
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

/// Get textual form of PostgreSQL/PostGIS type from FDO property type.
/// \param
/// prop [in] - pointer to FDO property.
/// \return 
/// Text representing name of PostgreSQL type, ie. character varying, integer, etc.
///
std::string PgTypeFromFdoProperty(FdoPtr<FdoPropertyDefinition> prop);

/// Simple function generating name of sequence associated with given table and column.
/// The association means that particular sequence can be easily detected as working
/// for particular column.
/// The sequence name is formatted in unified way for PostGIS provider:
/// <tablename>_<columnname>_seq
///
/// \param
/// table [in] - name of table.
/// \param
/// column [in] - name of column.
/// \return
/// Name of sequence.
///
std::string MakeSequenceName(std::string const& table, std::string const& column);

}}} // namespace fdo::postgis::details

#endif // FDOPOSTGIS_PGUTILITY_H_INCLUDED
