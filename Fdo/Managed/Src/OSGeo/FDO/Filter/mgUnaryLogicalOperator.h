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

#include "FDO\Filter\mgLogicalOperator.h"
#include "FDO\Filter\mgUnaryLogicalOperations.h"

class FdoUnaryLogicalOperator;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
ref class Filter;
interface class IFilterProcessor;

/// \ingroup (OSGeoFDOFilter)
/// \brief
/// The UnaryLogicalOperator class derives from LogicalOperator and allows a
/// logical "Not" to be performed against a filter.
public ref class UnaryLogicalOperator : public NAMESPACE_OSGEO_FDO_FILTER::LogicalOperator
{
public:
    /// \brief
    /// Constructs a default instance of an UnaryLogicalOperator.
    /// 
    /// \return
    /// Returns UnaryLogicalOperator
    /// 
	UnaryLogicalOperator();

    /// \brief
    /// Constructs an instance of an UnaryLogicalOperator using the specified arguments.
    /// 
    /// \param operand 
    /// Input operand
    /// \param operation 
    /// Input operation
    /// 
    /// \return
    /// Returns UnaryLogicalOperator
    /// 
	UnaryLogicalOperator(NAMESPACE_OSGEO_FDO_FILTER::Filter^ operand, NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperations operation);

    /// \brief
    /// Gets the unary operation to be performed on the operand.
    /// 
    /// \return
    /// Returns the unary operation
    /// 
    /// \brief
    /// Sets the unary operation to be performed on the operand.
    /// 
    /// \param value 
    /// Input the unary operation
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperations Operation
    {
        NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperations get();
        System::Void set(NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperations value);
    }

    /// \brief
    /// Gets Filter operand to apply the operator to.
    /// 
    /// \return
    /// Returns filter
    /// 
    /// \brief
    /// Sets Filter operand to apply the operator to.
    /// 
    /// \param value 
    /// Input filter
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_FILTER::Filter^ Operand
    {
        NAMESPACE_OSGEO_FDO_FILTER::Filter^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_FILTER::Filter^ value);
    }

    /// \brief
    /// Overrides Filter.Process to pass UnaryLogicalOperator to the
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
    /// Constructs a UnaryLogicalOperator object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	UnaryLogicalOperator(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoUnaryLogicalOperator* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_FILTER


