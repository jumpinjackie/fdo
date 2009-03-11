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
#include "FDO\Expression\mgUnaryOperations.h"

class FdoUnaryExpression;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
ref class Expression;
interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The UnaryExpression class derives from Expression and allows negation of
/// arithmetic expressions.
public ref class UnaryExpression : public NAMESPACE_OSGEO_FDO_EXPRESSION::Expression
{
public:
    /// \brief
    /// Constructs a default instance of an UnaryExpression.
    /// 
    /// \return
    /// Returns UnaryExpression
    /// 
	UnaryExpression();

    /// \brief
    /// Constructs an instance of an UnaryExpression using the specified arguments.
    /// 
    /// \param operation 
    /// Input the type of unary operation
    /// \param expression 
    /// Input the expression
    /// 
    /// \return
    /// Returns UnaryExpression
    /// 
	UnaryExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ expression);

    /// \brief
    ///  Gets the unary operation to be evaluated by this expression.
    /// 
    /// \return
    /// Returns the unary operation
    /// 
    /// \brief
    /// Sets the unary operation to be evaluated by this expression.
    /// 
    /// \param value 
    /// Input the type of unary operation
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryOperations Operation
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryOperations get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryOperations value);
    }

    /// \brief
    /// Gets the expression to be operated on.
    /// 
    /// \return
    /// Returns the expression
    /// 
    /// \brief
    /// Sets the expression to be operated on.
    /// 
    /// \param value 
    /// Input the value expression
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ Expressions
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ value);
    }

    /// \brief
    /// Overrides Expression.Process to pass the UnaryExpression to the
    /// appropriate expression processor operation.
    /// 
    /// \param processor 
    /// Input expression processor interface
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor) override;

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns the well defined text representation of this expression
    /// 
	virtual System::String^ ToString() override;

    /// \brief
    /// Constructs a UnaryExpression object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	UnaryExpression(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoUnaryExpression* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


