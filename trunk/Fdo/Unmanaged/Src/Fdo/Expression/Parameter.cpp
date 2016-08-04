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
#include <Fdo/Expression/Parameter.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

// Constructs a default instance of a Parameter.
FdoParameter* FdoParameter::Create()
{
	return new FdoParameter();
}

// Constructs an instance of a Parameter using the specified arguments.
FdoParameter* FdoParameter::Create(FdoString* name)
{
	return new FdoParameter(name);
}

FdoParameter::~FdoParameter()
{
    FdoStringUtility::ClearString(m_name);
}

void FdoParameter::Dispose()
{
    delete this;
}

// Constructs a default instance of a Parameter.
FdoParameter::FdoParameter()
{
	m_name = NULL;
}

// Constructs an instance of a Parameter using the specified arguments.
FdoParameter::FdoParameter(FdoString* name)
{
	m_name = NULL;
	SetName(name);
}

// Gets the name of the parameter.
FdoString* FdoParameter::GetName()
{
	return m_name;
}

// Gets the name of the parameter.
void FdoParameter::SetName(FdoString* value)
{
    FdoStringUtility::ClearString(m_name);
    m_name = FdoStringUtility::MakeString(value);
}

// Overrides Expression.Process to pass the Parameter to the appropriate
// expression processor operation.
void FdoParameter::Process(FdoIExpressionProcessor* p)
{ 
    p->ProcessParameter(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoParameter::ToString()
{
    FdoStringUtility::ClearString(m_toString);

    // if NULL string or no quotes or spaces, then don't quote.
    if (GetName() == NULL)
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_10_INCOMPLETEPARAMETER)));
        throw pExcept;
    }

    if (FdoStringUtility::FindCharacter(GetName(), L' ') == NULL &&
        FdoStringUtility::FindCharacter(GetName(), L'\'') == NULL)
        m_toString = FdoStringUtility::MakeString(L":", GetName());
    else
    {
        wchar_t*    pszTemp = FdoStringUtility::QuoteString(GetName(), L'\'');
        m_toString = FdoStringUtility::MakeString(L":", pszTemp);
        FdoStringUtility::ClearString(pszTemp);
    }
	return m_toString;
}

