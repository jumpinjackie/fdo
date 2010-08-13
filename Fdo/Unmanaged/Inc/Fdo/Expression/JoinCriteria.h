#ifndef _JOINCRITERIA_H_
#define _JOINCRITERIA_H_
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
#include <Fdo/Expression/JoinType.h>
class FdoFilter;
class FdoIdentifier;
class FdoJoinCriteriaCollection;

/// \brief
/// The FdoJoinCriteria class is used to define a join criteria for 
/// a select or sub-select
/// 
class FdoJoinCriteria : public FdoIDisposable
{
    friend class FdoJoinCriteriaCollection;
/// \cond DOXYGEN-IGNORE
protected:
    /// Constructs a default instance of a join criteria.
    FdoJoinCriteria();

    /// Constructs an instance of a join criteria using the specified arguments.
    FdoJoinCriteria(FdoIdentifier* joinClass, FdoJoinType joinType);

    /// Constructs an instance of a join criteria using the specified arguments.
    FdoJoinCriteria(FdoIdentifier* joinClass, FdoJoinType joinType, FdoFilter* filter);

    /// Constructs an instance of a join criteria using the specified arguments.
    FdoJoinCriteria(FdoString* alias, FdoIdentifier* joinClass, FdoJoinType joinType);

    /// Constructs an instance of a join criteria using the specified arguments.
    FdoJoinCriteria(FdoString* alias, FdoIdentifier* joinClass, FdoJoinType joinType, FdoFilter* filter);
    
    virtual void Dispose();

    virtual ~FdoJoinCriteria();

    void EnsureValidCriteria();
public:
    /// Constructs a default instance of a join criteria.
    FDO_API static FdoJoinCriteria* Create();

    /// Constructs a full defined instance of a join criteria.
    /// In this case Alias = joinClass->GetName().
    FDO_API static FdoJoinCriteria* Create(FdoIdentifier* joinClass, FdoJoinType joinType);

    /// Constructs a full defined instance of a join criteria.
    /// In this case Alias = joinClass->GetName().
    FDO_API static FdoJoinCriteria* Create(FdoIdentifier* joinClass, FdoJoinType joinType, FdoFilter* filter);

    /// Constructs a full defined instance of a join criteria.
    FDO_API static FdoJoinCriteria* Create(FdoString* alias, FdoIdentifier* joinClass, FdoJoinType joinType);

    /// Constructs a full defined instance of a join criteria.
    FDO_API static FdoJoinCriteria* Create(FdoString* alias, FdoIdentifier* joinClass, FdoJoinType joinType, FdoFilter* filter);

    /// Returns the alias name for the join criteria class
    FDO_API FdoString* GetAlias();

    /// Sets the alias name for the join criteria class
    FDO_API void SetAlias(FdoString* value);

    /// Returns true if there is an alias set and false if GetAlias() returns the join class
    FDO_API bool HasAlias();

    /// Returns the identifier of the right join class
    FDO_API FdoIdentifier* GetJoinClass();

    /// Sets the identifier of the right join class
    FDO_API void SetJoinClass(FdoIdentifier* value);

    /// Returns the join type, e.g. inner join, cross join, left join
    /// using FdoJoinType_None in join criteria will generate an exception
    FDO_API FdoJoinType GetJoinType() const;

    /// Sets the join type, e.g. inner join, cross join, left join
    FDO_API void SetJoinType(FdoJoinType value);

    /// Returns the join filter, e.g. LEFT JOIN rclass ON(filter)
    FDO_API FdoFilter* GetFilter();

    /// Sets the join filter, e.g. LEFT JOIN rclass ON(filter)
    FDO_API void SetFilter(FdoFilter* value);

private:
    FdoPtr<FdoIdentifier> m_JoinClass;
    FdoPtr<FdoIdentifier> m_JoinAlias;
    FdoJoinType m_JoinType;
    FdoPtr<FdoFilter> m_Filter;
};
#endif
