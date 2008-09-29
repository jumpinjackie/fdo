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
#ifndef POSTGISOVERRIDES_H_INCLUDED
#define POSTGISOVERRIDES_H_INCLUDED


namespace fdo { namespace postgis {

/// \brief
/// Dedicated for Schema Overrides mapping defined for PostGIS provider.
///
namespace ov {

#ifdef _WIN32
	const wchar_t* VERSION = L"1.0.0.0";
#endif

}}} // namespace fdo::postgis::ov

#endif // POSTGISOVERRIDES_H_INCLUDED
