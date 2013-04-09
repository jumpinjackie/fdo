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
#include <stdlib.h>
#include <Fdo/Filter/Filter.h>
#include <Fdo/Filter/FilterException.h>
#include <Fdo/Filter/NullCondition.h>
#include <Fdo/Filter/BinaryLogicalOperator.h>
#include "StringUtility.h"
#include <Fdo/Expression/ComputedIdentifier.h>

FdoFilter::FdoFilter()
{
    m_toString = NULL;
}

FdoFilter::~FdoFilter()
{
    FdoStringUtility::ClearString(m_toString);
}

/* in Parse\Parse.cpp
FdoFilter* FdoFilter::Parse(FdoString* filterText)
{
	// Make a null condition for now
	return FdoNullCondition::Create( filterText );;
}
*/

// Static operation that combines two filters using the specified binary
// logical operation.
FdoFilter* FdoFilter::Combine(FdoFilter* lhs, FdoBinaryLogicalOperations operation, FdoFilter* rhs)
{
	return FdoBinaryLogicalOperator::Create(lhs, operation, rhs);
}

// Static operation that combines two filters using the specified binary
// logical operation.
FdoFilter* FdoFilter::Combine(FdoString* lhs, FdoBinaryLogicalOperations operation, FdoFilter* rhs)
{
	FdoFilter*	lhsFilter = FdoFilter::Parse(lhs);
	if (lhsFilter == NULL)
    {
        FdoFilterException* pExcept = FdoFilterException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_8_INVALIDFILTERSTRING)));
        throw pExcept;
    }

	FdoFilter*  pFilter = FdoBinaryLogicalOperator::Create(lhsFilter, operation, rhs);
	lhsFilter->Release();
    return pFilter;
}

// Static operation that combines two filters using the specified binary
// logical operation.
FdoFilter* FdoFilter::Combine(FdoFilter* lhs, FdoBinaryLogicalOperations operation, FdoString* rhs)
{
	FdoFilter*	rhsFilter = FdoFilter::Parse(rhs);
	if (rhsFilter == NULL)
    {
        FdoFilterException* pExcept = FdoFilterException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_8_INVALIDFILTERSTRING)));
        throw pExcept;
    }

	FdoFilter*  pFilter = FdoBinaryLogicalOperator::Create(lhs, operation, rhsFilter);
	rhsFilter->Release();
    return pFilter;
}

// Static operation that combines two filters using the specified binary
// logical operation.
FdoFilter* FdoFilter::Combine(FdoString* lhs, FdoBinaryLogicalOperations operation, FdoString* rhs)
{
	FdoFilter*	lhsFilter = FdoFilter::Parse(lhs);
	FdoFilter*	rhsFilter = FdoFilter::Parse(rhs);
	if (rhsFilter == NULL || lhsFilter == NULL)
	{
		FDO_SAFE_RELEASE(rhsFilter);
		FDO_SAFE_RELEASE(lhsFilter);

        FdoFilterException* pExcept = FdoFilterException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_8_INVALIDFILTERSTRING)));
        throw pExcept;
	}

	FdoFilter*  pFilter = FdoBinaryLogicalOperator::Create(lhsFilter, operation, rhsFilter);
	lhsFilter->Release();
	rhsFilter->Release();
    return pFilter;
}

FdoString* FdoFilter::ToString()
{
    FdoPtr<FdoIdentifierCollection> idenCol = FdoIdentifierCollection::Create();
    wchar_t*       identStr = FdoStringUtility::MakeString( ToStringInternal( idenCol ) );

    if( idenCol->GetCount() == 0 )
    {
        FdoStringUtility::ClearString(m_toString);
        m_toString = FdoStringUtility::MakeString( identStr );
        FdoStringUtility::ClearString(identStr);
        return m_toString;
    }
    else
    {
        wchar_t*     tmp;
        FdoStringUtility::ClearString(m_toString);
        for(int i=0; i<idenCol->GetCount(); i++ )
        {
            FdoPtr<FdoComputedIdentifier>id = (FdoComputedIdentifier*)idenCol->GetItem( i );
            tmp = m_toString;
            m_toString = FdoStringUtility::MakeString(m_toString, (i!=0)?L" ":L"", id->ToString(), L"," );
            FdoStringUtility::ClearString( tmp );
        }
        tmp = m_toString;
        m_toString = FdoStringUtility::MakeString(m_toString, L" ", identStr);
        FdoStringUtility::ClearString( tmp );
        FdoStringUtility::ClearString(identStr);
        return m_toString;
    }
}


