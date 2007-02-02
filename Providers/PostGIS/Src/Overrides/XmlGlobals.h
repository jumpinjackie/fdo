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
#ifndef FDOPOSTGIS_XMLGLOBALS_H_INCLUDED
#define FDOPOSTGIS_XMLGLOBALS_H_INCLUDED

namespace fdo { namespace postgis { namespace ov {

/// \brief
/// Dedicated for XML names used by Schema Overrides mapping.
///
namespace xml
{
// TODO: Remove 'static' qualifiers but add 'const'

static FdoString* SchemaMapping          = L"SchemaMapping";
static FdoString* ClassDefinitionElement = L"ComplexType";
static FdoString* FullTableNameAttribute = L"FullTableName";
static FdoString* PropertyElement        = L"Element";
static FdoString* ColumnElement          = L"Column";
static FdoString* XmlnsValue             = L"http://fdopostgis.osgeo.org/schemas";

} // namespace xml

}}} // namespace fdo::postgis::ov

#endif // FDOPOSTGIS_XMLGLOBALS_H_INCLUDED
