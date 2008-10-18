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

class FdoFunction;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc class ExpressionCollection;
public __gc __interface IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The Function class derives from Expression and represents a function call. As
/// such it defines the name of the function and the collection of arguments to
/// be passed to it.
public __gc class Function : public NAMESPACE_OSGEO_FDO_EXPRESSION::Expression
{
public:
    /// \brief
    /// Constructs a default instance of an Function.
    /// 
    /// \return
    /// Returns Function class
    /// 
	Function();

    /// \brief
    /// Constructs an instance of a function using the specified arguments.
    /// 
    /// \param name 
    /// Input name of the function
    /// \param arguments 
    /// Input pointer to arguments
    /// 
    /// \return
    /// Returns Function
    /// 
	Function(System::String* name, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* arguments []);

    /// \brief
    /// Constructs an instance of a function using the specified arguments.
    /// 
    /// \param name 
    /// Input name of the function
    /// \param arguments 
    /// Input arguments as collection
    /// 
    /// \return
    /// Returns Function
    /// 
	Function(System::String* name, NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionCollection* arguments);

    /// \brief
    /// Gets the name of the function.
    /// 
    /// \return
    /// Returns name of the function
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Sets the name of the function.
    /// 
    /// \param value 
    /// Input name of the function
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Name(System::String* value);

    /// \brief
    /// Gets the ExpressionCollection that contains the arguments to the
    /// function.
    /// 
    /// \return
    /// Returns arguments as collection
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionCollection* get_Arguments();

    /// \brief
    /// Overrides Expression.Process to pass the Function to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor 
    /// Input expression processor
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

    /// \brief
    /// Constructs a Function object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	Function(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoFunction* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


