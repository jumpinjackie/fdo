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

#include "FDO\Expression\mgValueExpression.h"

class FdoParameter;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The FdoParameter class derives from FdoValueExpression and represents a
/// substitution parameter in the expression. FdoParameters are represented by a
/// name: when expressions are evaluated, the caller must supply a value to be
/// bound to each parameter name.
/// \remarks
/// For information on how to set the value of a parameter, see the 
/// FdoICommand interface and FdoParameterValue.
public __gc class Parameter : public NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression
{
public:

    /// \brief
    /// Constructs a default instance of a parameter.
    /// 
    /// \return
    /// Returns FdoParameter
    /// 
	Parameter();

    /// \brief
    /// Constructs an instance of a parameter using the specified arguments.
    /// 
    /// \param name 
    /// Input parameter name
    /// 
    /// \return
    /// Returns FdoParameter
    /// 
	Parameter(System::String* name);

    /// \brief
    /// Gets the name of the parameter.
    /// 
    /// \return
    /// Returns the parameter name
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Sets the name of the parameter.
    /// 
    /// \param value 
    /// Input parameter name
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Name(System::String* value);

    /// \brief
    /// Overrides FdoExpression.Process to pass the FdoParameter to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor 
    /// Input an FdoIExpressionProcessor
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns the well defined text string
    /// 
	System::String* ToString();

	Parameter(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoParameter* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


