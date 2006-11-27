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

#include "FDO\Expression\mgExpression.h"
#include "FDO\Expression\mgBinaryOperations.h"

class FdoBinaryExpression;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
public __gc class Express;
public __gc __interface IExpressionProcessor;

/// \brief
/// The FdoBinaryExpression class derives from FdoExpression and represents string
/// concatenation and the simple arithmetic operations add, subtract, multiply,
/// and divide.
public __gc class BinaryExpression : public NAMESPACE_OSGEO_FDO_EXPRESSION::Expression
{
public:
    /// \brief
    /// Constructs a default instance of an FdoBinaryExpression.
    /// 
    /// \return
    /// Returns FdoBinaryExpression
    /// 
	BinaryExpression();

    /// \brief
    /// Constructs an instance of an FdoBinaryExpression using the specified arguments.
    /// 
    /// \param leftExpression 
    /// Input left hand expression
    /// \param operation 
    /// Input binary operation
    /// \param rightExpression 
    /// Input right hand expression
    /// 
    /// \return
    /// Returns FdoBinaryExpression
    /// 
	BinaryExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* leftExpression, NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* rightExpression);

    /// \brief
    /// Gets the binary operation to be evaluated by this expression.
    /// 
    /// \return
    /// Returns the binary operation
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryOperations get_Operation();

    /// \brief
    /// Sets the binary operation to be evaluated by this expression.
    /// 
    /// \param value 
    /// Input binary operation value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Operation(NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryOperations value);

    /// \brief
    /// Gets the expression on the left side of the binary expression.
    /// 
    /// \return
    /// Returns the left hand side expression
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* get_LeftExpression();

    /// \brief
    /// Sets the expression on the left side of the binary expression.
    /// 
    /// \param value 
    /// Input left side expression
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LeftExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* value);

    /// \brief
    /// Gets the expression on the right side of the binary expression.
    /// 
    /// \return
    /// Returns the right hand side expression
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* get_RightExpression();

    /// \brief
    /// Sets the expression on the right side of the binary expression.
    /// 
    /// \param value 
    /// Input right side expression
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_RightExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* value);

    /// \brief
    /// Overrides FdoExpression.Process to pass the FdoBinaryOperation to the appropriate expression processor operation.
    /// 
    /// \param processor 
    /// Input the FdoIExpressionProcessor
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns the well defined text representation of this expression
    /// 
	System::String* ToString();

	BinaryExpression(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoBinaryExpression* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


