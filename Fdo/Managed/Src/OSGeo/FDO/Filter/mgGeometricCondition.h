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

#include "FDO\Filter\mgSearchCondition.h"

class FdoGeometricCondition;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
ref class Identifier;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

using namespace NAMESPACE_OSGEO_FDO_EXPRESSION;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER

/// \ingroup (OSGeoFDOFilter)
/// \brief
/// GeometricCondition is an abstract class that derives from SearchCondition and can
/// be used to test whether the value of a geometric property meets a particular
/// spatial criteria. GeometricCondition is the base class for DistanceCondition
/// and SpatialCondition.
public ref class GeometricCondition : public NAMESPACE_OSGEO_FDO_FILTER::SearchCondition
{
public:
    /// \brief
    /// Gets the name of the geometric property.
    /// 
    /// \return
    /// Returns the name of the geometric property
    /// 
    /// \brief
    /// Sets the name of the geometric property.
    /// 
    /// \param value 
    /// Input the name of the geometric property
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ PropertyName
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value);
    }

    /// \brief
    /// Constructs a GeometricCondition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	GeometricCondition(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoGeometricCondition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_FILTER


