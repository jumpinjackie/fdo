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
ref class Expression;
ref class Identifier;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

using namespace NAMESPACE_OSGEO_FDO_EXPRESSION;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
interface class IFilterProcessor;

/// \ingroup (OSGeoFDOFilter)
/// \brief
/// The DistanceCondition class derives GeometricCondition and can be used
/// to test whether the value of a geometric property is within or beyond a
/// specified distance of the specified geometric value. Some feature providers
/// may only support literal geometric values; if so, the provider's capabilities
/// will indicate this limitation.
public ref class DistanceCondition : public NAMESPACE_OSGEO_FDO_FILTER::GeometricCondition
{
public:

    /// \brief
    /// Constructs a default instance of an DistanceCondition.
    /// 
    /// \return
    /// Returns DistanceCondition
    /// 
	DistanceCondition();

    /// \brief
    /// Constructs an instance of an DistanceCondition using the specified arguments.
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
    /// Returns DistanceCondition
    /// 
	DistanceCondition(System::String^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ geometry, System::Double distance);

    /// \brief
    /// Constructs an instance of an DistanceCondition using the specified arguments.
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
    /// Returns DistanceCondition
    /// 
	DistanceCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ geometry, System::Double distance);

    /// \brief
    /// Gets the distance operation to use.
    /// 
    /// \return
    /// Returns the distance operation
    /// 
    /// \brief
    /// Sets the distance operation to use.
    /// 
    /// \param value 
    /// Input the distance operation
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations Operation
    {
        NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations get();
        System::Void set(NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations value);
    }

    /// \brief
    /// Gets the literal geometric value.
    /// 
    /// \return
    /// Returns the literal geometric value
    /// 
    /// \brief
    /// Sets the literal geometric value.
    /// 
    /// \param value 
    /// Input the literal geometric value
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ Geometry
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ value);
    }

    /// \brief
    /// Gets the distance value.
    /// 
    /// \return
    /// Returns the distance value
    /// 
    /// \brief
    /// Sets the distance value.
    /// 
    /// \param value 
    /// Input the distance value
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double Distance
    {
        System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Overrides Filter.Process to pass DistanceCondition to the
    /// appropriate filter processor operation.
    /// 
    /// \param processor 
    /// Input filter processor
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor^ processor);

    /// \brief
    /// Constructs a DistanceCondition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	DistanceCondition(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoDistanceCondition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_FILTER


