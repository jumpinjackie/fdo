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
#include <Fdo/Expression/Function.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

// Constructs a default instance of a Function.
FdoFunction* FdoFunction::Create()
{
	return new FdoFunction();
}

// Constructs an instance of a Function using the specified arguments.
FdoFunction* FdoFunction::Create(FdoString* name, FdoExpression** arguments, FdoInt32 numArgs)
{
	return new FdoFunction(name, arguments, numArgs);
}

// Constructs an instance of a Function using the specified arguments.
FdoFunction* FdoFunction::Create(FdoString* name, FdoExpressionCollection* arguments)
{
    FdoFunction*    pRet = FdoFunction::Create();

    FdoExpressionCollection*    pColl = NULL;
    FdoExpression*  pexpr = NULL;
    try
    {
        pRet->SetName(name);
        pColl = pRet->GetArguments();
	    FdoInt32		i;
        for (i = 0; i < arguments->GetCount(); i++)
        {
            pexpr = arguments->GetItem(i);
            pColl->Add(pexpr);
            FDO_SAFE_RELEASE(pexpr);
        }
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pexpr);
        FDO_SAFE_RELEASE(pColl);
        FDO_SAFE_RELEASE(pRet);
        throw pExcept;
    }

    pColl->Release();
    return pRet;
}

// Default destructor for Function.
FdoFunction::~FdoFunction()
{
    FdoStringUtility::ClearString(m_name);
    FDO_SAFE_RELEASE(ExpArguments);
}

void FdoFunction::Dispose()
{
    delete this;
}

// Constructs a default instance of a Function.
FdoFunction::FdoFunction() : FdoExpression()
{
	m_name = NULL;
	ExpArguments = FdoExpressionCollection::Create();
}

// Constructs an instance of a Function using the specified arguments.
FdoFunction::FdoFunction(FdoString* name, FdoExpression** arguments, FdoInt32 numArgs) : FdoExpression()
{
    m_name = FdoStringUtility::MakeString(name);
	ExpArguments = FdoExpressionCollection::Create();

    if (ExpArguments != NULL)
    {
	    FdoInt32		i;
	    for (i = 0; i < numArgs; i++)
		    ExpArguments->Add(arguments[i]);
    }
}

// Gets the name of the function.
FdoString* FdoFunction::GetName()
{
    return m_name;
}

// Sets the name of the function.
void FdoFunction::SetName(FdoString* value)
{
    FdoStringUtility::ClearString(m_name);
    m_name = FdoStringUtility::MakeString(value);
}

// Gets the ExpressionCollection that contains the arguments to the
// function.
FdoExpressionCollection* FdoFunction::GetArguments()
{
    return FDO_SAFE_ADDREF(ExpArguments);
}

// Overrides Expression.Process to pass the Function to the appropriate
// expression processor operation.
void FdoFunction::Process(FdoIExpressionProcessor* p)
{
    p->ProcessFunction(*this); 
}

FdoString* FdoFunction::ToString(  )
{
    return ToStringInternal( NULL );
}

// Returns the well defined text representation of this expression.
FdoString* FdoFunction::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    FdoStringUtility::ClearString(m_toString);

    FdoExpressionCollection*    pColl = GetArguments();
    if (pColl == NULL)
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_9_INCOMPLETEFUNCTION)));
        throw pExcept;
    }

    wchar_t*    szArgs = NULL;
    try
    {
        size_t  iLen = 3;   // 1 for EOS, 2 for parenthsis
        FdoString*  expression;

        bool    bFirst = true;
        FdoInt32     i;
        for (i = 0; i < pColl->GetCount(); i++)
        {
            FdoExpression*  pExpr;

            pExpr = pColl->GetItem(i);
            if (pExpr == NULL)
                continue;

            expression = pExpr->ToStringInternal( pIdCol );
            if (expression == NULL)
                continue;

            if (bFirst)
                bFirst = false;
            else
                iLen += 2;  // for ", "
            iLen += FdoStringUtility::StringLength(expression);
            pExpr->Release();
        }

        szArgs = new wchar_t[iLen];
        FdoStringUtility::StringCopy(szArgs, L"(");
        bFirst = true;
        for (i = 0; i < pColl->GetCount(); i++)
        {
            FdoExpression*  pExpr;

            pExpr = pColl->GetItem(i);
            if (pExpr == NULL)
                continue;

            expression = pExpr->ToString();
            if (expression == NULL)
                continue;

            if (bFirst)
                bFirst = false;
            else
                FdoStringUtility::StringConcatenate(szArgs, L", ");
            FdoStringUtility::StringConcatenate(szArgs, expression);
            pExpr->Release();
        }
        FdoStringUtility::StringConcatenate(szArgs, L")");

        m_toString = FdoStringUtility::MakeString(GetName(), szArgs);
    }
    catch (FdoException* pExcept)
    {
        if(szArgs != NULL)
            delete [] szArgs;
        FDO_SAFE_RELEASE(pColl);
        throw pExcept;
    }

    delete [] szArgs;
    FDO_SAFE_RELEASE(pColl);
    return m_toString;
}

