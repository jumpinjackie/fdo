// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
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
#include <Fdo/Expression/SubSelectExpression.h>
#include "StringUtility.h"
#include <Fdo/Expression/IExpressionProcessor.h>

/// Constructs a default instance of an FdoSubSelectExpression.
FdoSubSelectExpression::FdoSubSelectExpression()
{
}

/// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
FdoSubSelectExpression::FdoSubSelectExpression(FdoIdentifier* className, FdoIdentifier* propertyName)
{
    m_className = FDO_SAFE_ADDREF(className);
    m_propertyName = FDO_SAFE_ADDREF(propertyName);
}

/// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
FdoSubSelectExpression::FdoSubSelectExpression(FdoIdentifier* className, FdoIdentifier* propertyName, FdoFilter* filter)
{
    m_className = FDO_SAFE_ADDREF(className);
    m_propertyName = FDO_SAFE_ADDREF(propertyName);
    m_filter = FDO_SAFE_ADDREF(filter);
}

/// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
FdoSubSelectExpression::FdoSubSelectExpression(FdoIdentifier* className, FdoIdentifier* propertyName, FdoJoinCriteriaCollection* joinCritColl)
{
    m_className = FDO_SAFE_ADDREF(className);
    m_propertyName = FDO_SAFE_ADDREF(propertyName);
    m_joinCritColl = FDO_SAFE_ADDREF(joinCritColl);
}

/// Constructs an instance of an FdoSubSelectExpression using the specified arguments.
FdoSubSelectExpression::FdoSubSelectExpression(FdoIdentifier* className, FdoIdentifier* propertyName, FdoFilter* filter, FdoJoinCriteriaCollection* joinCritColl)
{
    m_className = FDO_SAFE_ADDREF(className);
    m_propertyName = FDO_SAFE_ADDREF(propertyName);
    m_filter = FDO_SAFE_ADDREF(filter);
    m_joinCritColl = FDO_SAFE_ADDREF(joinCritColl);
}

FdoSubSelectExpression* FdoSubSelectExpression::Create()
{
    return new FdoSubSelectExpression();
}

FdoSubSelectExpression* FdoSubSelectExpression::Create(FdoIdentifier* className, FdoIdentifier* propertyName)
{
    return new FdoSubSelectExpression(className, propertyName);
}

FdoSubSelectExpression* FdoSubSelectExpression::Create(FdoIdentifier* className, FdoIdentifier* propertyName, FdoFilter* filter)
{
    return new FdoSubSelectExpression(className, propertyName, filter);
}

FdoSubSelectExpression* FdoSubSelectExpression::Create(FdoIdentifier* className, FdoIdentifier* propertyName, FdoJoinCriteriaCollection* joinCritColl)
{
    return new FdoSubSelectExpression(className, propertyName, joinCritColl);
}

FdoSubSelectExpression* FdoSubSelectExpression::Create(FdoIdentifier* className, FdoIdentifier* propertyName, FdoFilter* filter, FdoJoinCriteriaCollection* joinCritColl)
{
    return new FdoSubSelectExpression(className, propertyName, filter, joinCritColl);
}

FdoSubSelectExpression::~FdoSubSelectExpression()
{
}

void FdoSubSelectExpression::Dispose()
{
    delete this;
}

FdoIdentifier* FdoSubSelectExpression::GetClassName()
{
	return FDO_SAFE_ADDREF( m_className.p );
}

void FdoSubSelectExpression::SetClassName(FdoIdentifier* value)
{
    m_className = FDO_SAFE_ADDREF( value );
}

FdoIdentifier* FdoSubSelectExpression::GetPropertyName()
{
	return FDO_SAFE_ADDREF( m_propertyName.p );
}

void FdoSubSelectExpression::SetPropertyName(FdoIdentifier* value)
{
    m_propertyName = FDO_SAFE_ADDREF( value );
}

FdoJoinCriteriaCollection* FdoSubSelectExpression::GetJoinCriteria()
{
	return FDO_SAFE_ADDREF( m_joinCritColl.p );
}

void FdoSubSelectExpression::SetJoinCriteria(FdoJoinCriteriaCollection* value)
{
    m_joinCritColl = FDO_SAFE_ADDREF( value );
}

FdoFilter* FdoSubSelectExpression::GetFilter()
{
    return FDO_SAFE_ADDREF( m_filter.p );
}

void FdoSubSelectExpression::SetFilter(FdoFilter* value)
{
    m_filter = FDO_SAFE_ADDREF( value );
}

void FdoSubSelectExpression::Process(FdoIExpressionProcessor* p)
{
    p->ProcessSubSelectExpression(*this);
}

FdoString* FdoSubSelectExpression::ToString()
{
    FdoStringUtility::ClearString(m_toString);
    if (m_className == NULL || m_propertyName == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_4_INCOMPLETEINCONDITION)));

    std::wstring tostr(L"SELECT(");
    tostr.append(m_className->ToString());
    tostr.append(L",");
    tostr.append(m_propertyName->ToString());
    if (m_filter != NULL)
    {
        tostr.append(L",");
        tostr.append(m_filter->ToString());
    }
    if (m_joinCritColl != NULL)
    {
        for(int i = 0; i < m_joinCritColl->GetCount(); i++)
        {
            FdoPtr<FdoJoinCriteria> jc = m_joinCritColl->GetItem(i);
            tostr.append(L",JOIN(");
            FdoPtr<FdoIdentifier> clsJoin = jc->GetJoinClass();
            tostr.append(clsJoin->ToString());
            if (jc->HasAlias())
            {
                tostr.append(L" AS ");
                tostr.append(jc->GetAlias());
            }
            switch(jc->GetJoinType())
            {
            case FdoJoinType_None:
                tostr.append(L",JOINNONE");
                break;
            case FdoJoinType_Inner:
                tostr.append(L",JOININNER");
                break;
            case FdoJoinType_RightOuter:
                tostr.append(L",JOINRIGHTOUTER");
                break;
            case FdoJoinType_LeftOuter:
                tostr.append(L",JOINLEFTOUTER");
                break;
            case FdoJoinType_FullOuter:
                tostr.append(L",JOINFULLOUTER");
                break;
            case FdoJoinType_Cross:
                tostr.append(L",JOINCROSS");
                break;
            }
            FdoPtr<FdoFilter> joinFilter = jc->GetFilter();
            if (joinFilter != NULL)
            {
                tostr.append(L",");
                tostr.append(joinFilter->ToString());
            }
            tostr.append(L")");
        }
    }
    tostr.append(L")");

    m_toString = FdoStringUtility::MakeString(tostr.c_str());
    return m_toString;
}

FdoString* FdoSubSelectExpression::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    return ToString();
}