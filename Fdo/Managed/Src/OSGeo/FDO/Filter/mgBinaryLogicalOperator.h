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
#include "FDO\Filter\mgBinaryLogicalOperations.h"

class FdoBinaryLogicalOperator;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
ref class Filter;
interface class IFilterProcessor;

/// \ingroup (OSGeoFDOFilter)
/// \brief
/// The BinaryLogicalOperator class derives from LogicalOperator and allows two
/// filters to be combined via a logical "and" or a logical "or".
public ref class BinaryLogicalOperator : public NAMESPACE_OSGEO_FDO_FILTER::LogicalOperator
{
public:
    /// \brief
    /// Constructs a default instance of an BinaryLogicalOperator.
    /// 
    /// \return
    /// Returns BinaryLogicalOperator
    /// 
	BinaryLogicalOperator();

    /// \brief
    /// Constructs an instance of an BinaryLogicalOperator using the specified arguments.
    /// 
    /// \param leftOperand 
    /// Input left hand operand
    /// \param operation 
    /// Input operator
    /// \param rightOperand 
    /// Input right hand operand
    /// 
    /// \return
    /// Returns BinaryLogicalOperator
    /// 
	BinaryLogicalOperator(NAMESPACE_OSGEO_FDO_FILTER::Filter^ leftOperand, NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations operation, NAMESPACE_OSGEO_FDO_FILTER::Filter^ rightOperand);

    /// \brief
    /// Gets the operation to be performed between the two operands.
    /// 
    /// \return
    /// Returns the operation to be performed
    /// 
    /// \brief
    /// Sets the operation to be performed between the two operands.
    /// 
    /// \param value 
    /// Input the operation to be performed
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations Operation
    {
        NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations get();
        System::Void set(NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations value);
    }

    /// \brief
    /// Gets the filter that defines the left operand.
    /// 
    /// \return
    /// Returns the filter that defines the left operand
    /// 
    /// \brief
    /// Sets the filter that defines the left operand.
    /// 
    /// \param value 
    /// Input the filter that defines the left operand
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_FILTER::Filter^ LeftOperand
    {
        NAMESPACE_OSGEO_FDO_FILTER::Filter^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_FILTER::Filter^ value);
    }

    /// \brief
    /// Gets the filter that defines the right operand.
    /// 
    /// \return
    /// Returns the filter that defines the right operand
    /// 
    /// \brief
    /// Sets the filter that defines the right operand.
    /// 
    /// \param value 
    /// Input the filter that defines the right operand
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_FILTER::Filter^ RightOperand
    {
        NAMESPACE_OSGEO_FDO_FILTER::Filter^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_FILTER::Filter^ value);
    }

    /// \brief
    /// Overrides Filter.Process to pass BinaryLogicalOperator to the
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
    /// Constructs a BinaryLogicalOperator object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	BinaryLogicalOperator(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoBinaryLogicalOperator* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_FILTER


