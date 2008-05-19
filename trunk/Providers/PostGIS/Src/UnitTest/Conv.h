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
#ifndef FDOPOSTGIS_FDOCONV_H_INCLUDED
#define FDOPOSTGIS_FDOCONV_H_INCLUDED

// std
#include <locale>
#include <string>

namespace fdo { namespace conv {

/// Convert one byte characters string to wide characters string.
/// It performs convertion from a smaller character set to a larger one.
std::wstring towide(std::string const& narrow);

/// Convert wide characters string to one byte characters string.
/// It performs convertion from a larger character set to a smaller one.
std::string tonarrow(std::wstring const& wide);


}} // namespace fdo::conv

#endif // FDOPOSTGIS_FDOCONV_H_INCLUDED
