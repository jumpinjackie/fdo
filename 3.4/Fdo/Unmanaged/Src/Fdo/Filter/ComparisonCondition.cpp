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
#include <Fdo/Filter/ComparisonCondition.h>
#include <Fdo/Filter/IFilterProcessor.h>
#include "StringUtility.h"

// Constructs a default instance of a ComparisonCondition.
FdoComparisonCondition* FdoComparisonCondition::Create()
{
	return new FdoComparisonCondition();
}

// Constructs an instance of a ComparisonCondition using the specified
// arguments.
FdoComparisonCondition* FdoComparisonCondition::Create(FdoExpression* leftExpression, FdoComparisonOperations operation, FdoExpression* rightExpression)
{
	return new FdoComparisonCondition(leftExpression, operation, rightExpression);
}

// Constructs a default instance of a ComparisonCondition.
FdoComparisonCondition::FdoComparisonCondition()
{
	m_leftExpression = NULL;
	m_rightExpression = NULL;
	m_operation = FdoComparisonOperations_EqualTo;
}

// Constructs an instance of a ComparisonCondition using the specified
// arguments.
FdoComparisonCondition::FdoComparisonCondition(FdoExpression* leftExpression, FdoComparisonOperations operation, FdoExpression* rightExpression)
{
	m_operation = operation;
	m_leftExpression = FDO_SAFE_ADDREF(leftExpression);
	m_rightExpression = FDO_SAFE_ADDREF(rightExpression);
}

FdoComparisonCondition::~FdoComparisonCondition()
{
    FDO_SAFE_RELEASE(m_leftExpression);
    FDO_SAFE_RELEASE(m_rightExpression);
}

void FdoComparisonCondition::Dispose()
{
    delete this;
}

// Gets the operation to be used to compare the two expressions.
FdoComparisonOperations FdoComparisonCondition::GetOperation()
{
    return m_operation;
}

// Sets the operation to be used to compare the two expressions.
void FdoComparisonCondition::SetOperation(FdoComparisonOperations operation)
{
	m_operation = operation;
}

// Gets the Expression that defines the left side of the comparison.
FdoExpression* FdoComparisonCondition::GetLeftExpression()
{
    return FDO_SAFE_ADDREF(m_leftExpression);
}

// Sets the Expression that defines the left side of the comparison.
void FdoComparisonCondition::SetLeftExpression(FdoExpression* value)
{
    FDO_SAFE_RELEASE(m_leftExpression);
	m_leftExpression = FDO_SAFE_ADDREF(value);
}

// Gets the Expression that defines the right side of the comparison.
FdoExpression* FdoComparisonCondition::GetRightExpression()
{
    return FDO_SAFE_ADDREF(m_rightExpression);
}

// Sets the Expression that defines the right side of the comparison.
void FdoComparisonCondition::SetRightExpression(FdoExpression* value)
{
    FDO_SAFE_RELEASE(m_rightExpression);
	m_rightExpression = FDO_SAFE_ADDREF(value);
}

// Overrides Expression.Process to pass the ComparisonCondition to the
// appropriate expression processor operation.
void FdoComparisonCondition::Process(FdoIFilterProcessor* p)
{
    p->ProcessComparisonCondition(*this); 
}

// Converts the filter expression to it's well defined text representation.
FdoString* FdoComparisonCondition::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    FdoStringUtility::ClearString(m_toString);

    FdoExpression*  pRight = GetRightExpression();
    FdoExpression*  pLeft = GetLeftExpression();
    if (pRight == NULL || pLeft == NULL)
    {
        FDO_SAFE_RELEASE(pRight);
        FDO_SAFE_RELEASE(pLeft);
        FdoFilterException* pExcept = FdoFilterException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_2_INCOMPLETECOMPARISONCONDITION)));
        throw pExcept;
    }

    wchar_t*    szOp;
    switch (GetOperation())
    {
    default:
    case FdoComparisonOperations_EqualTo:
        szOp = L" = ";
        break;
    case FdoComparisonOperations_NotEqualTo:
        szOp = L" <> ";
        break;
    case FdoComparisonOperations_GreaterThan:
        szOp = L" > ";
        break;
    case FdoComparisonOperations_GreaterThanOrEqualTo:
        szOp = L" >= ";
        break;
    case FdoComparisonOperations_LessThan:
        szOp = L" < ";
        break;
    case FdoComparisonOperations_LessThanOrEqualTo:
        szOp = L" <= ";
        break;
    case FdoComparisonOperations_Like:
        szOp = L" LIKE ";
        break;
    }

    try
    {
        m_toString = FdoStringUtility::MakeString(pLeft->ToStringInternal( pIdCol ), szOp, pRight->ToStringInternal( pIdCol ));
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pRight);
        FDO_SAFE_RELEASE(pLeft);
        throw pExcept;
    }

    pLeft->Release();
    pRight->Release();
    return m_toString;
}

