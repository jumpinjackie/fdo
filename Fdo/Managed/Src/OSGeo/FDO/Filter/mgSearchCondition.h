/*
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
*/

#pragma once

#include "FDO\Filter\mgFilter.h"

class FdoSearchCondition;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER

/// \ingroup (OSGeoFDOFilter)
/// \brief
/// The SearchCondition is an abstract base class. SpatialCondition,
/// ComparisonCondition, LikeCondition, InCondition, and NullCondition derive
/// from the SearchCondition class.
public ref class SearchCondition : public NAMESPACE_OSGEO_FDO_FILTER::Filter
{
public:
    /// \brief
    /// Constructs a SearchCondition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	SearchCondition(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoSearchCondition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_FILTER


