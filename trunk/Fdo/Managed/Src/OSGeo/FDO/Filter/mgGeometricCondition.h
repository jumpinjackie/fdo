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
public __gc class Identifier;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

using namespace NAMESPACE_OSGEO_FDO_EXPRESSION;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER

/// \brief
/// The FdoGeometricCondition abstract class derives from FdoSearchCondition and can
/// be used to test whether the value of a geometric property meets a particular
/// spatial criteria. FdoGeometricCondition is the base class for FdoDistanceCondition
/// and FdoSpatialCondition.
public __gc class GeometricCondition : public NAMESPACE_OSGEO_FDO_FILTER::SearchCondition
{
public:
    /// \brief
    /// Gets the name of the geometric property.
    /// 
    /// \return
    /// Returns the name of the geometric property
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* get_PropertyName();

    /// \brief
    /// Sets the name of the geometric property.
    /// 
    /// \param value 
    /// Input the name of the geometric property
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_PropertyName(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* value);

	GeometricCondition(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoGeometricCondition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_FILTER


