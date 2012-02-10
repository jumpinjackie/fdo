// 
//  
//  Copyright (C) 2004-2011  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include <wctype.h>
#include <Fdo/Expression/JoinCriteria.h>
#include <Fdo/Expression/Identifier.h>
#include <Fdo/Filter/Filter.h>
#include <Fdo/Expression/ExpressionException.h>
#include "StringUtility.h"

// Constructs a default instance of an Identifier.
FdoJoinCriteria* FdoJoinCriteria::Create()
{
    return new FdoJoinCriteria();
}

// Constructs an instance of an join criteria using the specified arguments.
FdoJoinCriteria* FdoJoinCriteria::Create(FdoIdentifier* joinClass, FdoJoinType joinType)
{
    return new FdoJoinCriteria(joinClass, joinType);
}

// Constructs an instance of an join criteria using the specified arguments.
FdoJoinCriteria* FdoJoinCriteria::Create(FdoIdentifier* joinClass, FdoJoinType joinType, FdoFilter* filter)
{
    return new FdoJoinCriteria(joinClass, joinType, filter);
}

/// Constructs a full defined instance of a join criteria.
FdoJoinCriteria* FdoJoinCriteria::Create(FdoString* alias, FdoIdentifier* joinClass, FdoJoinType joinType)
{
    return new FdoJoinCriteria(alias, joinClass, joinType);
}

/// Constructs a full defined instance of a join criteria.
FdoJoinCriteria* FdoJoinCriteria::Create(FdoString* alias, FdoIdentifier* joinClass, FdoJoinType joinType, FdoFilter* filter)
{
    return new FdoJoinCriteria(alias, joinClass, joinType, filter);
}

FdoJoinCriteria::~FdoJoinCriteria()
{
}

void FdoJoinCriteria::Dispose()
{
    delete this;
}

// Constructs a default instance of a join criteria.
FdoJoinCriteria::FdoJoinCriteria():
m_JoinType(FdoJoinType_None)
{
}

// Constructs an instance of a join criteria using the specified arguments.
FdoJoinCriteria::FdoJoinCriteria(FdoIdentifier* joinClass, FdoJoinType joinType)
{
    m_JoinClass = FDO_SAFE_ADDREF(joinClass);
    m_JoinType = joinType;
}

// Constructs an instance of a join criteria using the specified arguments.
FdoJoinCriteria::FdoJoinCriteria(FdoIdentifier* joinClass, FdoJoinType joinType, FdoFilter* filter)
{
    m_JoinClass = FDO_SAFE_ADDREF(joinClass);
    m_JoinType = joinType;
    m_Filter = FDO_SAFE_ADDREF(filter);
}

// Constructs an instance of a join criteria using the specified arguments.
FdoJoinCriteria::FdoJoinCriteria(FdoString* alias, FdoIdentifier* joinClass, FdoJoinType joinType)
{
    m_JoinClass = FDO_SAFE_ADDREF(joinClass);
    m_JoinType = joinType;
    if (alias != NULL)
        m_JoinAlias = FdoIdentifier::Create(alias);
}

// Constructs an instance of a join criteria using the specified arguments.
FdoJoinCriteria::FdoJoinCriteria(FdoString* alias, FdoIdentifier* joinClass, FdoJoinType joinType, FdoFilter* filter)
{
    m_JoinClass = FDO_SAFE_ADDREF(joinClass);
    m_JoinType = joinType;
    m_Filter = FDO_SAFE_ADDREF(filter);
    if (alias != NULL)
        m_JoinAlias = FdoIdentifier::Create(alias);
}

FdoString* FdoJoinCriteria::GetAlias()
{
    return (m_JoinAlias != NULL) ? m_JoinAlias->GetName() : ((m_JoinClass != NULL) ? m_JoinClass->GetName() : NULL);
}

void FdoJoinCriteria::SetAlias(FdoString* value)
{
    if (value != NULL)
        m_JoinAlias = FdoIdentifier::Create(value);
    else
        m_JoinAlias = NULL;
}

bool FdoJoinCriteria::HasAlias()
{
    FdoString* alias = (m_JoinAlias != NULL) ? m_JoinAlias->GetName() : NULL;
    return (alias != NULL && *alias != 0);
}

FdoIdentifier* FdoJoinCriteria::GetJoinClass()
{
	return FDO_SAFE_ADDREF( m_JoinClass.p );
}

void FdoJoinCriteria::SetJoinClass(FdoIdentifier* value)
{
    m_JoinClass = FDO_SAFE_ADDREF( value );
}

FdoJoinType FdoJoinCriteria::GetJoinType() const
{
    return m_JoinType;
}

void FdoJoinCriteria::SetJoinType(FdoJoinType value)
{
    m_JoinType = value;
}

FdoFilter* FdoJoinCriteria::GetFilter()
{
	return FDO_SAFE_ADDREF( m_Filter.p );
}

void FdoJoinCriteria::SetFilter(FdoFilter* value)
{
    m_Filter = FDO_SAFE_ADDREF( value );
}

void FdoJoinCriteria::EnsureValidCriteria()
{
    if (m_JoinType == FdoJoinType_None)
    {
        FdoString* alias = GetAlias();
        throw FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_JOIN_INCOMPLETE_DEFINITION), (alias!=NULL)?alias:L""));
    }

    if (m_JoinType == FdoJoinType_Cross && m_Filter != NULL)
    {
        FdoString* alias = GetAlias();
        throw FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_JOIN_CANNOTHAVE_FILTER), (alias!=NULL)?alias:L""));
    }

    if (m_JoinType != FdoJoinType_Cross && m_Filter == NULL)
    {
        FdoString* alias = GetAlias();
        throw FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_JOIN_MUSTHAVE_FILTER), (alias!=NULL)?alias:L""));
    }
}
