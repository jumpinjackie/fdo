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
public __gc class Identifier;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
public __gc class IdentifierCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
public __gc __interface IFilterProcessor;
public __gc class ValueExpressionCollection;

/// \ingroup (OSGeoFDOFilter)
/// \brief
/// The InCondition class derives from SearchCondition and can be used to test
/// if the value of a specified data property is within a given set of literal
/// values.
public __gc class InCondition : public NAMESPACE_OSGEO_FDO_FILTER::SearchCondition
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
	InCondition(System::String* propertyName, System::String* values []);

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
	InCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* propertyName, System::String* values []);

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
	InCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* propertyName, NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection* collection);

    /// \brief
    /// Gets the name of the data property.
    /// 
    /// \return
    /// Returns the name of the data property
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* get_PropertyName();

    /// \brief
    /// Sets the name of the data property.
    /// 
    /// \param value 
    /// Input the name of the data property
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_PropertyName(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* value);

    /// \brief
    ///  Gets LiteralCollection that contains the literal values.
    /// 
    /// \return
    /// Returns ValueExpressionCollection
    /// 
	__property NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection* get_Values();

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
	System::Void Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor* processor);

    /// \brief
    /// Converts the filter expression to its well defined text representation.
    /// 
    /// \return
    /// Returns well defined text string
    /// 
	System::String* ToStringInternal( NAMESPACE_OSGEO_FDO_COMMANDS::IdentifierCollection *idCollection );

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

public private:
	inline FdoInCondition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_FILTER


