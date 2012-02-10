/*
* Copyright (C) 2004-2011  Autodesk, Inc.
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

#include <Fdo\Expression\mgJoinType.h>

class FdoJoinCriteria;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
 ref class Filter;
END_NAMESPACE_OSGEO_FDO_FILTER 

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

ref class Expression;
interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The JoinCriteria class is used to define a join criteria used in select statements
/// 
public ref class JoinCriteria : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs a default instance of a join criteria.
    /// 
    /// \return
    /// Returns the JoinCriteria
    /// 
	JoinCriteria();

    /// \brief
    /// Constructs an instance of a join criteria using the specified arguments.
    /// 
    /// \param joinClass 
    /// Input the identifier join name
    /// \param joinType 
    /// Input  join type
    /// 
    /// \return
    /// Returns the JoinCriteria
    /// 
	JoinCriteria(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ joinClass, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType joinType);

    /// \brief
    /// Constructs an instance of a join criteria using the specified arguments.
    /// 
    /// \param joinClass 
    /// Input the identifier join name
    /// \param joinType 
    /// Input  join type
    /// \param filter 
    /// Input filter
    /// 
    /// \return
    /// Returns the JoinCriteria
    /// 
	JoinCriteria(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ joinClass, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType joinType, NAMESPACE_OSGEO_FDO_FILTER::Filter^ filter);

    /// \brief
    /// Constructs an instance of a join criteria using the specified arguments.
    /// 
    /// \param joinClass 
    /// Input the identifier join name
    /// \param joinType 
    /// Input  join type
    /// 
    /// \return
    /// Returns the JoinCriteria
    /// 
	JoinCriteria(System::String^ alias, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ joinClass, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType joinType);

    /// \brief
    /// Constructs an instance of a join criteria using the specified arguments.
    /// 
    /// \param joinClass 
    /// Input the identifier join name
    /// \param joinType 
    /// Input  join type
    /// \param filter 
    /// Input filter
    /// 
    /// \return
    /// Returns the JoinCriteria
    /// 
	JoinCriteria(System::String^ alias, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ joinClass, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType joinType, NAMESPACE_OSGEO_FDO_FILTER::Filter^ filter);

    /// \brief
    /// Gets the alias of join class.
    /// 
    /// \return
    /// Returns the alias of join class
    /// 
    /// \brief
    /// Sets the alias of join class.
    /// 
    /// \param value 
    /// Input alias of join class
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::String^ Alias
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets if join criteria has alias.
    /// 
    /// \return
    /// Returns true if join criteria has an alias
    /// 
    property System::Boolean HasAlias
    {
        System::Boolean get();
    }

    /// \brief
    /// Gets the join class identifier.
    /// 
    /// \return
    /// Returns the identifier
    /// 
    /// \brief
    /// Sets the join class identifier.
    /// 
    /// \param value 
    /// Input join class identifier
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ JoinClass
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value);
    }

    /// \brief
    /// Gets the join type.
    /// 
    /// \return
    /// Returns the join type
    /// 
    /// \brief
    /// Sets the join type.
    /// 
    /// \param value 
    /// Input join type
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType JoinType
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType value);
    }

    /// \brief
    /// Gets the join filter. The filter is used to define a join-condition. 
    /// For cross joins the filter is required to be null. For all other join types, a filter is required.
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
    /// Constructs a JoinCriteria object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	JoinCriteria(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoJoinCriteria* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


