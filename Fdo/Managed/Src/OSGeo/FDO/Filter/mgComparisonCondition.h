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
#include "FDO\Filter\mgComparisonOperations.h"

class FdoComparisonCondition;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
public __gc class Expression;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

using namespace NAMESPACE_OSGEO_FDO_EXPRESSION;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
public __gc __interface IFilterProcessor;

/// \brief
/// The ComparisonCondition class derives SearchCondition and can be used
/// to test the equality of two expressions.
public __gc class ComparisonCondition : public NAMESPACE_OSGEO_FDO_FILTER::SearchCondition
{
public:

    /// \brief
    /// Constructs a default instance of an ComparisonCondition.
    /// 
    /// \return
    /// Returns ComparisonCondition
    /// 
	ComparisonCondition();

    /// \brief
    /// Constructs an instance of a ComparisonCondition using the specified arguments.
    /// 
    /// \param leftExpression 
    /// Input the expression that defines the left side of the comparison
    /// \param operation 
    /// Input the operation to be used to compare the two expressions
    /// \param rightExpression 
    /// Input the expression that defines the right side of the comparison
    /// 
    /// \return
    /// Returns ComparisonCondition
    /// 
	ComparisonCondition( NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* leftExpression, NAMESPACE_OSGEO_FDO_FILTER::ComparisonOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* rightExpression);

    /// \brief
    /// Gets the operation to be used to compare the two expressions.
    /// 
    /// \return
    /// Returns the operation to be used to compare the two expressions
    /// 
	__property NAMESPACE_OSGEO_FDO_FILTER::ComparisonOperations get_Operation();

    /// \brief
    /// Sets the operation to be used to compare the two expressions.
    /// 
    /// \param operation 
    /// Input the operation to be used to compare the two expressions
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Operation(NAMESPACE_OSGEO_FDO_FILTER::ComparisonOperations operation);

    /// \brief
    /// Gets the expression that defines the left side of the comparison.
    /// 
    /// \return
    /// Returns the expression that defines the left side of the comparison
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* get_LeftExpression();

    /// \brief
    /// Sets the expression that defines the left side of the comparison.
    /// 
    /// \param value 
    /// Input the expression that defines the left side of the comparison
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LeftExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* value);

    /// \brief
    /// Gets the expression that defines the right side of the comparison.
    /// 
    /// \return
    /// Returns the expression that defines the right side of the comparison
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* get_RightExpression();

    /// \brief
    /// Sets the expression that defines the right side of the comparison.
    /// 
    /// \param value 
    /// Input the expression that defines the right side of the comparison
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_RightExpression( NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* value);

    /// \brief
    /// Overrides Filter.Process to pass ComparisonCondition to the
    /// appropriate filter processor operation.
    /// 
    /// \param processor 
    /// Input filter processor
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor* processor);

    /// \brief
    /// Constructs a ComparisonCondition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	ComparisonCondition(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoComparisonCondition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_FILTER


