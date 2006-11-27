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
#include <Fdo/Expression/BinaryExpression.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

// Constructs a default instance of a BinaryExpression.
FdoBinaryExpression* FdoBinaryExpression::Create()
{
	return new FdoBinaryExpression();
}

// Constructs an instance of a BinaryExpression using the specified arguments.
FdoBinaryExpression* FdoBinaryExpression::Create(FdoExpression* leftExpression, FdoBinaryOperations operation, FdoExpression* rightExpression)
{
	return new FdoBinaryExpression(leftExpression, operation, rightExpression);
}

// Constructs a default instance of a BinaryExpression.
FdoBinaryExpression::FdoBinaryExpression()
{
	m_operation = (FdoBinaryOperations)0;
	m_leftExpr = NULL;
	m_rightExpr = NULL;
}


// Constructs an instance of a BinaryExpression using the specified arguments.
FdoBinaryExpression::FdoBinaryExpression(FdoExpression* leftExpression, FdoBinaryOperations operation, FdoExpression* rightExpression)
{
	m_operation = operation;
	m_leftExpr = FDO_SAFE_ADDREF(leftExpression);
	m_rightExpr = FDO_SAFE_ADDREF(rightExpression);
}

FdoBinaryExpression::~FdoBinaryExpression()
{
    FDO_SAFE_RELEASE(m_leftExpr);
    FDO_SAFE_RELEASE(m_rightExpr);
}

void FdoBinaryExpression::Dispose()
{
    delete this;
}

// Gets the binary operation to be evaluated by this expression.
FdoBinaryOperations FdoBinaryExpression::GetOperation()
{
	return m_operation;
}

// Sets the binary operation to be evaluated by this expression.
void FdoBinaryExpression::SetOperation(FdoBinaryOperations value)
{
	m_operation = value;
}

// Gets the Expression on the left side of the binary expression.
FdoExpression* FdoBinaryExpression::GetLeftExpression()
{
	return FDO_SAFE_ADDREF(m_leftExpr);
}

// Sets the Expression on the left side of the binary expression.
void FdoBinaryExpression::SetLeftExpression(FdoExpression* value)
{
	FDO_SAFE_RELEASE(m_leftExpr);
	m_leftExpr = FDO_SAFE_ADDREF(value);
}

// Gets the Expression on the right side of the binary expression.
FdoExpression* FdoBinaryExpression::GetRightExpression()
{
	return FDO_SAFE_ADDREF(m_rightExpr);
}

// Sets the Expression on the right side of the binary expression.
void FdoBinaryExpression::SetRightExpression(FdoExpression* value)
{
	FDO_SAFE_RELEASE(m_rightExpr);
	m_rightExpr = FDO_SAFE_ADDREF(value);
}

// Overrides Expression.Process to pass the BinaryOperation to the
// appropriate expression processor operation.
void FdoBinaryExpression::Process(FdoIExpressionProcessor* p)
{
    p->ProcessBinaryExpression(*this); 
}

FdoString* FdoBinaryExpression::ToString(  )
{
    return ToStringInternal( NULL );
}

// Returns the well defined text representation of this expression.
FdoString* FdoBinaryExpression::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    FdoStringUtility::ClearString(m_toString);

    FdoExpression*  pExprL = GetLeftExpression();
    FdoExpression*  pExprR = GetRightExpression();
    if (pExprL == NULL || pExprR == NULL)
    {
        FDO_SAFE_RELEASE(pExprL);
        FDO_SAFE_RELEASE(pExprR);

        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_8_INCOMPLETEBINARYEXPRESSION)));
        throw pExcept;
    }

    FdoString*      pszRight = NULL;
    FdoString*      pszLeft = NULL;
	wchar_t*		pszRightParen = NULL;
	wchar_t*		pszLeftParen = NULL;
    try
    {
        pszRight = pExprR->ToStringInternal( pIdCol );
        pszLeft = pExprL->ToStringInternal( pIdCol );
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pExprL);
        FDO_SAFE_RELEASE(pExprR);
        throw pExcept;
    }

    if (GetOperation() == FdoBinaryOperations_Multiply || 
        GetOperation() == FdoBinaryOperations_Divide)
    {
        FdoBinaryExpression*   pBE = dynamic_cast<FdoBinaryExpression*>(pExprR);
        if (pBE != NULL && (pBE->GetOperation() == FdoBinaryOperations_Add ||
            pBE->GetOperation() == FdoBinaryOperations_Subtract))
        {
            pszRightParen = FdoStringUtility::MakeString(L"(", pszRight, L")");
			pszRight = pszRightParen;
        }

        pBE = dynamic_cast<FdoBinaryExpression*>(pExprL);
        if (pBE != NULL && (pBE->GetOperation() == FdoBinaryOperations_Add ||
            pBE->GetOperation() == FdoBinaryOperations_Subtract))
        {
            pszLeftParen = FdoStringUtility::MakeString(L"(", pszLeft, L")");
			pszLeft = pszLeftParen;
        }
    }

    wchar_t*    szOp;
    switch (GetOperation())
    {
    default:
    case FdoBinaryOperations_Add:
        szOp = L"+";
        break;
    case FdoBinaryOperations_Subtract:
        szOp = L"-";
        break;
    case FdoBinaryOperations_Multiply:
        szOp = L"*";
        break;
    case FdoBinaryOperations_Divide:
        szOp = L"/";
        break;
    }

    m_toString = FdoStringUtility::MakeString(pszLeft, szOp, pszRight);
    FdoStringUtility::ClearString(pszLeftParen);
    FdoStringUtility::ClearString(pszRightParen);
    pExprL->Release();
    pExprR->Release();
    return m_toString;
}

