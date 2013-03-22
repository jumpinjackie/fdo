#ifndef FDO_GML311_H
#define FDO_GML311_H
// 

//
// Copyright (C) 2009-2010  Autodesk, Inc.
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

#ifdef _WIN32
#pragma once
#endif

/// \brief
/// FdoGml311 is a utility class defining the constants that apply to GML 3.1.1
/// subsequent versions
class FdoGml311:public FdoGml212
{
public:
    /// Various GML types recognized by FDO

	//GML 3 new elements
	FDO_API_COMMON static const FdoStringP mCurveProperty;
	FDO_API_COMMON static const FdoStringP mSurfaceProperty;

	FDO_API_COMMON static const FdoStringP mGeometryArrayProperty;
	FDO_API_COMMON static const FdoStringP mPointArrayProperty;
	FDO_API_COMMON static const FdoStringP mCurveArrayProperty;
	FDO_API_COMMON static const FdoStringP mSurfaceArrayProperty;

};

#endif


