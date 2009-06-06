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
#include <Fdo/Filter/BinaryLogicalOperator.h>
#include <Fdo/Filter/IFilterProcessor.h>
#include "StringUtility.h"

// Constructs a default instance of a BinaryLogicalOperator.
FdoBinaryLogicalOperator* FdoBinaryLogicalOperator::Create()
{
	return new FdoBinaryLogicalOperator();
}

// Constructs an instance of a BinaryLogicalOperator using the specified
// arguments.
FdoBinaryLogicalOperator* FdoBinaryLogicalOperator::Create(FdoFilter* leftOperand, FdoBinaryLogicalOperations operation, FdoFilter* rightOperand)
{
	return new FdoBinaryLogicalOperator(leftOperand, operation, rightOperand);
}

// Constructs a default instance of a BinaryLogicalOperator.
FdoBinaryLogicalOperator::FdoBinaryLogicalOperator()
{
	m_operation = FdoBinaryLogicalOperations_And;
	m_leftOperand = NULL;
	m_rightOperand = NULL;
}

// Constructs an instance of a BinaryLogicalOperator using the specified
// arguments.
FdoBinaryLogicalOperator::FdoBinaryLogicalOperator(FdoFilter* leftOperand, FdoBinaryLogicalOperations operation, FdoFilter* rightOperand)
{
	m_operation = operation;
	m_leftOperand = FDO_SAFE_ADDREF(leftOperand);
	m_rightOperand = FDO_SAFE_ADDREF(rightOperand);
}

FdoBinaryLogicalOperator::~FdoBinaryLogicalOperator()
{
    FDO_SAFE_RELEASE(m_leftOperand);
    FDO_SAFE_RELEASE(m_rightOperand);
}

void FdoBinaryLogicalOperator::Dispose()
{
    delete this;
}

// Gets the operation to be performed between the two operands.
FdoBinaryLogicalOperations FdoBinaryLogicalOperator::GetOperation()
{
    return m_operation;
}

// Sets the operation to be performed between the two operands.
void FdoBinaryLogicalOperator::SetOperation(FdoBinaryLogicalOperations value)
{
	m_operation = value;
}

// Gets the Filter that defines the left operand.
FdoFilter* FdoBinaryLogicalOperator::GetLeftOperand()
{
    return FDO_SAFE_ADDREF(m_leftOperand);
}

// Sets the Filter that defines the left operand.
void FdoBinaryLogicalOperator::SetLeftOperand(FdoFilter* value)
{
	FDO_SAFE_RELEASE(m_leftOperand);
	m_leftOperand = FDO_SAFE_ADDREF(value);
}

// Gets the Filter that defines the right operand.
FdoFilter* FdoBinaryLogicalOperator::GetRightOperand()
{
    return FDO_SAFE_ADDREF(m_rightOperand);
}

// Sets the Filter that defines the right operand.
void FdoBinaryLogicalOperator::SetRightOperand(FdoFilter* value)
{
	FDO_SAFE_RELEASE(m_rightOperand);
	m_rightOperand = FDO_SAFE_ADDREF(value);
}

// Overrides Expression.Process to pass the BinaryLogicalOperator to the
// appropriate expression processor operation.
void FdoBinaryLogicalOperator::Process(FdoIFilterProcessor* p)
{
    p->ProcessBinaryLogicalOperator(*this); 
}

// Converts the filter expression to it's well defined text representation.
FdoString* FdoBinaryLogicalOperator::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    FdoStringUtility::ClearString(m_toString);

    FdoFilter*  pRight = GetRightOperand();
    FdoFilter*  pLeft = GetLeftOperand();
    if (pRight == NULL || pLeft == NULL)
    {
        FDO_SAFE_RELEASE(pRight);
        FDO_SAFE_RELEASE(pLeft);

        FdoFilterException* pExcept = FdoFilterException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_1_INCOMPLETEBINARYOPERATOR)));
        throw pExcept;
    }

    bool        bParenRight = false;
    bool        bParenLeft = false;
    FdoString*  pszRight = NULL;
    FdoString*  pszLeft = NULL;
    try
    {
        pszRight = pRight->ToStringInternal( pIdCol );
        pszLeft = pLeft->ToStringInternal( pIdCol );
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pRight);
        FDO_SAFE_RELEASE(pLeft);
        throw pExcept;
    }

    if (GetOperation() == FdoBinaryLogicalOperations_And)
    {
        FdoBinaryLogicalOperator*   pBLO = dynamic_cast<FdoBinaryLogicalOperator*>(pRight);
        if (pBLO != NULL && pBLO->GetOperation() == FdoBinaryLogicalOperations_Or)
        {
            bParenRight = true;
            pszRight = FdoStringUtility::MakeString(L"(", pszRight, L")");
        }

        pBLO = dynamic_cast<FdoBinaryLogicalOperator*>(pLeft);
        if (pBLO != NULL && pBLO->GetOperation() == FdoBinaryLogicalOperations_Or)
        {
            bParenLeft = true;
            pszLeft = FdoStringUtility::MakeString(L"(", pszLeft, L")");
        }
    }

    const wchar_t*    szOp;
    switch (GetOperation())
    {
    default:
    case FdoBinaryLogicalOperations_And:
        szOp = L" AND ";
        break;
    case FdoBinaryLogicalOperations_Or:
        szOp = L" OR ";
        break;
    }

    m_toString = FdoStringUtility::MakeString(pszLeft, szOp, pszRight);

    if (bParenLeft)
        delete [] pszLeft;
    if (bParenRight)
        delete [] pszRight;
    pLeft->Release();
    pRight->Release();
    return m_toString;
}

