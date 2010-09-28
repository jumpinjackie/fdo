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
#include "Pch.h"
#include "Conv.h"

// std
#include <locale>
#include <string>

namespace fdo { namespace conv {

std::wstring towide(std::string const& narrow)
{
    std::wstring wide(narrow.size(), wchar_t(0)); 
    typedef std::ctype<wchar_t> ctype_t;
    ctype_t const& ct = std::use_facet<ctype_t>(std::locale());
    ct.widen(&narrow[0], &narrow[0] + narrow.size(), &wide[0]); 
    return wide;
}

std::string tonarrow(std::wstring const& wide)
{
    std::string narrow(wide.size(), char(0));
    typedef std::ctype<wchar_t> ctype_t;
    ctype_t const& ct = std::use_facet<ctype_t>(std::locale());
    ct.narrow(&wide[0], &wide[0] + wide.size(), '_', &narrow[0]);
    return narrow;
}


}} // namespace fdo::conv
