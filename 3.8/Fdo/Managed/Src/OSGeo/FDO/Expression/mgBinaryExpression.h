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
ref class Express;
interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The BinaryExpression class derives from Expression and represents string
/// concatenation and the simple arithmetic operations add, subtract, multiply,
/// and divide.
public ref class BinaryExpression : public NAMESPACE_OSGEO_FDO_EXPRESSION::Expression
{
public:
    /// \brief
    /// Constructs a default instance of an BinaryExpression.
    /// 
    /// \return
    /// Returns BinaryExpression
    /// 
	BinaryExpression();

    /// \brief
    /// Constructs an instance of an BinaryExpression using the specified arguments.
    /// 
    /// \param leftExpression 
    /// Input left hand expression
    /// \param operation 
    /// Input binary operation
    /// \param rightExpression 
    /// Input right hand expression
    /// 
    /// \return
    /// Returns BinaryExpression
    /// 
	BinaryExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ leftExpression, NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ rightExpression);

    /// \brief
    /// Gets the binary operation to be evaluated by this expression.
    /// 
    /// \return
    /// Returns the binary operation
    /// 
    /// \brief
    /// Sets the binary operation to be evaluated by this expression.
    /// 
    /// \param value 
    /// Input binary operation value
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryOperations Operation
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryOperations get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryOperations value);
    }

    /// \brief
    /// Gets the expression on the left side of the binary expression.
    /// 
    /// \return
    /// Returns the left hand side expression
    /// 
    /// \brief
    /// Sets the expression on the left side of the binary expression.
    /// 
    /// \param value 
    /// Input left side expression
    /// 
    /// \return
    /// Returns nothing
    /// 
	property NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ LeftExpression
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ value);
    }

    /// \brief
    /// Gets the expression on the right side of the binary expression.
    /// 
    /// \return
    /// Returns the right hand side expression
    /// 
    /// \brief
    /// Sets the expression on the right side of the binary expression.
    /// 
    /// \param value 
    /// Input right side expression
    /// 
    /// \return
    /// Returns nothing
    /// 
	property NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ RightExpression
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ value);
    }

    /// \brief
    /// Overrides Expression.Process to pass the BinaryOperation to the appropriate expression processor operation.
    /// 
    /// \param processor 
    /// Input the IExpressionProcessor
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
    /// Constructs a BinaryExpression object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	BinaryExpression(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoBinaryExpression* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


