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

#include "FDO\Expression\mgIdentifier.h"

class FdoComputedIdentifier;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc class Expression;
public __gc __interface IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The ComputedIdentifier class derives from Identifier and represents an expression
/// with alias. The name or alias must be a simple name and should not be scoped or contain the
/// the schema name. The ComputedIdentifier can be used to provide an alias to an expression. 
/// 
public __gc class ComputedIdentifier : public NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier
{
public:
    /// \brief
    /// Constructs a default instance of an identifier.
    /// 
    /// \return
    /// Returns the ComputedIdentifier
    /// 
	ComputedIdentifier();

    /// \brief
    /// Constructs an instance of an identifier using the specified arguments.
    /// 
    /// \param name 
    /// Input the computed identifier name
    /// \param expression 
    /// Input  expression
    /// 
    /// \return
    /// Returns the ComputedIdentifier
    /// 
	ComputedIdentifier(System::String* name, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* expression);

    /// \brief
    /// Gets the full text of the identifier.
    /// 
    /// \return
    /// Returns the identifier text
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* get_Expression();

    /// \brief
    /// Sets the full text of the identifier.
    /// 
    /// \param value 
    /// Input identifier text
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void set_Expression(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* value);

    /// \brief
    /// Overrides Expression.Process to pass the ComputedIdentifier to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor 
    /// Input expression processor interface
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

    /// \brief
    /// Constructs a ComputedIdentifier object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	ComputedIdentifier(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoComputedIdentifier* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


