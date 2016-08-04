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

class FdoInCondition;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
ref class Identifier;
ref class SubSelectExpression;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
ref class IdentifierCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
interface class IFilterProcessor;
ref class ValueExpressionCollection;

/// \ingroup (OSGeoFDOFilter)
/// \brief
/// The InCondition class derives from SearchCondition and can be used to test
/// if the value of a specified data property is within a given set of literal
/// values.
public ref class InCondition : public NAMESPACE_OSGEO_FDO_FILTER::SearchCondition
{
public:
    /// \brief
    /// Constructs a default instance of an InCondition.
    /// 
    /// \return
    /// Returns InCondition
    /// 
	InCondition();

    /// \brief
    /// Constructs an instance of an InCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input property name
    /// \param values 
    /// Input array of values
    /// 
    /// \return
    /// Returns InCondition
    /// 
	InCondition(System::String^ propertyName, array<System::String^>^ values);

    /// \brief
    /// Constructs an instance of an InCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input property name
    /// \param values 
    /// Input array of values
    /// 
    /// \return
    /// Returns InCondition
    /// 
	InCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, array<System::String^>^ values);

    /// \brief
    /// Constructs an instance of an InCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input property name
    /// \param collection 
    /// Input collection of values
    /// 
    /// \return
    /// Returns InCondition
    /// 
	InCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection^ collection);

    /// \brief
    /// Constructs an instance of an InCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input property name
    /// \param subSel
    /// Input sub select
    /// 
    /// \return
    /// Returns InCondition
    /// 
	InCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression^ subSel);

    /// \brief
    /// Gets the name of the data property.
    /// 
    /// \return
    /// Returns the name of the data property
    /// 
    /// \brief
    /// Sets the name of the data property.
    /// 
    /// \param value 
    /// Input the name of the data property
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ PropertyName
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value);
    }

    /// \brief
    ///  Gets LiteralCollection that contains the literal values.
    /// 
    /// \return
    /// Returns ValueExpressionCollection
    /// 
    property NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection^ Values
    {
        NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection^ get();
    }

    /// \brief
    /// Gets the sub-select expression.
    /// 
    /// \return
    /// Returns the sub-select expression
    /// 
    /// \brief
    /// Sets the sub-select expression.
    /// 
    /// \param value 
    /// Input sub-select expression
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression^ SubSelect
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression^ value);
    }

    /// \brief
    /// Overrides Filter.Process to pass InCondition to the appropriate
    /// filter processor operation.
    /// 
    /// \param processor 
    /// Input filter processor
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor^ processor);

    /// \brief
    /// Converts the filter expression to its well defined text representation.
    /// 
    /// \return
    /// Returns well defined text string
    /// 
	System::String^ ToStringInternal( NAMESPACE_OSGEO_FDO_COMMANDS::IdentifierCollection ^idCollection );

    /// \brief
    /// Constructs a InCondition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	InCondition(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoInCondition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_FILTER


