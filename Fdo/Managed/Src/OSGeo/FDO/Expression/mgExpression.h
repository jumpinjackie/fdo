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

class FdoExpression;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// Expression is an abstract base class that forms the root of an expression
/// tree. The Expression class contains operations for converting between the
/// well defined text representation and the object representation of an
/// expression.
public __gc class Expression : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Static operation that parses the specified text and returns an
    /// expression. An exception is thrown if the text does not conform to the
    /// well defined text representation of an expression.
    /// 
    /// \param expressionText 
    /// Input expression in well defined text
    /// 
    /// \return
    /// Returns parse tree root node
    /// 
	static NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* Parse(System::String* expressionText);

    /// \brief
    /// Abstract operation that takes an IExpressionProcessor as an argument.
    /// Concrete expression subclasses must override this method and pass
    /// themselves as an argument to the appropriate expression processor
    /// operation.
    /// 
    /// \param processor 
    /// Input expression processor
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

    /// \brief
    /// Abstract operation that returns the well defined text representation of
    /// this expression.
    /// 
    /// \return
    /// Returns the well defined text string
    /// 
	System::String* ToString();

    /// \brief
    /// Constructs a Expression object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	Expression(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
	inline FdoExpression* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


