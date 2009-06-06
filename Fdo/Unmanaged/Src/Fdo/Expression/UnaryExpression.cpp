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
#include <Fdo/Expression/UnaryExpression.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

// Constructs a default instance of a UnaryExpression.
FdoUnaryExpression* FdoUnaryExpression::Create()
{
	return new FdoUnaryExpression();
}

// Constructs an instance of a UnaryExpression using the specified arguments.
FdoUnaryExpression* FdoUnaryExpression::Create(FdoUnaryOperations operation, FdoExpression* expression)
{
	return new FdoUnaryExpression(operation, expression);
}

// Constructs a default instance of a UnaryExpression.
FdoUnaryExpression::FdoUnaryExpression()
{
	m_operation = FdoUnaryOperations_Negate;
	m_expr = NULL;
}

// Constructs an instance of a UnaryExpression using the specified arguments.
FdoUnaryExpression::FdoUnaryExpression(FdoUnaryOperations operation, FdoExpression* expression)
{
	m_operation = operation;
	m_expr = FDO_SAFE_ADDREF(expression);
}

FdoUnaryExpression::~FdoUnaryExpression()
{
    FDO_SAFE_RELEASE(m_expr);
}

void FdoUnaryExpression::Dispose()
{
    delete this;
}


// Gets the unary operation to be evaluated by this expression.
FdoUnaryOperations FdoUnaryExpression::GetOperation()
{
	return m_operation;
}

// Sets the unary operation to be evaluated by this expression.
void FdoUnaryExpression::SetOperation(FdoUnaryOperations value)
{
	m_operation = value;
}

// Gets the Expression to be operated on.
FdoExpression* FdoUnaryExpression::GetExpression()
{
	return FDO_SAFE_ADDREF(m_expr);
}

// Sets the Expression to be operated on.
void FdoUnaryExpression::SetExpression(FdoExpression* value)
{
	FDO_SAFE_RELEASE(m_expr);
	m_expr = FDO_SAFE_ADDREF(value);
}

// Overrides Expression.Process to pass the UnaryExpression to the
// appropriate expression processor operation.
void FdoUnaryExpression::Process(FdoIExpressionProcessor* p)
{ 
    p->ProcessUnaryExpression(*this); 
}

FdoString* FdoUnaryExpression::ToString(  )
{
    return ToStringInternal( NULL );
}

// Returns the well defined text representation of this expression.
FdoString* FdoUnaryExpression::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    FdoStringUtility::ClearString(m_toString);

    FdoExpression*  pExpression = GetExpression();
    if (pExpression == NULL)
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_11_INCOMPLETEUNARYEXPRESSION)));
        throw pExcept;
    }

    const wchar_t*    szOp;
    switch (GetOperation())
    {
    default:
    case FdoUnaryOperations_Negate:
        szOp = L"-("; 
        break;
    }

    FdoString*  pszExpr = NULL;
    try
    {
        pszExpr = pExpression->ToStringInternal( pIdCol );
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pExpression);
        throw pExcept;
    }

    m_toString = FdoStringUtility::MakeString(szOp, pszExpr, L")");
    pExpression->Release();
    return m_toString;
}

