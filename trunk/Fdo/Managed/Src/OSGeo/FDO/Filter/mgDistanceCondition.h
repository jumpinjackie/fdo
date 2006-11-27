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

#include "FDO\Filter\mgGeometricCondition.h"
#include "FDO\Filter\mgDistanceOperations.h"

class FdoDistanceCondition;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
public __gc class Expression;
public __gc class Identifier;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

using namespace NAMESPACE_OSGEO_FDO_EXPRESSION;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
public __gc __interface IFilterProcessor;

/// \brief
/// The FdoDistanceCondition class derives from FdoGeometricCondition and can be used
/// to test whether the value of a geometric property is within or beyond a
/// specified distance of the specified geometric value. Some feature providers
/// may only support literal geometric values; if so, the provider's capabilities
/// will indicate this limitation.
public __gc class DistanceCondition : public NAMESPACE_OSGEO_FDO_FILTER::GeometricCondition
{
public:

    /// \brief
    /// Constructs a default instance of an FdoDistanceCondition.
    /// 
    /// \return
    /// Returns FdoDistanceCondition
    /// 
	DistanceCondition();

    /// \brief
    /// Constructs an instance of an FdoDistanceCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input property name
    /// \param operation 
    /// Input operation
    /// \param geometry 
    /// Input geometry
    /// \param distance 
    /// Input distance
    /// 
    /// \return
    /// Returns FdoDistanceCondition
    /// 
	DistanceCondition(System::String* propertyName, NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* geometry, System::Double distance);

    /// \brief
    /// Constructs an instance of an FdoDistanceCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input property name
    /// \param operation 
    /// Input operation
    /// \param geometry 
    /// Input geometry
    /// \param distance 
    /// Input distance
    /// 
    /// \return
    /// Returns FdoDistanceCondition
    /// 
	DistanceCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* propertyName, NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* geometry, System::Double distance);

    /// \brief
    /// Gets the distance operation to use.
    /// 
    /// \return
    /// Returns the distance operation
    /// 
	__property NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations get_Operation();

    /// \brief
    /// Sets the distance operation to use.
    /// 
    /// \param value 
    /// Input the distance operation
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Operation(NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations value);

    /// \brief
    /// Gets the literal geometric value.
    /// 
    /// \return
    /// Returns the literal geometric value
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* get_Geometry();

    /// \brief
    /// Sets the literal geometric value.
    /// 
    /// \param value 
    /// Input the literal geometric value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Geometry(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* value);

    /// \brief
    /// Gets the distance value.
    /// 
    /// \return
    /// Returns the distance value
    /// 
	__property System::Double get_Distance();

    /// \brief
    /// Sets the distance value.
    /// 
    /// \param value 
    /// Input the distance value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Distance( System::Double value);

    /// \brief
    /// Overrides FdoFilter.Process to pass the FdoDistanceCondition to the
    /// appropriate filter processor operation.
    /// 
    /// \param processor 
    /// Input filter processor
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor* processor);

	DistanceCondition(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoDistanceCondition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_FILTER


