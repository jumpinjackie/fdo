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
#include "FDO\Filter\mgSpatialOperations.h"

class FdoSpatialCondition;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
ref class Expression;
ref class Identifier;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
interface class IFilterProcessor;

/// \ingroup (OSGeoFDOFilter)
/// \brief
/// The SpatialCondition class derives from GeometricCondition and can be used
/// to test whether the value of a geometric property (referenced using the name
/// of the property) and another geometric value satisfy the spatial
/// relationship implied by the operation. Some feature providers may only
/// support literal geometric values; if so, the provider's capabilities 
/// will indicate this limitation.
public ref class SpatialCondition : public NAMESPACE_OSGEO_FDO_FILTER::GeometricCondition
{
public:
    /// \brief
    /// Constructs a default instance of an SpatialCondition.
    /// 
    /// \return
    /// Returns SpatialCondition
    /// 
	SpatialCondition();

    /// \brief
    /// Constructs an instance of an SpatialCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input property name
    /// \param operation 
    /// Input spatial operation
    /// \param geometry 
    /// Input geometry
    /// 
    /// \return
    /// Returns SpatialCondition
    /// 
	SpatialCondition(System::String^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ geometry);

    /// \brief
    /// Constructs an instance of an SpatialCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input property name
    /// \param operation 
    /// Input spatial operation
    /// \param geometry 
    /// Input geometry
    /// 
    /// \return
    /// Returns SpatialCondition
    /// 
	SpatialCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations operation,  NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ geometry);

    /// \brief
    /// Gets the spatial operation to use.
    /// 
    /// \return
    /// Returns the spatial operation to use
    /// 
    /// \brief
    /// Sets the spatial operation to use.
    /// 
    /// \param value 
    /// Input the spatial operation to use
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations Operation
    {
        NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations get();
        System::Void set(NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations value);
    }

    /// \brief
    /// Gets the literal geometric value.
    /// 
    /// \return
    /// Returns the literal geometric valueg
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
    /// Overrides Filter.Process to pass SpatialCondition to the
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
    /// Constructs a SpatialCondition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	SpatialCondition(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoSpatialCondition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_FILTER


