#ifndef _SUBSELECT_H_
#define _SUBSELECT_H_

// 
//
// Copyright (C) 2004-2011  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>
#include <Fdo/Expression/Expression.h>
#include <Fdo/Expression/Identifier.h>
#include <Fdo/Filter/Filter.h>
#include <Fdo/Expression/JoinCriteriaCollection.h>

/// \brief
/// The FdoSubSelectExpression class derives from FdoExpression and can be used with IN condition
/// to test if a property is in a range of values returned by this sub-select
class FdoSubSelectExpression : public FdoExpression
{
/// \cond DOXYGEN-IGNORE
protected:
    /// Constructs a default instance of an FdoSubSelectExpression.
    FdoSubSelectExpression();

    /// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
    FdoSubSelectExpression(FdoIdentifier* className, FdoIdentifier* propertyName);

    /// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
    FdoSubSelectExpression(FdoIdentifier* className, FdoIdentifier* propertyName, FdoFilter* filter);
    
    /// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
    FdoSubSelectExpression(FdoIdentifier* className, FdoIdentifier* propertyName, FdoJoinCriteriaCollection* joinCritColl);
    
    /// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
    FdoSubSelectExpression(FdoIdentifier* className, FdoIdentifier* propertyName, FdoFilter* filter, FdoJoinCriteriaCollection* joinCritColl);

    virtual ~FdoSubSelectExpression();

    virtual void Dispose();
/// \endcond

public:
    /// \brief
    /// Constructs a default instance of an FdoSubSelectExpression.
    /// 
    /// \return
    /// Returns FdoSubSelectExpression
    /// 
    FDO_API static FdoSubSelectExpression* Create();

    /// \brief
    /// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
    /// 
    /// \param className 
    /// Input class name to select from as identifier 
    /// \param propertyName 
    /// Input property name as identifier to be selected
    /// 
    /// \return
    /// Returns FdoSubSelectExpression
    /// 
    FDO_API static FdoSubSelectExpression* Create(FdoIdentifier* className, FdoIdentifier* propertyName);

    /// \brief
    /// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
    /// 
    /// \param className 
    /// Input class name to select from as identifier 
    /// \param propertyName 
    /// Input property name as identifier to be selected
    /// \param filter
    /// Input filter as a Filter tree for sub-select.
    /// 
    /// \return
    /// Returns FdoSubSelectExpression
    /// 
    FDO_API static FdoSubSelectExpression* Create(FdoIdentifier* className, FdoIdentifier* propertyName, FdoFilter* filter);

    /// \brief
    /// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
    /// 
    /// \param className 
    /// Input class name to select from as identifier 
    /// \param propertyName 
    /// Input property name as identifier to be selected
    /// \param joinCritColl
    /// Input join criteria collection for sub-selects having joins.
    /// 
    /// \return
    /// Returns FdoSubSelectExpression
    /// 
    FDO_API static FdoSubSelectExpression* Create(FdoIdentifier* className, FdoIdentifier* propertyName, FdoJoinCriteriaCollection* joinCritColl);

    /// \brief
    /// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
    /// 
    /// \param className 
    /// Input class name to select from as identifier 
    /// \param propertyName 
    /// Input property name as identifier to be selected
    /// \param filter
    /// Input filter as a Filter tree for sub-select.
    /// \param joinCritColl
    /// Input join criteria collection for sub-selects having joins.
    /// 
    /// \return
    /// Returns FdoSubSelectExpression
    /// 
    FDO_API static FdoSubSelectExpression* Create(FdoIdentifier* className, FdoIdentifier* propertyName, FdoFilter* filter, FdoJoinCriteriaCollection* joinCritColl);

    /// \brief
    /// Gets the name of the feature class to be operated upon as an identifier.
    /// 
    /// \return
    /// Returns the name of the class as identifier
    /// 
    FDO_API FdoIdentifier* GetClassName();

    /// \brief
    /// Sets the name of the main join class to be operated upon as an identifier.
    /// 
    /// \param value 
    /// Input the name of the join class
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetClassName(FdoIdentifier* value);

    /// \brief
    /// Gets the name of the property to be selected by sub-select. 
    /// 
    /// \return
    /// Returns the name of the property
    /// 
    FDO_API FdoIdentifier* GetPropertyName();

    /// \brief
    /// Sets the name of the property to be selected by sub-select. 
    /// It can be a computed identifier (alias or expression)
    /// 
    /// \param value 
    /// Input the name of the property
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetPropertyName(FdoIdentifier* value);

    /// \brief
    /// Gets join criteria collection, it can be NULL 
    /// 
    /// \return
    /// Returns FdoJoinCriteriaCollection
    /// 
    FDO_API FdoJoinCriteriaCollection* GetJoinCriteria();

    /// \brief
    /// Sets the join criteria collection for sub-selects having joins.
    /// 
    /// \param value 
    /// Input FdoJoinCriteriaCollection.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetJoinCriteria(FdoJoinCriteriaCollection* value);

    /// \brief
    /// Gets the filter as a Filter tree for sub-select.
    /// 
    /// \return
    /// Returns FdoFilter
    /// 
    FDO_API FdoFilter* GetFilter();

    /// \brief
    /// Sets the filter as a Filter tree for sub-select.
    /// 
    /// \param value 
    /// Input FdoFilter.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetFilter(FdoFilter* value);

    /// \brief
    /// Overrides FdoExpression.Process to pass the FdoSubSelectExpression to the appropriate
    /// filter processor operation.
    /// 
    /// \param p 
    /// Input expression processor
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void Process(FdoIExpressionProcessor* p);
    
    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns the well defined text string
    /// 
    FDO_API FdoString* ToString();

    /// \brief
    /// Returns the type of the expression as sub-select
    /// 
    /// \return
    /// Returns FdoExpressionItemType (FdoExpressionItemType_SubSelectExpression)
    /// 
    FDO_API FdoExpressionItemType GetExpressionType() { return FdoExpressionItemType_SubSelectExpression; }

    /// \brief
    /// Converts the filter expression to its well defined text representation.
    /// 
    /// \return
    /// Returns well defined text string
    /// 
    FdoString* ToStringInternal( FdoIdentifierCollection *pIdCol );

private:
    FdoPtr<FdoIdentifier> m_className;
    FdoPtr<FdoIdentifier> m_propertyName;
    FdoPtr<FdoFilter> m_filter;
    FdoPtr<FdoJoinCriteriaCollection> m_joinCritColl;
};
#endif


