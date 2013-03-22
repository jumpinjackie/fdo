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
#include <Fdo/Filter/UnaryLogicalOperator.h>
#include <Fdo/Filter/IFilterProcessor.h>
#include "StringUtility.h"

// Constructs a default instance of an UnaryLogicalOperator.
FdoUnaryLogicalOperator* FdoUnaryLogicalOperator::Create()
{
	return new FdoUnaryLogicalOperator();
}

// Constructs an instance of a UnaryLogicalOperator using the specified
// arguments.
FdoUnaryLogicalOperator* FdoUnaryLogicalOperator::Create(FdoFilter* operand, FdoUnaryLogicalOperations operation)
{
	return new FdoUnaryLogicalOperator(operand, operation);
}

// Constructs a default instance of an UnaryLogicalOperator.
FdoUnaryLogicalOperator::FdoUnaryLogicalOperator()
{
	m_operand = NULL;
	m_operation = FdoUnaryLogicalOperations_Not;
}

// Constructs an instance of a UnaryLogicalOperator using the specified
// arguments.
FdoUnaryLogicalOperator::FdoUnaryLogicalOperator(FdoFilter* operand, FdoUnaryLogicalOperations operation)
{
	m_operation = operation;
    m_operand = FDO_SAFE_ADDREF(operand);
}

FdoUnaryLogicalOperator::~FdoUnaryLogicalOperator()
{
    FDO_SAFE_RELEASE(m_operand);
}

void FdoUnaryLogicalOperator::Dispose()
{
    delete this;
}

// Gets the unary operation to be performed on the operand.
FdoUnaryLogicalOperations FdoUnaryLogicalOperator::GetOperation()
{
    return m_operation;
}

// Sets the unary operation to be performed on the operand.
void FdoUnaryLogicalOperator::SetOperation(FdoUnaryLogicalOperations value)
{
	m_operation = value;
}

// Gets the Filter operand to apply the operator to.
FdoFilter* FdoUnaryLogicalOperator::GetOperand()
{
    return FDO_SAFE_ADDREF(m_operand);
}

// Sets the Filter operand to apply the operator to.
void FdoUnaryLogicalOperator::SetOperand(FdoFilter* value)
{
    FDO_SAFE_RELEASE(m_operand);
	m_operand = FDO_SAFE_ADDREF(value);
}

// Overrides Expression.Process to pass the UnaryLogicalOperator to the
// appropriate expression processor operation.
void FdoUnaryLogicalOperator::Process(FdoIFilterProcessor* p)
{
    p->ProcessUnaryLogicalOperator(*this); 
}

// Converts the filter expression to it's well defined text representation.
FdoString* FdoUnaryLogicalOperator::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    FdoStringUtility::ClearString(m_toString);

    FdoFilter*  pOperand = GetOperand();
    if (pOperand == NULL)
    {
        FdoFilterException* pExcept = FdoFilterException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_7_INCOMPLETEUNARYOPERATOR)));
        throw pExcept;
    }

    try
    {
        m_toString = FdoStringUtility::MakeString(L"NOT (", pOperand->ToStringInternal(pIdCol), L")");
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pOperand);
        throw pExcept;
    }

    pOperand->Release();
    return m_toString;
}

