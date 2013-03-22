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

class FdoSubSelectExpression;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
 ref class Filter;
END_NAMESPACE_OSGEO_FDO_FILTER 

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

ref class Identifier;
ref class JoinCriteriaCollection;
interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The SubSelectExpression class derives from Identifier and represents an expression
/// with alias. The name or alias must be a simple name and should not be scoped or contain the
/// the schema name. The SubSelectExpression can be used to provide an alias to an expression. 
/// 
public ref class SubSelectExpression : public NAMESPACE_OSGEO_FDO_EXPRESSION::Expression
{
public:
    /// \brief
    /// Constructs a default instance of an identifier.
    /// 
    /// \return
    /// Returns the SubSelectExpression
    /// 
	SubSelectExpression();

    /// \brief
    /// Constructs an instance of an identifier using the specified arguments.
    /// 
    /// \param className 
    /// Input the main class name
    /// \param propertyName 
    /// Input  property to select, it can be a computed identifier
    /// 
    /// \return
    /// Returns the SubSelectExpression
    /// 
	SubSelectExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ className, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName);

    /// \brief
    /// Constructs an instance of an identifier using the specified arguments.
    /// 
    /// \param className 
    /// Input the main class name
    /// \param propertyName 
    /// Input  property to select, it can be a computed identifier
    /// \param filter 
    /// Input filter of the sub select
    /// 
    /// \return
    /// Returns the SubSelectExpression
    /// 
	SubSelectExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ className, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::Filter^ filter);

    /// \brief
    /// Constructs an instance of an identifier using the specified arguments.
    /// 
    /// \param className 
    /// Input the main class name
    /// \param propertyName 
    /// Input  property to select, it can be a computed identifier
    /// \param joinCritColl 
    /// Input join criteria collection of the sub select
    /// 
    /// \return
    /// Returns the SubSelectExpression
    /// 
	SubSelectExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ className, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^ joinCritColl);

    /// \brief
    /// Constructs an instance of an identifier using the specified arguments.
    /// 
    /// \param className 
    /// Input the main class name
    /// \param propertyName 
    /// Input  property to select, it can be a computed identifier
    /// \param filter 
    /// Input filter of the sub select
    /// \param joinCritColl 
    /// Input join criteria collection of the sub select
    /// 
    /// \return
    /// Returns the SubSelectExpression
    /// 
	SubSelectExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ className, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::Filter^ filter, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^ joinCritColl);

    /// \brief
    /// Gets the class identifier.
    /// 
    /// \return
    /// Returns the identifier
    /// 
    /// \brief
    /// Sets the class identifier.
    /// 
    /// \param value 
    /// Input class identifier
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ FeatureClassName
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value);
    }

    /// \brief
    /// Gets the property identifier.
    /// 
    /// \return
    /// Returns the identifier
    /// 
    /// \brief
    /// Sets the property identifier.
    /// 
    /// \param value 
    /// Input property identifier
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
    /// Gets the JoinCriteria collection.
    /// 
    /// \return
    /// Returns the JoinCriteria collection
    /// 
    /// \brief
    /// Sets the JoinCriteria collection.
    /// 
    /// \param value 
    /// Input JoinCriteria collection
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^ JoinCriteria
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^ value);
    }

    /// \brief
    /// Gets the join filter.
    /// 
    /// \return
    /// Returns the join filter
    /// 
    /// \brief
    /// Sets the join filter.
    /// 
    /// \param value 
    /// Input join filter
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_FILTER::Filter^ Filter
    {
        NAMESPACE_OSGEO_FDO_FILTER::Filter^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_FILTER::Filter^ value);
    }

    /// \brief
    /// Overrides Expression.Process to pass the SubSelectExpression to the
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
    /// Constructs a SubSelectExpression object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	SubSelectExpression(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoSubSelectExpression* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


